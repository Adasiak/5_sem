close all;
clear all;
grid on;

fp=200;
fp3=1000;

fi1=0;

N =1000; %liczba probek sygna?u

t=0:1/fp:(N-1)/fp;

x = rand(1,N);
x2 = randn(1,N);


y=x2;
plot (t, y);

xlabel ('czas [s]');
ylabel ('sygnal');
title ('wykres sinusoidy');
