clc
clear

f = [20, 50, 100, 111, 253, 700];
A = [0.5, 1, 0.25, 3, 1.5, 2];

fs = 1000;
Ts = 1/fs;
%% ZAWSZE SPRAWDZAĆ PRZED WŁĄCZENIEM!
N = 10240;
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
