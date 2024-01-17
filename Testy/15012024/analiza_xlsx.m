clc

%load('sinus_kodek_1k.mat');
%x = str2double(sinuskodek1k);
load('szum_bez_filtru.mat');
x = str2double(szumbezfiltru);

figure();
stairs(x); grid on;
xlabel('Nr próbki [-]'); ylabel('Amplituda [-]'); title('Zarejestrowany sygnał audio');
