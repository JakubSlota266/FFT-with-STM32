%% DANE DO SPRWADZENIA PRZED WŁĄCZENIEM:
N = 10240;
Fs = 1000;
%%
flag = false;

%% Budowa wektora częsototliowści
fs = zeros(N,1);
for i = 1:1:N
    fs(i) = (i-1) * Fs/N;
end

%% Wczytuje dane z wynikowego pliku z algorytmu.exe
wyj_alg_c = dlmread('wynik.txt');
real_dft = wyj_alg_c(1:end-1,1);
imag_dft = wyj_alg_c(1:end-1,2);
real_fft = wyj_alg_c(1:end-1,3);
imag_fft = wyj_alg_c(1:end-1,4);
t_dft = wyj_alg_c(end,1);
t_fft = wyj_alg_c(end,2);

%% Filtr na częstotliwości Nyquista
%{
nyquist_freq = Fs/2;
for i = 1:1:N
    if fs(i) >= nyquist_freq 
        if(~flag)
           nyquist_index= i;
           flag = true;
        end
        real(i) = 0;
        imag(i) = 0;
    end
end
%}

%% Przejście na zespolone matlabowe
z_dft = complex(real_dft, imag_dft);
z_fft = complex(real_fft, imag_fft);
fs(N/2+1:end) = fs(N/2+1:end) - Fs;

M_dft = abs(z_dft)./(N/2);
M_fft = abs(z_fft)./(N/2);

figure();
subplot(2,1,1);
plot(fs,M_dft);
title("Moduł DFT z algorytmu C")
subtitle(["N = ", num2str(N), ", t_{exec} = ", num2str(t_dft)]);
xlabel("Częstotliowść [Hz]")
ylabel("Amplituda [-]")
grid on;
subplot(2,1,2);
plot(fs,M_fft);
title("Moduł DFT z algorytmu C")
subtitle(["N = ", num2str(N), ", t_{exec} = ", num2str(t_fft)]);
xlabel("Częstotliowść [Hz]")
ylabel("Amplituda [-]")
grid on;
