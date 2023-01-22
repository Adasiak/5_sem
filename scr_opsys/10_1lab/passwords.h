#include <stdlib.h>
#include <stdbool.h>
typedef struct {
char hasz[1000][33];
char email[1000][33];
char **slownik;
char **tmp;
char tmp_email[1000][33];
char plik[20];
int k;
bool flaga;
int number;
int first_count_slownik;
int first_count_hasla;
bool check_producent0;
bool check_producent1;
bool check_producent2;
bool check_producent3;
bool check_producent4;
bool check_producent5;
pthread_mutex_t count_mutex;
} var;

extern var variables;