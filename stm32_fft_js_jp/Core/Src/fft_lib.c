/*============================================================*/
/* LIBRARIES */
#include "fft_lib.h"

/*============================================================*/
/* DEFINES */

/*============================================================*/
/* STATIC FUNCTIONS DECLARATIONS */
static inline void calc_fft(float complex * x, uint16_t N);
static inline void calc_intervals_fft(void);
static inline void swap(int16_t * a, int16_t * b);

/*============================================================*/
/* OBJECTS DECLARATIONS */
FFT_Data_T FFT_Data =
{
	.input_buffer_idx = 0U,
};

static uint16_t intervals_ranges[FREQ_INTERVALS + 1U] =
{
    200U,
    2500U,
    5000U,
    7500U,
    10000U,
    12500U,
	15000U,
	17500U,
    20000U,
};

static float intervals_prescalers[FREQ_INTERVALS] =
{
	1.0f,
	1.0f,
	1.0f,
	1.0f,
	1.0f,
	1.0f,
	1.0f,
	1.0f,
};

/*============================================================*/
/* STATIC FUNCTIONS DEFINIONS */
static inline void calc_fft(float complex * x, uint16_t N)
{
	if (N <= 1) return;
	else
	{
		float complex even[N/2];
		float complex odd[N/2];

		for (uint16_t i = 0; i < N/2; i++)
		{
			even[i] = x[2*i];
			odd[i] = x[2*i+1];
		}

		calc_fft(even, N/2);
		calc_fft(odd, N/2);

		for (uint16_t k = 0; k < N/2; k++)
		{
			float complex exp_term = cexp(-2.0 * I * M_PI * k / N);
			x[k] = even[k] + exp_term*odd[k];
			x[k+N/2] = even[k] - exp_term*odd[k];
		}
	}
}

/**********/

static inline void calc_intervals_fft(void)
{
    uint8_t idx = 0U;
    float temp_sum[FREQ_INTERVALS] = {0.0f};
    float temp_val[FREQ_INTERVALS] = {0.0f};
    float max_val = 0.0f;

    for (uint16_t i=(intervals_ranges[0U]*FFT_WINDOW/SAMPLING_FREQ); i<FFT_WINDOW/2; i++)
    {
    	FFT_Data.result_abs_buffer[i] = (float)cabs(FFT_Data.result_raw_buffer[i]);
        temp_sum[idx] += FFT_Data.result_abs_buffer[i];

        if (i*SAMPLING_FREQ/FFT_WINDOW >= intervals_ranges[idx+1])
        {
        	temp_val[idx] = temp_sum[idx] / (float)(intervals_ranges[idx+1] - intervals_ranges[idx]) * intervals_prescalers[idx];
			if (temp_val[idx] > max_val)
			{
				max_val = temp_val[idx];
			}
            if (idx < (FREQ_INTERVALS-1U))
            {
                idx++;
            }
            else
            {
                break;
            }
        }
    }

    for (uint8_t i = 0U; i < FREQ_INTERVALS; i++)
    {
		#if (AMPLITIUDE_MAX_VAL == 0U)
    	FFT_Data.result_intervals_buffer[i] = (int)(temp_val[i] * (float)AMPLITIUDE_LEVELS / max_val);
		#else
    	FFT_Data.result_intervals_buffer[i] = (int)(temp_val[i] * (float)AMPLITIUDE_LEVELS / (float)AMPLITIUDE_MAX_VAL);
		#endif
    }
}

/**********/

static inline void swap(int16_t * a, int16_t * b)
{
	int16_t temp = *a;
	*a = *b;
	*b = temp;
}

/*============================================================*/
/* EXPORTED FUNCTIONS DEFINIONS */
int16_t FFT_apply_filter(int16_t new_value)
{
#if (((FILTER_TYPE != 1U) && (FILTER_TYPE != 2U)) || (FILTER_WINDOW == 0U))
	return new_value;
#else
	static bool buffer_filled = false;
	#if (FILTER_TYPE == 1U) /* Moving average filter */
		FFT_Data.filter_buffer[FFT_Data.filter_buffer_idx++] = new_value;
		if (FFT_Data.filter_buffer_idx >= FILTER_WINDOW)
		{
			FFT_Data.filter_buffer_idx = 0U;
			buffer_filled = true;
		}

		if (false == buffer_filled)		/* Do not use filter before the buffer is filled */
		{
			return new_value;
		}
		else
		{
			int32_t sum = 0;
			for (uint8_t i = 0U; i < FILTER_WINDOW; i++)
			{
				sum += FFT_Data.filter_buffer[i];
			}
			return (sum / FILTER_WINDOW);
		}
	#elif (FILTER_TYPE == 2U) /* Median filter */
		if (false == buffer_filled)
		{
			FFT_Data.filter_buffer[FFT_Data.filter_buffer_idx++] = new_value;
			if (FFT_Data.filter_buffer_idx >= FILTER_WINDOW)
			{
				buffer_filled = true;
			}
			return new_value;
		}
		else
		{
			/* Delete the oldest value by shifting buffer and write a new one */
			memmove(&FFT_Data.filter_buffer[0U],&FFT_Data.filter_buffer[1U],(FILTER_WINDOW-1U)*sizeof(int16_t));
			FFT_Data.filter_buffer[FILTER_WINDOW-1U] = new_value;
			/* Declare another buffer to store data to be sorted */
			int16_t temp_buffer[FILTER_WINDOW];
			memcpy(temp_buffer,FFT_Data.filter_buffer,(FILTER_WINDOW)*sizeof(int16_t));
			/* Use comb sort algorithm to sort data */
			uint8_t gap = FILTER_WINDOW;
			bool was_swapped = false;
			while ((gap > 1U) || (was_swapped))
			{
				/* Change comb gap size in each iteration until it's equal 1 */
				was_swapped = false;
				gap = gap * 10U / 13U;
				if (gap == 0U) gap = 1U;
				for (uint8_t idx = 0U; idx + gap < FILTER_WINDOW; idx++)
				{
					if (temp_buffer[idx + gap] < temp_buffer[idx])
					{
						swap(&temp_buffer[idx + gap],&temp_buffer[idx]);
						was_swapped = true;
					}
				}
			}
			/* Return filtered value */
			if ((FILTER_WINDOW % 2) == 0U)
			{
				return ((temp_buffer[FILTER_WINDOW/2U] + temp_buffer[(FILTER_WINDOW/2U)-1U]) / 2U);
			}
			else
			{
				return temp_buffer[FILTER_WINDOW/2U];
			}
		}
	#endif
#endif
}

/**********/

void FFT_update_buffer(int16_t new_value)
{
    FFT_Data.input_buffer[FFT_Data.input_buffer_idx++] = (float complex)new_value;

    if (FFT_Data.input_buffer_idx >= FFT_WINDOW)
    {
        memcpy(FFT_Data.result_raw_buffer, FFT_Data.input_buffer, sizeof(float complex)*FFT_WINDOW);
        if (SAMPLES_OVERLAP > 0)
        {
            memcpy(FFT_Data.input_buffer, &FFT_Data.input_buffer[FFT_WINDOW-SAMPLES_OVERLAP], sizeof(float complex)*SAMPLES_OVERLAP);
        }
        FFT_Data.input_buffer_idx = SAMPLES_OVERLAP;
        calc_fft(FFT_Data.result_raw_buffer, FFT_WINDOW);
        calc_intervals_fft();
    }
}
