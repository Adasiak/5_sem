#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
// in order to measure the time
#include <time.h>


double get_actual_time_in_us()
{
    struct timespec now;
    clock_gettime(TIME_UTC, &now);
    return now.tv_sec*1e6 + now.tv_nsec*1e-3;
}

/*
The following structure contains the necessary information
to allow the function "dotprod" to access its input data and
place its output so that it can be accessed later.
*/

typedef struct
{
  double      *a;
  double      *b;
  double     sum;
  int    veclen;
} DOTDATA;

//#define VECLEN 1000000
#define VECLEN 1000000
#define NUMTHRDS 1                   // Stała określająca ilość wątków
  DOTDATA dotstr;
  pthread_t callThd[NUMTHRDS];       // Tablica wątków
  pthread_mutex_t mutexsum;

/*
We will use a function (dotprod) to perform the scalar product.
All input to this routine is obtained through a structure of
type DOTDATA and all output from this function is written into
this same structure.  While this is unnecessarily restrictive
for a sequential program, it will turn out to be useful when
we modify the program to compute in parallel.
*/

void *dotprod()
{

/* Define and use local variables for convenience */

   int start, end, i;                           // TUTAJ!
   long offset;             // Zmienna przechowujaca pewne odsadzenie miedzy watkami
   double mysum, *x, *y;

   start=0;
   end = dotstr.veclen;
   x = dotstr.a;
   y = dotstr.b;

/*
Perform the dot product and assign result
to the appropriate variable in the structure.
*/

   mysum = 0;
   for (i=start; i<end ; i++)
    {
      mysum += (x[i] * y[i]);
    }

   pthread_mutex_lock(&mutexsum);

   dotstr.sum += mysum;

   pthread_mutex_unlock(&mutexsum);

   pthread_exit((void*)0);

}

/*
The main program initializes data and calls the dotprd() function.
Finally, it prints the result.
*/

int main (int argc, char *argv[])
{
int i,len;
double *a, *b;
void *status;
pthread_attr_t attr;

/* Assign storage and initialize values */
len = VECLEN;
a = (double*) malloc (len*sizeof(double));
b = (double*) malloc (len*sizeof(double));

for (i=0; i<len; i++) {
  a[i]=1;
  b[i]=a[i];
  }

dotstr.veclen = len;
dotstr.a = a;
dotstr.b = b;
dotstr.sum=0;

pthread_mutex_init(&mutexsum, NULL);
pthread_attr_init(&attr);
pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);


double time_start = get_actual_time_in_us();
/* Perform the  dotproduct */

for(i=0;i<NUMTHRDS;i++){
    pthread_create(&callThd[i],&attr,dotprod,NULL);

}

//dotprod ();
double time_stop = get_actual_time_in_us();


pthread_attr_destroy(&attr);

for(i=0;i<NUMTHRDS;i++){
    pthread_join(callThd[i],&status);
}

/* Print result and release storage */
printf ("Sum =  %f \n", dotstr.sum);
printf ("It took %.3lf microseconds to run.\n", time_stop-time_start );
free (a);
free (b);
pthread_mutex_destroy(&mutexsum);
pthread_exit(NULL);
}