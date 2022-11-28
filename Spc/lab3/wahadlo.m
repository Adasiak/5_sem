clear all;
close all;

M = 0.5; % masa wozka
m = 0.2; % masa wahadla
L = 0.3; % dlugosc od mocowania do srodka ciezkosci wahadla
b = 0.1; % wspolczynnik tarcia wozka
g = 9.8; % przyspieszenie ziemskie

A = [0, g * (M + m) / (M * L), b / (M * L), 0;
    1, 0, 0, 0;
    0, -g * m / M, -b / M, 0;
    0, 0, 1, 0];
B = [-1/(M * L);
    0;
    1/M;
    0];
C = diag([1 1 1 1]);
D = [0;0;0;0];

% test obserwowalnosci
ob = obsv(A,C);
rank(ob)
% test sterowalnosci
ct = ctrb(A,B);
rank(ct)

% lqr
Q = diag([0 0 0 10]);
R = 10;
K = lqr(A,B,Q,R);
disp(['K = [',num2str(K(1)),',',num2str(K(2)),',',num2str(K(3)),',',num2str(K(4)),'];']);


%tout = 30;
x = [0;0;0;1];
sim("simulink_wahadlo.slx");
out = sim("simulink_wahadlo.slx");
figure(1);

for i=1:4
    subplot(1,2,1), hold on,grid on;
    plot(ans.Wyj(:,i));
end

subplot(1,2,1);
title("Q - I_4; R = 10; x= [0;0;0;1]");
xlabel("Czas[s]");
ylabel("x*(t)");
legend("Prędkość kątowa", "Kąt wychylenia", "Prędkość","Położenie");

figure(1);
subplot(1,2,2),hold on , grid on;
plot(ans.R, "r- ");
title("F(t)");
xlabel("Czas[s]");
ylabel("Siła[N]");
legend("Prędkość kątowa", "Kąt wychylenia", "Prędkość","Położenie");







