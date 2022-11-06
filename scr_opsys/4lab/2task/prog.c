#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>


void sigint_handler(int sig){
    printf("Int signal - end work\n");
    exit(1);
}

void sigusr1_handler(int sig){
    printf("Usr1 signal - end work\n");
}

void sigusr2_handler(int sig){
    for(int i = 0; i <1000; i++){
        printf("usr2 signal i =1000 - end work\n");
    }
}

void sigterm_handler(int sig){
    printf("term signal - end work\n");
}

int main(){
    struct  timespec t;
    int i=1;
    
    t.tv_sec=0;
    t.tv_nsec=0;

    signal(SIGTERM,sigterm_handler);
    signal(SIGUSR1,sigusr1_handler);
    signal(SIGUSR2,sigusr2_handler);
    signal(SIGINT,sigint_handler);

    while(1){
        printf("%d %d\n", getpid(), i);
        i++;
        nanosleep(&t,0);
    }
    return 0;
}
