% -----------------------------------------------------------
%  HISTOGRAMY I AUTOKORELACJE R??NYCH SYGNA??????W
% -----------------------------------------------------------
clear all; close all;                    % czyszczenie pamieci programu
% -----------------------------------------------------------
%  histogram
% -----------------------------------------------------------
% do generowania np. sygna??w sinusoidalnych
N = 1000;
 fp = 20;
 t = 0:1/fp:(N-1)/fp;
 
 %-------------------------------------------------
 %plik audio
%[x,fp] = audioread('s3.wav'); % dla przyk?adowego pliku wav

%soundsc(x,fp); %odtwarzanie sygna?u 

%x=randn(10000,1); %generowanie sygna?u losowego
% -----------------------------------------------------------

% % -----------------------------------------------------------
%  histogram
% -----------------------------------------------------------
%x =sin(2*pi*2*t);%+ 0.7*randn(1,N);%sygna? sinusoidalny z szumem
%y=sin(2*pi*4*t);
% x=[1 2 3 1 2 3 4 5 7 2 1 9 5 5 2 1 9 1 4 3 20 21];%przykladowy sygna?
% y=[20 21 1 2 3 1 2 3 4 5 7 2 1 9 5 5 2 1 9 1 4 3];
% x=cntr(x);
% y=cntr(y);
% x=stnd(x);
% y=stnd(y);
%x=randn(1,N); %generowanie sygna?u losowego
[x N]=syg_gen(4);
%edges=[min(x):2:max(x)]; %generowanie przedzia??w dla histogramu
% figure(1)
%histogram(x,edges) %rysowanie histogramu dla wybranych przedzia??w
%nbins=100; %liczba przedzia??w
%histogram(x, nbins)% 
 %xlabel ('wartosc probki')
% ylabel ('l.prob. w przedziale')
% %-----------------------------

% % -----------------------------------------------------------
% %  autokorelacja x
% % -----------------------------------------------------------
 kmax =20; %d?ugo?? autokorelacji - ile pr?bek na siebie zachodzi
% %  %subplot (212)
%   figure(2)
%x=[5 2 1 5 7 2 5 3 2 5 1 3 5 9 2 5 7 4 5];
%x=randn(1,200000);
%y=[2 3 1 5];
figure(2)
  [rx, tau]=xcorr(x, x,kmax); %zwraca wektor przesuni?? i autokorelacji, mozna zamienic na korelacje krzyzowa
  plot(tau, rx); %rysuje autokorelacje
  % [rx, tau]=xcorr(x,x, kmax, 'coeff'); %unormowana autokorelacja
   %plot(tau, rx);
%[i j]=size(rx); %zwraca rozmiar wektora rx
% 
% %plot(tau, rx);
% rx=rx/(rx(ceil(j/2))); %wyznaczenie srodkowej wartosci wektora
% 
%plot(tau, rx);
 %tr = -kmax/fp:1/fp:kmax/fp; %przeliczenie przesuni?cia na czas
%  %subplot (212)
 %plot(tr, rx);
% %tr = -kmax:kmax; %zakres autokorelacji na osi x - os x w przesunieciach

%plot(tr,rx);
  %xlabel ('przesuniecie[s]')
 % ylabel ('autokorelacja')
%  wart_sred_x = mean (x)
% wariancja_x = var (x)