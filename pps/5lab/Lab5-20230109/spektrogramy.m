clear all; close all;

N=2500;
fp=5000;
t=0:1/fp:(N-1)/fp;

x=chirp(t,100,(N-1)/fp,500);
%x=sin(2*pi*3*t)+sin(2*pi*6*t);
figure(1)
subplot(211)
plot(t,x);
subplot(212)
Nf=2048;
N21=Nf/2+1;
f=linspace(0,fp/2,N21);
widmo=abs(fft(x,Nf));
plot(f,widmo(1:N21));

figure(2)

subplot(311)
M=256;
M21=M/2+1;
f2=linspace(0,fp/2,M21);

widmo=abs(fft(x(1:200)));
plot(f2,widmo(1:M21));
subplot(312)
widmo=abs(fft(x(300:500)));
plot(f2,widmo(1:M21));
subplot(313)
widmo=abs(fft(x(700:900)));
plot(f2,widmo(1:M21));

figure(3)
spectrogram (x, 256, 128 , 512, fp);

