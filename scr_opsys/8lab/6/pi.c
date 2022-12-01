#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <math.h>

#define THREADS 5
#define NUM 100000


pthread_mutex_t mutex;

typedef struct
 {
   int kolo;
   int kwadrawt;
   int r;
   long double pi;

 } PI;

PI war;


void *lotto(){

    double x;
    double y;
    double r = (double)war.r;
    int srodek = 0;
    int licznik = 0;
    long double pi;

    sleep(1);

    for(int i=0; i<NUM; i++){
        x = drand48();
        y = drand48();

        if((pow(x,2)+pow(y,2))<=pow(r,2)){
            srodek++;
        }
        licznik++;
    }

    pi = (long double)srodek/licznik;
    pi = pi*4;
    pthread_mutex_lock (&mutex);
    war.pi += pi;


    pthread_mutex_unlock (&mutex);
}




int main(){

    pthread_attr_t attr;
    long double wynik = 0;
    war.pi = 0;
    war.r = 1;

    pthread_t watek[THREADS];

    pthread_mutex_init(&mutex,NULL);
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for(int i=0; i<THREADS;i++){

        pthread_create(&watek[i], &attr, lotto,NULL);

        pthread_join(watek[i],NULL);
    }
    pthread_attr_destroy(&attr);

    wynik = war.pi/THREADS;

    printf(" Pi %Lf\n", wynik);


    pthread_mutex_destroy(&mutex);
    pthread_exit(NULL);

}