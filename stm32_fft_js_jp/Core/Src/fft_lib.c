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
    float temp_sum[FREQ_INTERVALS] = {0.0};
    static float max_val = AMPLITIUDE_MAX_VAL;

    for (uint16_t i=(intervals_ranges[0U]*FFT_WINDOW/SAMPLING_FREQ); i<FFT_WINDOW/2; i++)
    {
    	FFT_Data.result_abs_buffer[i] = cabs(FFT_Data.result_raw_buffer[i]);
        temp_sum[idx] += FFT_Data.result_abs_buffer[i];

        if (i*SAMPLING_FREQ/FFT_WINDOW >= intervals_ranges[idx+1])
        {
            if (idx <= (FREQ_INTERVALS-1))
            {
            	float val_temp = temp_sum[idx] / (float)(intervals_ranges[idx+1] - intervals_ranges[idx]);
            	if (val_temp > max_val)
            	{
            		max_val = val_temp;
            	}
            	FFT_Data.result_intervals_buffer[idx] = (int)(val_temp * AMPLITIUDE_LEVELS / max_val);
                idx++;
            }
            else
            {
                break;
            }
        }
    }
}

/*============================================================*/
/* EXPORTED FUNCTIONS DEFINIONS */
void FFT_update_buffer(float new_value)
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
