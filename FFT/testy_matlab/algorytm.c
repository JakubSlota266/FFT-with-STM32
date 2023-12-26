
#include <stdio.h>
#include <string.h>
#include <complex.h>
#include <math.h>
#include <time.h>

void dft(float complex * input, float complex * output, int N)
{
    for (int k = 0; k < N; ++k)
    {
        output[k] = 0;
        for (int n = 0; n < N; ++n) 
        {
            float complex exp_term = cexp(-2 * I * M_PI * k * n / N);
            output[k] += input[n] * exp_term;
        }
    }
}

void fft(float complex * x, int N)
{
    if (N <= 1) return;
    else
    {
        float complex even[N/2];
        float complex odd[N/2];

        for (int i = 0; i < N/2; i++)
        {
            even[i] = x[2*i];
            odd[i] = x[2*i+1];
        }

        fft(even, N/2);
        fft(odd, N/2);

        for (int k = 0; k < N/2; k++)
        {
            float complex exp_term = cexp(-2.0 * I * M_PI * k / N);
            x[k] = even[k] + exp_term*odd[k];
            x[k+N/2] = even[k] - exp_term*odd[k];
        }
    }
}

#define N 10240
int main()
{
    complex float x[N];
    complex float X1[N];
    complex float X2[N];
    memset(x, (complex float)0, N*sizeof(complex float));

    /* Wczytanie danych z Matlaba */
    FILE * file;
    file = fopen("matrix.txt", "r");
    
    int i = 0;
    while(fscanf(file, "%f", &x[i]) != EOF)
    {
        //printf("%f\n",x[i]);
        i++;
    }
    fclose(file);

    clock_t tic, tac;
    /* Wyliczenie DFT */
    tic = clock();
    dft(x, X1, N);
    double exec_time_dft = (double)(clock() - tic) / CLOCKS_PER_SEC;
    /* Wyliczenie FFT */
    memcpy(X2, x, N*sizeof(complex float));
    tic = clock();
    fft(X2, N);
    double exec_time_fft = (double)(clock() - tic) / CLOCKS_PER_SEC;

    /* Zapis wyniku do pliku */
    file = fopen("wynik.txt", "w");
    
    for (int i=0; i<N; i++)
    {
        fprintf(file, "%f, %f, %f, %f\n", creal(X1[i]), cimag(X1[i]), creal(X2[i]), cimag(X2[i]));
    }
    fprintf(file, "%.6f, %.6f\n", exec_time_dft, exec_time_fft);
    fclose(file);

    return 0;
}