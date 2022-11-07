#include <stdio.h>
#include <time.h>

int main(){
    int i=0;
    struct  timespec t;
    
    t.tv_sec=0;
    t.tv_nsec=0;
    // t.tv_nsec=100000;

    for (;;){
        i+=1;
        nanosleep(&t,0);
    }
    return 0;
}
