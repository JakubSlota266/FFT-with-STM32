#include "fft_lib.h"
#include "stdlib.h"

#define SIGNAL_SIZE 2048
static float signal[SIGNAL_SIZE] = {0};
static int16_t temp_data_filt = 0;

int main(void)
{
    FILE *file;
    file = fopen("signal_mixed.txt", "r");
    
    uint16_t i = 0U;
    while(fscanf(file, "%f", &signal[i++]) != EOF) {}
    fclose(file);

    i = 0U;
    while (i < SIGNAL_SIZE)
    {
        temp_data_filt = FFT_apply_filter((int16_t)signal[i++]);
        FFT_update_buffer(temp_data_filt);
    }

    return 0;
}