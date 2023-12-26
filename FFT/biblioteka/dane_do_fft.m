clc
clear

f = [30, 200, 1000, 3000, 5000, 18000];
A = [0.5, 1, 1.5, 2, 0.25, 0.5];

fs = 60000;
Ts = 1/fs;
%% ZAWSZE SPRAWDZAĆ PRZED WŁĄCZENIEM!
N = 16384;
%%

tk = N*Ts;
t = 0:Ts:tk-Ts;
x = 0;

for i=1:length(f)
    x = x + A(i)*sin(2*pi*f(i).*t);
end

T = table(x');
writetable(T, 'matrix.txt','WriteVariableNames',0)

plot(t,x);
