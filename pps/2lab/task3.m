close all;
clear all;
grid on;

fp=20;
fi1=0;
N =100; %liczba probek sygna?u

t=0:1/fp:(N-1)/fp;
f0=1;
t1=5;
f1=5;
t2=2;

figure(1);
subplot(1,1,1);
y=chirp(t,f0,t1,f1);
%plot(t,y);

pspectrum(y,t,'spectrogram','TimeResolution',0.2, ...
    'OverlapPercent',99,'Leakage',0.85)

xlabel ('czas [s]');
ylabel ('sygnal');
title ('wykres chirp()');
