%% Wczytuje dane z wynikowego pliku
load("wynik_abs.txt");
load("wynik_interval.txt");
FFT_WINDOW = wynik_abs(end);

%% Budowa wektora częsototliowści
fs = zeros(FFT_WINDOW/2,1);
for i = 1:FFT_WINDOW/2
    fs(i) = (i-1) * Fs/FFT_WINDOW;
end

%% Wizualizacja
series = (length(wynik_abs)-1)/(FFT_WINDOW/2);
subplot_dim = ceil(sqrt(series));
intervals = categorical({'0-125','125-500','500-2k','2k-4k','4k-8k','8k-20k'});
intervals = reordercats(intervals,{'0-125','125-500','500-2k','2k-4k','4k-8k','8k-20k'});

figure();
for i=1:series
    subplot(subplot_dim,subplot_dim,i); plot(fs,wynik_abs((1+(i-1)*FFT_WINDOW/2):(i*FFT_WINDOW/2))); grid on;
end

figure();
for i=1:series
    subplot(subplot_dim,subplot_dim,i); bar(intervals,wynik_interval(1+(i-1)*6:i*6)); grid on;
end

