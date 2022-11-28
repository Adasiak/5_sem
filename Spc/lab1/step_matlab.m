clear all;
hold on;
grid on;

A = [-5,5,0.5,-0.5,-2];
B = [4,4,1,1,0];
L = ["Niestabilny bez oscylacji","Stabilny bez oscylacji","Stabilny z oscylacjami","Niestabilny z oscylacjami","Na granicy stabilności"];

for i = 1:max(size(A))
    a=A(i);
    b=B(i)

    sim("step.slx");
    figure(i);
    plot(ans.data);
    title(L(i));
end