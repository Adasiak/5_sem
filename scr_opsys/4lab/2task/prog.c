#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define DELAY 5e08

void sigint_handler(int sig){
    printf("\nInt signal - end work\n");
    exit(1);
}

void sigusr1_handler(int sig){
    printf("\nUsr1 signal\n");
}

void sigusr2_handler1(int sig){
    printf("\nUsr2 signal\n");
    for (int i=0;i<1000;i++){
        continue;
    }
}

void sigusr2_handler2(int sig){
    printf("\nUsr2 signal - end works\n");
    exit(1);
}

void sigterm_handler(int sig){
    printf("\nTerm signal\n");
    exit(1);
}


int main(){
    struct  timespec t;
    int i=1;
    
    t.tv_sec=0;
    t.tv_nsec=0;

    signal(SIGTERM,sigterm_handler);
    signal(SIGUSR1,sigusr1_handler);
    // signal(SIGUSR2,sigusr2_handler1);
    signal(SIGINT,sigint_handler);
    signal(SIGALRM,SIG_IGN);

    printf("%d\n", getpid());
    alarm(3);

    while(1){
        i++;
        if(i > 1000){
            i=0;
        }
        else if (i == 1000)
        {
            signal(SIGUSR2,sigusr2_handler2);
        }
        else{
            signal(SIGUSR2,sigusr2_handler1);
        }
        nanosleep(&t,0);
    }
    return 0;
}
