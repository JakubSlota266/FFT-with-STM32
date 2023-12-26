clear; close all; clc

Fs = 96000;         % ustawiona czestotliwosc probkowania kodeka [Hz]
dt = 1/Fs;
t = 0:dt:10;

f = [500];
A = [1];

y = zeros(1,length(t));

for i=1:length(f)
    y = y + A(i)*sin(2*pi*f(i).*t);
end

%plot(t,y);
sound(y,Fs);
%audiowrite('test.wav',y,Fs);