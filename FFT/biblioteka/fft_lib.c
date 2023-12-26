/*============================================================*/
/* LIBRARIES */
#include "fft_lib.h"

/*============================================================*/
/* DEFINES */

/*============================================================*/
/* STATIC FUNCTIONS DECLARATIONS */
static void calc_fft(float complex * x, uint16_t N);
static void calc_abs_fft(void);
static void calc_intervals_fft(void);

/*============================================================*/
/* OBJECTS DECLARATIONS */
static float complex input_buffer[FFT_WINDOW];
static float complex result_raw_buffer[FFT_WINDOW];
static float result_abs_buffer[FFT_WINDOW/2];
static float result_intervals_buffer[FREQ_INTERVALS];
static bool fft_done_flag = false;

static uint16_t intervals_ranges[FREQ_INTERVALS+1] =
{
    0U,
    125U,
    500U,
    2000U,
    4000U,
    8000U,
    20000U,
};

/*============================================================*/
/* STATIC FUNCTIONS DEFINIONS */
static void calc_fft(float complex * x, uint16_t N)
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

static void calc_abs_fft(void)
{
    for (uint16_t i=0; i<FFT_WINDOW/2; i++)
    {
        result_abs_buffer[i] = cabs(result_raw_buffer[i]);
    }
}

/**********/

static void calc_intervals_fft(void)
{
    uint8_t interval_idx = 0;
    float temp_sum[FREQ_INTERVALS] = {0.0};

    for (uint16_t i=0; i<FFT_WINDOW/2; i++)
    {
        temp_sum[interval_idx] += result_abs_buffer[i];

        if (i*SAMPLING_FREQ/FFT_WINDOW >= intervals_ranges[interval_idx+1])
        {
            if (interval_idx < (FREQ_INTERVALS-1))
            {
                interval_idx++;
            }
            else
            {
                break;
            }
        }
    }

    float temp_max = 0;
    for (uint8_t i=0; i<FREQ_INTERVALS; i++)
    {
        result_intervals_buffer[i] = temp_sum[i] / (float)(intervals_ranges[interval_idx+1] - intervals_ranges[interval_idx]);
        if (result_intervals_buffer[i] > temp_max)
        {
            temp_max = result_intervals_buffer[i];
        }
    }
    for (uint8_t i=0; i<FREQ_INTERVALS; i++)
    {
        result_intervals_buffer[i] = result_intervals_buffer[i] * 1000.0 / temp_max;
    }
}

/*============================================================*/
/* EXPORTED FUNCTIONS DEFINIONS */
void FFT_update_buffer(float new_value)
{
    static uint16_t buffer_idx = 0;

    input_buffer[buffer_idx++] = (float complex)new_value;

    if (buffer_idx >= FFT_WINDOW)
    {
        memcpy(result_raw_buffer, input_buffer, sizeof(float complex)*FFT_WINDOW);
        if (SAMPLES_OVERLAP > 0)
        {
            memcpy(input_buffer, &input_buffer[FFT_WINDOW-SAMPLES_OVERLAP+1], sizeof(float complex)*SAMPLES_OVERLAP);
        }
        buffer_idx = SAMPLES_OVERLAP;
        calc_fft(result_raw_buffer, FFT_WINDOW);
        calc_abs_fft();
        calc_intervals_fft();
        fft_done_flag = true;
    }
}

/**********/

bool FFT_check_done_flag(void)
{
    return fft_done_flag;
}

/**********/

void FFT_clear_done_flag(void)
{
    fft_done_flag = false;
}

/**********/

void FFT_read_complex_results(float complex * result)
{
    memcpy(result, result_raw_buffer, sizeof(float complex)*FFT_WINDOW);
}

/**********/

void FFT_read_abs_results(float * result)
{
    memcpy(result, result_abs_buffer, sizeof(float)*FFT_WINDOW/2);
}

/**********/

void FFT_read_intervals_results(float * result)
{
    memcpy(result, result_intervals_buffer, sizeof(float)*FREQ_INTERVALS);
}