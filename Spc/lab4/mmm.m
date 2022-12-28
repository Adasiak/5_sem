for i=1:ilosc_pom
    if i == 400
        theta+prawdziwa = theta_prawdziwa + [0.05;0;0;0]
    end

    Wyj(i) = Wyj(i)_następne(phi,theta_prawdziwa);
    Wyj_zak(i)=Wyj(i)normrnd(0,0.01);

    P=(P-(P*phi*phi`*P)/(lambda+phi`*phi))/lambda;
    theta_estymowane=theta_estymowane+P*phi`*(Wyj_zak(i)-phi`*theta_estymowane);
    Wyj_estymowane(i)=Wyj_nastepne(phi_estymowane,theta_estymowane);

    if i~=ilosc_pom
        phi = [Wyj_zak(i); phi(1);Wej(i+1);phi(3)];
        phi_estymowane = [Wyj_estymowane(i);phi_estymowane(1);Wej(i+1);phi_estymowane(3)];
    end
end

subplot(1,2,1)
plot(Wyj_zak-Wyj_estymowane)`
title("Róznica wyjscia prawdziwego i estymowanego")
xlabel("numer pomiadu") 
ylabel("roznica wyjscia prawdziwego i estymowanego")
legend("Wyj_{Prawdziwe}-Wyj_{esstymowane}")

subplot(1,2,2)
hold on
plot(Wyj_zak,"-k")
plot(Wyj_estymowane,"--C")
title("Wyjscie prawdziwe i estymowane")
xlabel("numer pomiadu") 
ylabel("wyjscia")
legend("Wyj_{Prawdziwe}","Wyj_{esstymowane}")
