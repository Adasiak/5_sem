clear all;
close all;
%stabilne dla kp<8, dla kp=8 na granicy stabilnosci
t=100;
maxStep=0.05;
kp=5;
Ki=5;
ki=linspace(0.2*Ki, 0.7*Ki,5);
for i=1:1:max(size(ki))
    k=ki(i);
    sim("regPIsim.slx");
    figure(i);
    tiledlayout(2,1)
    nexttile;
    plot(ans.data);
    title(ki(i));
    nexttile; 
    %tutaj potrzeba wyswietlic punkt ktory wychodzi z wyliczenia calki
    %oznaczonej z danych w data1
    %plot();
    
    
    
end
