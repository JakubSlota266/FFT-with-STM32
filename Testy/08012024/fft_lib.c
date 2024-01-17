/*============================================================*/
/* LIBRARIES */
#include "fft_lib.h"

/*============================================================*/
/* DEFINES */

/*============================================================*/
/* STATIC FUNCTIONS DECLARATIONS */
static void calc_fft(float complex * x, uint16_t N);
static void calc_intervals_fft(void);

/*============================================================*/
/* OBJECTS DECLARATIONS */
FFT_Data_T FFT_Data =
{
	.input_buffer_idx = 0U,
};

static uint16_t intervals_ranges[FREQ_INTERVALS + 1U] =
{
    0U,
    2000U,
    4000U,
    6000U,
    8000U,
    10000U,
	12000U,
	14000U,
    16000U,
};

static float intervals_prescalers[FREQ_INTERVALS] =
{
	1.0f,
	1.38f,
	1.0f,
	1.0f,
	1.38f,
	1.0f,
	1.0f,
	1.38f,
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

/*============================================================*/
/* EXPORTED FUNCTIONS DEFINIONS */
int16_t FFT_apply_filter(int16_t new_value)
{
#if (MVG_AVG_WINDOW > 0)
	/* Circular buffer is used */
	static bool buffer_filled = false;
	int32_t sum = 0;

	FFT_Data.mvg_avg_buffer[FFT_Data.mvg_avg_idx++] = new_value;
	if (FFT_Data.mvg_avg_idx >= MVG_AVG_WINDOW)
	{
		FFT_Data.mvg_avg_idx = 0U;
		buffer_filled = true;
	}

	if (false == buffer_filled)		/* Do not use filter before the buffer is filled */
	{
		return new_value;
	}
	else
	{
		for (uint8_t i = 0U; i < MVG_AVG_WINDOW; i++)
		{
			sum += FFT_Data.mvg_avg_buffer[i];
		}
		return (sum / MVG_AVG_WINDOW);
	}
#else
	return new_value;
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
