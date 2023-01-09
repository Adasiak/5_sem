% skrypt wyznacza widmo amplitudowe i fazowe
% sygnalu bedacego suma dwoch sinusoid
% clear all; 
close all;
f1=10;	% czestotliwosc pierwszej sinosoidy
A1=10;		% amplituda pierwszej sinusoidy
phi1=0;	% faza pierwszej sinusoidy

f2=20;	% czestotliwosc drugiej sinosoidy
A2=1;		% amplituda drugiej sinusoidy
phi2=0;		% faza drugiej sinusoidy

N=200;		% dlugosc sygnalu
fp=20;		% czestotliwosc probkowania
Nf=100;     % d?ugo?? transformacji Fouriera
% -------------------------------------------------------------------
%       W tych warunkach odleg?o?? pomi?dzy pr??kami widma wynosi 1Hz
% -------------------------------------------------------------------

% generuj os czasu
t=0:1/fp:(N-1)/fp;

% generuj sygnal
x=A1*sin(2*pi*f1*t+phi1)+A2*sin(2*pi*f2*t+phi2);
% x=A1*sin(2*pi*f1*t+phi1);

subplot (311);
plot (t,x);
xlabel ('czas[s]');
ylabel ('sygnal');

% wyznacz widmo
widmo=fft(x, Nf) / Nf;
%widmo=fft(x, Nf);% / Nf;
N21 = Nf/2 + 1; %probka odpowiadajaca czestotliwosc Nyquista
f = linspace (0, fp/2, N21); %rownomierny podzial osi x na wartosci od 0 do 1/2fp (generuje N21 punktow)

% wykres czesci rzeczywistej i urojonej
xr = real (widmo); %czesc rzeczywista
subplot (312); 
hold off;
stem (f, xr(1:N21), 'g'); %rysujemy tylko polowe widma
hold on;
xi = imag (widmo); %czesc urojona
stem (f, xi(1:N21), 'r'); %rysujemy tylko polowe widma 
hold off;
xlabel ('czestotl.[Hz]');
ylabel ('re(X) & im(X)');

% wykres modulu i fazy widma
widmo_amp=abs(widmo); %modul
widmo_faz=angle(widmo); %faza
subplot(615);
stem (f, widmo_amp(1:N21), 'k');
ylabel ('|X|');
subplot(616);
stem (f, widmo_faz(1:N21), 'b');
xlabel ('czestotl.[Hz]');
ylabel ('arg(X)');

set (gcf,'Position',[50 50 800 700]);