function [x, T]=syg_gen(a)
T=1000;
if a==1
    x=gencomp(T,1,T,1,T,1,T,0.1,0,0.0); % generation of a sine-waveform signal (consult the gencomp.m file)
end
if a==2
    x=gencomp(T,1,T,1,T,1,T,0.1,0.2,0.3); % generation of a sine-waveform signal (consult the gencomp.m file)
end
if a==3
x=gencomp(T,1,T,1,200,400,T,0.1,0.5,0.9); % generation of a sine-waveform signal (consult the gencomp.m file)
end
if a==4
    x = gen(T,0.1,0.15,0.85,0.9); % generation of a narrow-band random signal (consult the gen.m file)
end