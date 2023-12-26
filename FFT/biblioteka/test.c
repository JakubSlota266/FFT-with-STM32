#include "fft_lib.h"

#define N 16384

int main()
{
    complex float x[N];
    float X[FFT_WINDOW];
    float Xf[FREQ_INTERVALS];
    memset(x, (complex float)0, N*sizeof(complex float));

    /* Wczytanie danych z Matlaba */
    FILE *file, *file1, *file2;
    file = fopen("matrix.txt", "r");
    
    int i = 0;
    while(fscanf(file, "%f", &x[i++]) != EOF) {}
    fclose(file);

    file1 = fopen("wynik_abs.txt", "w");
    file2 = fopen("wynik_interval.txt", "w");
    fclose(file1);
    fclose(file2);
    file1 = fopen("wynik_abs.txt", "a");
    file2 = fopen("wynik_interval.txt", "a");

    for (uint16_t i=0; i<N; i++)
    {
        FFT_update_buffer(x[i]);

        if (false != FFT_check_done_flag())
        {
            FFT_clear_done_flag();
            FFT_read_abs_results(X);
            FFT_read_intervals_results(Xf);

            /* Zapis wyniku do plikow */
            for (int i=0; i<FFT_WINDOW/2; i++)
            {
                fprintf(file1, "%f\n", X[i]);
            }
            for (int i=0; i<FREQ_INTERVALS; i++)
            {
                fprintf(file2, "%f\n", Xf[i]);
            }
        }
    }

    fprintf(file1, "%d\n", FFT_WINDOW);
    fclose(file1);
    fclose(file2);

    return 0;
}