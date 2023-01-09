#include <stdio.h>
#include <string.h>

// 
int main(){

    int c;
    int licznik = 0;
    FILE *file;
    file = fopen("hasla_2.txt","r");
    if(file){
        while((c = getc(file)) != EOF){
            putchar(c);
            if(c == '\n'){
                licznik++;
            }
        }
        fclose(file);
    }
    printf("Naliczylo %d, Koniec!\n",licznik);

    return 0;
}
