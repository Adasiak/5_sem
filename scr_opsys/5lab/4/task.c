#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(){
    char *hai;
    // hai="USER";
    hai="LOGNAME";
    printf("%s %s\n","Hello ",getenv(hai));
    return 0;
}