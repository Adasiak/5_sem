%[x]=syg_gen(4);
%fp=1000;
clear all
close all
[x,fp] = audioread('s6.wav');
plot(x);
T=length(x);
Nf = T;
v = fft (x,Nf);             % widmo zespolone
w = abs (v);                % widmo amlitudowe
f = linspace (0, fp, Nf);   % generowanie osi czestotliwosci
sound(x,fp);
%x=x(1:50000);
% subplot (512);
 %stem (f,w)

 figure(1)
 %subplot(2,1,1)
 %[Wx,wx] = wgmx(x); % WGM

 %plot(wx/pi,Wx/max(Wx)); title('Input signal PSD'); grid; 
 %xlabel ('czestotliwosc[kHz]');

%subplot(2,1,1)
figure(2)
xl=length(x);
v=abs(fft(x,xl));
f = linspace (0, fp, xl);   % generowanie osi czestotliwosci
plot (f,v)
%periodogram(x,rectwin(length(x)),64000,fp)
% [Pxx,F] = periodogram(x,hamming(length(x)),length(x),fp);
% plot(F,10*log10(Pxx))
% subplot(2,1,2)
% [Pxx,F]=pwelch(x,rectwin(length(x)),50,64000,fp)
% plot(F,10*log10(Pxx))
% xlabel('Hz')
% ylabel('dB')
% title('Modified Periodogram Power Spectral Density Estimate')
figure(3)
  %subplot(2,1,2);
    spectrogram(x,kaiser(64),32,1024, fp);