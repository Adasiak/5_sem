#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define WATKI 5                   //Zawiera liczbe watkow

void *Print_hello(){              // Funkcja wyświetla napis
                                               // Zwraca wartość ID wątku
    printf("Hello SCR. Written by thread %d\n",pthread_self());

    pthread_exit(NULL);              // Funkcja zakańcza wątek
}

int main(int arc, char *argv[]){

    pthread_t id[WATKI];            // Identyfikator wątku
    int error_code;                 // Zmienna sprawdzająca błąd

    for(int i = 0; i < WATKI; i++){
        error_code = pthread_create(&id[i], NULL, Print_hello, NULL);
    }

    if(error_code){                 // Sprawdzana jest poprawność stworzenia wątku
                                    // 1 - wystąpił błąd
        fprintf(stderr,"Thread nie zostal poprawnie stworzony!\n");
    }

    for(int i = 0; i < WATKI; ++i){
        // Dany wątek zostaje wstrzymany do momentu gdy wątek zakończy działąnie
        pthread_join(id[i],NULL);

    }
    pthread_exit(NULL);             // Zakończenie wątku
}