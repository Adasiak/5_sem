clear all;
close all;
%stabilne dla kp<8, dla kp=8 na granicy stabilnosci
Kp=9;
kp=linspace(0.5*Kp,0.8*Kp,4);
for i=1:1:max(size(kp))
    k=kp(i);
    sim("stabil.slx");
    figure(i);
    tiledlayout(2,1)
    nexttile;
    plot(ans.data);title(kp(i));
    nexttile; 
    plot(ans.data1);
    
end
