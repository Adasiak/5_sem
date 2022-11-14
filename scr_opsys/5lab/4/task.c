#include <stdio.h>
#include <time.h>
#include <stdlib.h>
// #include <sys.h>

int main(){
    // struct  timespec t;
    // int i=1;
    
    // t.tv_sec=0;
    // t.tv_nsec=0;

    // printf("%s %d\n","Hello ",getenv());

    char *hai;
    printf("The current User name is\n");
    hai="USER";
    printf("%s %s\n","Hello ",getenv(hai));
    
    
    // sleep(10);
    return 0;
}