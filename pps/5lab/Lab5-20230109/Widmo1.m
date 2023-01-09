% -----------------------------------------------------------
%  WIDMO AMPLITUDOWE SYGNA?U W ZAKRESIE 0-fp
% -----------------------------------------------------------
clear all; close all;                    % czyszczenie pamieci programu
% -----------------------------------------------------------
%  parametry osi czasu, generowanie osi czasu
% -----------------------------------------------------------
N = 1024;
fp = 120;
t = 0:1/fp:(N-1)/fp;
% -----------------------------------------------------------
%  suma sinusow i szumu
% -----------------------------------------------------------
x = 2*sin(2*pi*41.8*t) + 10*sin(2*pi*42*t)+0.2*randn(1,N);
subplot (511);
plot (t,x)
xlabel ('czas[s]');
ylabel ('sygnal');
% -----------------------------------------------------------
%  widmo
% -----------------------------------------------------------
Nf = 1024;
v=fft(x, Nf) / Nf;
N21 = Nf/2 + 1;
f = linspace (0, fp/2, N21);
            % widmo zespolone
%subplot(512)
%plot(v);
w = abs (v);                % widmo amlitudowe
  
subplot (512);
stem (f,w(1:N21))
xlabel ('czestotliwosc[Hz]');
ylabel ('modul widma');
 
%-----------------------------------------------------------
%  okienkowanie
% %---
dok=1024;
 okno=hann(dok);
 okno=okno';
% 
subplot(513)
plot(okno);
s4=x(1:dok).*okno;
subplot(514)
plot(s4);
Nf = dok;
v=fft(s4, Nf) / Nf;
vs=abs(v);
N21 = Nf/2 + 1;
f = linspace (0, fp/2, N21);

subplot (515);
stem (f,vs(1:N21))
xlabel ('czestotliwosc[Hz]');
ylabel ('modul widma z oknem');
% % 
% % -----------------------------------------------------------

