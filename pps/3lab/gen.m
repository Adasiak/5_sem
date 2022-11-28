function [x] = gen(T,d1,g1,d2,g2)
% Generation of a random narrow-band signal 
% -------------------------------------------------
% T = number of the generated signal samples 
% d1, g1 = frequencies of the lower transition band (normalized frequencies: 0.0 - 1.0)
% d2, g2 = frequencies of the upper transition band (normalized frequencies: 0.0 - 1.0)
% x = vector of the generated signal samples
%
d1pi=d1*pi; g1pi=g1*pi;
d2pi=d2*pi; g2pi=g2*pi;
pasmo=min((g1pi-d1pi),(g2pi-d2pi));
dlfil=ceil(11*pi/pasmo)+1;
n=[0:1:dlfil-1];
czdol=(d1pi+g1pi)/2;
czgor=(d2pi+g2pi)/2;
u=n-(dlfil-1)/2+eps;
odpdol=sin(czdol*u)./(pi*u);
odpgor=sin(czgor*u)./(pi*u);
odp=odpgor-odpdol;
okno=(hanning(dlfil))'; % you can use any other window function!
h=odp.*okno;
noise=randn(1,T); % generation of 'white noise'
y=conv(noise,h);
for t=1:T x(t)=y(t+round(dlfil/2)); end
% [Wn,wn] = wgmx(noise); % computation of the 'white noise' PSD
% [H,wH] = ampl(h); % computation of the FIR filter amplitude characteristic
% [Wx,wx] = wgmx(x); % computation of the output PSD
% subplot(3,2,1); plot(noise); grid; title('Signal randn');
% pause;
% subplot(3,2,3); plot(h); grid; title('Filter impulse response');
% pause;
% subplot(3,2,5); plot(x); grid; title('Signal x(n)');
% pause;
% subplot(3,2,2); plot(wn/pi,Wn); grid; title('PSD of the signal randn');
% pause;
% subplot(3,2,4); plot(wH/pi,H); grid; title('Filter magnitude response');
% pause;
% subplot(3,2,6); plot(wx/pi,Wx); grid; title('PSD of the signal x(n)');
% pause;
