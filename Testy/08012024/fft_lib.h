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
#define SAMPLES_OVERLAP     (0U)
#define SAMPLING_FREQ       (96000U)
#define FREQ_INTERVALS      (8U)        /* specify how the resulting audio band should be divided into n smaller bands */
#define AMPLITIUDE_LEVELS	(8U)		/* specify how they should be scaled (max value and number of levels) */
#define AMPLITIUDE_MAX_VAL	(0U)		/* leave it as 0 if the intervals values should be calculated adaptively */
#define MVG_AVG_WINDOW		(0U)

/*============================================================*/
/* EXPORTED OBJECTS */
typedef struct
{
	int16_t mvg_avg_buffer[MVG_AVG_WINDOW];
	uint8_t mvg_avg_idx;
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
