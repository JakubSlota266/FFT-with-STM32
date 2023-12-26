/* LIBRARIES */
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <complex.h>
#undef __STRICT_ANSI__
#include <math.h>

/*============================================================*/
/* DEFINES */
#define FFT_WINDOW          (1024U)
#define SAMPLES_OVERLAP     (512U)
#define SAMPLING_FREQ       (60000U)
#define FREQ_INTERVALS      (6U)        /* specify how the resulting audio band should be divided into n smaller bands */

/*============================================================*/
/* EXPORTED FUNCTIONS DECLARATIONS */
void FFT_update_buffer(float new_value);
bool FFT_check_done_flag(void);
void FFT_clear_done_flag(void);
void FFT_read_complex_results(float complex * result);
void FFT_read_abs_results(float * result);
void FFT_read_intervals_results(float * result);
