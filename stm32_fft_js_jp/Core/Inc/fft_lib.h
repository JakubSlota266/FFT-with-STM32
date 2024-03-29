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
#define FFT_WINDOW          (256U)
#define SAMPLES_OVERLAP     (128U)
#define SAMPLING_FREQ       (96000U)
#define FREQ_INTERVALS      (8U)        /* specify how the resulting audio band should be divided into n smaller bands */
#define AMPLITIUDE_LEVELS	(8U)		/* specify how they should be scaled (max value and number of levels) */
#define AMPLITIUDE_MAX_VAL	(0U)		/* leave it as 0 if the intervals values should be calculated adaptively */
#define FILTER_TYPE			(2U)		/* choose filter type to be applied for the input digital audio signal
										(0 - no filter, 1 - moving average filter, 2 - non-recursive median filter) */
#define FILTER_WINDOW		(4U)

/*============================================================*/
/* EXPORTED OBJECTS */
typedef struct
{
#if (((FILTER_TYPE == 1U) || (FILTER_TYPE == 2U)) && (FILTER_WINDOW > 0U))
	int16_t filter_buffer[FILTER_WINDOW];
	uint8_t filter_buffer_idx;
#endif
	float complex input_buffer[FFT_WINDOW];
	uint16_t input_buffer_idx;
	float complex result_raw_buffer[FFT_WINDOW];
	float result_abs_buffer[FFT_WINDOW/2];
	int result_intervals_buffer[FREQ_INTERVALS];
} FFT_Data_T;

extern FFT_Data_T FFT_Data;

/*============================================================*/
/* EXPORTED FUNCTIONS DECLARATIONS */
int16_t FFT_apply_filter(int16_t new_value);
void FFT_update_buffer(int16_t new_value);
