clear;
close all;

%W razie gdyby matlab wyrzucal warningi apropo interpretera latexa
%wpisac w konsole 'warning on verbose', aby przy kolejnych warningach
%matlab poinformowal jak je wylaczyc

%Wszystkie linie i strzalki na wykresy mozna nanosic prosto w oknie
%wykresu. Nie potrzeba zadnych narzedzi firm trzecich. W oknie wykresu
%na pasku narzedzi wybrac opcje insert.

s = tf('s');


% ODPOWIEDZI NA SKOK - ROZNE ROZMIESZCZENIE BIEGUNOW
B_Array = [10, -1, 1, -1];
C_Array = [3, -7, 10, 5];

for i = 1:4
    
    B = B_Array(i);
    C = C_Array(i);
    sys = (1/(s^2+B*s+C));
    sys_txt = sprintf('$\\frac{1}{s^2 %+.ds %+.d}$', B, C);
    
    figure(i);
    step(sys);
    grid on;
    title(['K(s) = ', sys_txt], 'FontSize', 25, 'interpreter', 'latex');
%     hold on;
%     a = 0.09887;
%     b = 0.4936;
%     c = 0.09994;
%     t=0:0.01:12;
%     expp = a*exp(-b*t)+c;
%     plot(t, expp,'Color',[0,0,0]);
%
% DO SPRAWKA:
% Powyzszy zakomentowany kod sluzy do naniesienia funkcji identyfikujacej
% transmitancje z wykresu numer 3 (stabilne oscylacje). Wartosci
% wspolczynników wyznaczone sa dzieki narzedziu curve fitting matlaba. Aby
% odkomentowany kod dzialal, trzeba zmienic rowniez indeks petli na 'i=3'.
end


% ODPOWIEDZI NA SKOK - CZLONY RZECZYWISTE
sys_0 = (4/s);
sys1 = (s/((5*s+1)*s)); %rozniczkujacy z inercja (rzeczywisty)
sys2 = (1/((12*s^2+s)*s)); %calkujacy z inercja (rzeczywisty)
sys_0_txt = "K(s) = 4";
sys1_txt = 'K(s) = $\frac{s}{5s+1}$';
sys2_txt = 'K(s) = $\frac{1}{12s^2+s}$';

sys_Array = [sys_0,sys1, sys2];
sys_txt_Array = {sys_0_txt,sys1_txt, sys2_txt};

for i = 1:3
    
    figure(i+4);
    impulse(sys_Array(i), 200);
    grid on;
    title(sys_txt_Array(i), 'FontSize', 25, 'interpreter', 'latex');
end


