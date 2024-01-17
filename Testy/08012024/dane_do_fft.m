clc
clear

f = [1000 3000 5000 7000 9000 11000 13000 15000];
A = [100 100 100 100 100 100 100 100];

fs = 96000;
Ts = 1/fs;

N = 2048;

tk = N*Ts;
%tk = 10;
t = 0:Ts:tk-Ts;
x = 0;

for i=1:length(f)
    x = x + A(i)*sin(2*pi*f(i).*t);
end

% T = table(x');
% writetable(T, 'signal_mixed.txt','WriteVariableNames',0)
%audiowrite('mixed_4_12_16.wav',x,fs);

figure();
plot(t,x); grid on;
xlabel('Czas [s]'); ylabel('Amplituda [-]'); title('Sygnał audio');