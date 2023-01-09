
clear all; close all;
% ----------------  generowanie sinusa
fpx = 2000;
Nx = 2000;
tx = 0:1/fpx:(Nx-1)/fpx;
f0 = 70; f1=200;
x = sin (2*pi*f0*tx);%+0.2*randn(1,Nx);% sin(2*pi*f1*tx);% ;%%0.2*randn(1,Nx);
subplot (221);
plot (tx,x);
xlabel ('czas[s]');
ylabel ('sygnal przed decymacja');

Nf = 2^11;
N21 = Nf/2 + 1;
wx = abs(fft(x,Nf));
fx = linspace(0, fpx/2, N21);
subplot (222);
plot (fx, wx(1:N21));
xlabel ('czest[Hz]');
ylabel ('modul widma');

% ------------------------------------------------------------------
%      decymacja w petli
%      gdyz funkcja decimate przeprowadza filtracje antyaliasingow???
% ------------------------------------------------------------------
dr = 13;
%  Ny = floor (Nx / dr); 
%   fpy = fpx / dr;
%   for i=1:Ny
%     y(i) = x(    (i-1)*dr+1     ); 
%   end  
y=decimate(x,dr);%resamples the sequence in vector X at 1/dr times the
    %original sample rate
fpy=fpx/dr;
Ny=length(y);
ty = 0:1/fpy:(Ny-1)/fpy;
subplot (223);
plot (ty,y);
xlabel ('czas[s]');
ylabel ('sygnal po decymacji');

wy = abs(fft(y,Nf));
fy = linspace(0, fpy/2, N21);
subplot (224);
plot (fy, wy(1:N21));
xlabel ('czest[Hz]');
ylabel ('modul widma');

set (gcf,'Position',[50 50 800 700]);
%print -depsc ProbkAlias1