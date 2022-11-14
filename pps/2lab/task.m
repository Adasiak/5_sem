clear all;
fp=200;
fp3=1000;

f1=10;
f2=20;
f3=200;

fi1=0;

A1=2;
A2=6;
A3=4;
N =1000; %liczba probek sygna?u
t=0:1/fp:(N-1)/fp;
t2=0:1/fp:(N-1)/fp;
t3=0:1/fp3:(N-1)/fp3;

%t3=1:3;
%x = A1*sin(2*pi*f1*t+fi1);

x = A1*sin(2*pi*f1*t);
x2 = A2*sin(2*pi*f2*t2);
x3 = A3*sin(2*pi*f3*t3);

x3(1:200)=0;
x3(801:1000)=0;
k=3;
%x=k*x;
figure(1);
subplot(4,1,1);
y1=x+x2+x3;
plot (t, y1);
%stem(t,x);
xlabel ('czas [s]');
ylabel ('sygnal');
title ('wykres sinusoidy');

subplot(4,1,2);
r1=rand(1,N);
y2=r1;
plot (t, y2);
xlabel ('czas [s]');
ylabel ('sygnal');
title ('rozklad gausa');

subplot(4,1,3);
r2=randn(1,N);
y3=r2;
plot (t, y3);
xlabel ('czas [s]');
ylabel ('sygnal');
title ('rozklad gausa');

subplot(4,1,4);
y4=y1+r1;
plot (t, y4);
xlabel ('czas [s]');
ylabel ('sygnal');
title ('rozklad gausa');
