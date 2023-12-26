load('pomiarykodek1kHz.mat');

figure();
plot(pomiarykodek1kHz); grid on;
xlabel('Samples'); ylabel('Amplitude');

signal_fft = abs(fft(pomiarykodek1kHz));
Fs = 96000;
n = length(signal_fft);
f = 0:(Fs/n):Fs-Fs/n;
figure();
plot(f(1:end/2), signal_fft(1:end/2)); grid on;
