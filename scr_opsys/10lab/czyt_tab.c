#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NAZWA "words_alpha.txt"


int dlug = 0;

int policz(){

    FILE *f;
    int l = 0;
    int chr;
    int m = 0;

    f = fopen(NAZWA,"r");
    if(f){
        while((chr = getc(f)) != EOF){
            m++;
            if(chr == '\n'){
               l++;
               if(m > dlug){
                   dlug = m;
               }
               m = 0;
            }
        }
        fclose(f);
    }
    printf("policz() - %d\n",l);

    return l;
}



int main(){

    int c;
    int licznik = 0;

    char **tab;
    int num = policz();
    int ID = num;
    int j = 0;

    FILE *file;


    tab = malloc(num*sizeof(char*));
    for(int i = 0; i < num; i++){
        tab[i] = malloc((ID + 1)*sizeof(char));
    }


    file = fopen(NAZWA,"r");
    if(file){
        while((c = getc(file)) != EOF){
            //putchar(c);
            tab[licznik][j] = c;
            j++;
            if(c == '\n'){
                licznik++;
                j = 0;
            }
        }
        fclose(file);
    }
    //printf("Naliczylo %d, Koniec!\n",licznik);
    //printf("dlugosc - %d, liczba wierszy - %d\n",dlug,num);

    printf("%s",tab[1]);

    free(tab);

    return 0;
}