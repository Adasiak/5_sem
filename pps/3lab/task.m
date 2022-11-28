x=[2,1,3,5,7,3,6,4];
sr = mean(x);
std(x);

centr = x - sr;

%nbis = 3;
%eg = [1:2:7];
%histogram(x,eg);

%y=rand(1,10000000);
%eg = [1:3:100];
%ybis= 100;
%histogram(y,ybis);

q = [2,1,3,5];
w = [5,1,4];

xcorr(q,w);