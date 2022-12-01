#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(){

    char **tab;
    int num = 11;
    int ID = 10;

    tab = malloc(num*sizeof(char*));
    for(int i = 0; i < num; i++){
        tab[i] = malloc((ID + 1)*sizeof(char));
    }
    tab[1] = "proba";


     printf("%s\n",tab[1]);



    free(tab);
    return 0;

}