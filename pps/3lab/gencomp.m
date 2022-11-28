function [x] = gencomp(T,t1p,t1k,t2p,t2k,t3p,t3k,w1,w2,w3)
% Generation of a signal with three sine-waveform components
% ---------------------------------------------------
% T = number of samples of the generated signal x
% tp1, tk1 = offset and end-point of the 1-st component 
% tp2, tk2 = offset and end-point of the 2-nd component
% tp3, tk3 = offset and end-point of the 3-rd component
% w1 = frequency of the 1-st component
% w2 = frequency of the 2-nd component
% w3 = frequency of the 3-rd component
% x = vector of the generated signal samples
%
for t=1:t1p-1 x1(t)=0; end
for t=t1p:t1k x1(t)=sin(w1*pi*t); end
for t=t1k+1:T x1(t)=0; end
for t=1:t2p-1 x2(t)=0; end
for t=t2p:t2k x2(t)=sin(w2*pi*t); end
for t=t2k+1:T x2(t)=0; end
for t=1:t3p-1 x3(t)=0; end
for t=t3p:t3k x3(t)=sin(w3*pi*t); end
for t=t3k+1:T x3(t)=0; end
x=x1+x2+x3;
%zakomentowane na 1 zajecia
figure(2)
subplot(2,1,1); plot(x); title('Generated x(n) time-series'); grid;
X=wgmx(x);
X=X/max(X);
X=abs(fft(x));
subplot(2,1,2); plot(X(1:T/2-1)); title('PSD of x(n)'); grid;
pause;
