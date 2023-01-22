#include  <openssl/evp.h>
#include  <string.h>
#define SIZE 14         //rozmiar tablicy slow slownika
#define HSIZE 11        //rozmiar tablicy hasel
#define LENGTH 33       //maksymalna ilosc liter wyrazu
#define NTHREDS 4       //liczba watkow

void bytes2md5(const char *data, int len, char *md5buf) {
        EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
        const EVP_MD *md = EVP_md5();
        unsigned char md_value[EVP_MAX_MD_SIZE];
        unsigned int md_len, i;
        EVP_DigestInit_ex(mdctx, md, NULL);
        EVP_DigestUpdate(mdctx, data, len);
        EVP_DigestFinal_ex(mdctx, md_value, &md_len);
        EVP_MD_CTX_free(mdctx);
        for (i = 0; i < md_len; i++) {
                snprintf(&(md5buf[i * 2]), 16 * 2, "%02x", md_value[i]);
        }
}

//implementacja bazy danych przechowujaca znalezione przez watki hasla
struct baza_hasel{
pthread_mutex_t m;
pthread_cond_t cond;
int iter;
char znalezione[SIZE][LENGTH], slownik[SIZE][LENGTH], hasla[SIZE][LENGTH], szyfr[HSIZE][LENGTH];
}baza;


void *cyfOdszy(void *_id ){

int* id = (int*) _id;
char kopia[LENGTH];
char cyfra[3];

for(int l = 0; l < 100; l++){
for(int j = 0; j < SIZE; j++){
strcpy(kopia, baza.slownik[j]);
sprintf(cyfra, "%d", l);
strcat(cyfra, kopia);
pthread_mutex_lock(&baza.m);
bytes2md5(cyfra, strlen(cyfra), baza.hasla[j]);
pthread_mutex_unlock(&baza.m);
        for(int k = 0; k < HSIZE; k++){
        //printf("%s ===> %s ?= %s\n", cyfra, hasla[j],  szyfr[k]);
                if(strcmp(baza.hasla[j], baza.szyfr[k]) == 0){
                //printf("[CyfraOdszyfrowanie] %s pasuje do %s, haslo: %s \n", baza.szyfr[k], baza.hasla[j], cyfra);
                pthread_mutex_lock(&baza.m);
                strcpy(baza.znalezione[baza.iter], cyfra);
                baza.iter ++;
                pthread_cond_signal(&baza.cond);
                pthread_mutex_unlock(&baza.m);
                }
        }
}
}
printf("%d\n", *id);
pthread_exit(NULL);
}
        void *odszyCyf(void *_id ){

int* id = (int*) _id;
char kopia[LENGTH];
char cyfra[3];

for(int l = 0; l < 100; l++){
for(int j = 0; j < SIZE; j++){
strcpy(kopia, baza.slownik[j]);
sprintf(cyfra, "%d", l);
strcat(kopia, cyfra);
pthread_mutex_lock(&baza.m);
bytes2md5(kopia, strlen(kopia), baza.hasla[j]);
pthread_mutex_unlock(&baza.m);
        //printf("%s pasuje do %s, haslo: %s \n", szyfr[k], hasla[j], cyfra);
        for(int k = 0; k < HSIZE; k++){
                if(strcmp(baza.hasla[j], baza.szyfr[k]) == 0 && strcmp(baza.hasla[j], baza.szyfr[k]) == 0){
                //printf("[OdszyfrowanieCyfra]%s pasuje do %s, haslo: %s \n", baza.szyfr[k], baza.hasla[j], kopia);
                pthread_mutex_lock(&baza.m);
                strcpy(baza.znalezione[baza.iter], kopia);
                baza.iter ++;
                pthread_cond_signal(&baza.cond);
                pthread_mutex_unlock(&baza.m);
                }
        }
}
}
printf("%d\n", *id);
pthread_exit(NULL);
}

        void *odszyfrowanie(void *_id ){

int* id = (int*) _id;

for(int j = 0; j < SIZE; j++){
pthread_mutex_lock(&baza.m);
bytes2md5(baza.slownik[j], strlen(baza.slownik[j]), baza.hasla[j]);
pthread_mutex_unlock(&baza.m);

        for(int k = 0; k < HSIZE; k++){
                if(strcmp(baza.hasla[j], baza.szyfr[k]) == 0){
                //printf("[Odszyfrowanie] %s pasuje do %s, haslo: %s \n", baza.szyfr[k], baza.hasla[j], baza.slownik[j]);
                pthread_mutex_lock(&baza.m);
                strcpy(baza.znalezione[baza.iter], baza.slownik[j]);
                baza.iter ++;
                pthread_cond_signal(&baza.cond);
                pthread_mutex_unlock(&baza.m);
                }
        }
        //printf("%s\n", slownik[j]);
}
printf("%d\n", *id);
pthread_exit(NULL);
}

        void *konsument(void* _id){

int* id = (int*) _id;

pthread_mutex_lock(&baza.m);
//if(baza.iter < 3){
while(baza.iter < HSIZE){
pthread_cond_wait(&baza.cond, &baza.m);
printf("[konsument] znalezione haslo: %s\n", baza.znalezione[baza.iter-1]);
}
pthread_mutex_unlock(&baza.m);

printf("%d\n", *id);
pthread_exit(NULL);
}

int main(int argc, char* argv[]){

FILE* plik;
// char *nazwa = argv[1];
char nazwa = "Hasla.txt";
// char *nazwaSlownik = argv[2];
char nazwaSlownik = "Slownik.txt";
char kosz[LENGTH];
int iter = 0;
int kontrola = 0;
pthread_t th[NTHREDS];
 pthread_attr_t attr;
int thid[NTHREDS];

//inicializacja baza
baza.iter = 0;
kontrola = pthread_attr_init(&attr);
if(kontrola){
perror("Blad inicializacji atrybutu watku");
exit(1);
}
pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

kontrola = pthread_cond_init(&baza.cond, NULL);
if(kontrola){
perror("Blad inicializacji zmiennej warunkowej");
pthread_attr_destroy(&attr);
exit(1);
}

kontrola = pthread_mutex_init(&baza.m, NULL);
if(kontrola){
perror("Blad inicializacji muteksu");
pthread_attr_destroy(&attr);
pthread_cond_destroy(&baza.cond);
exit(1);
}


//while(strcmp(nazwa, "k") != 0){ //petla pozwalajaca na wpisanie nowej listy hasel do odszyfrowania

plik = fopen(nazwaSlownik, "r");

if(plik == NULL){
perror("Blad wczytania pliku");
pthread_attr_destroy(&attr);
pthread_mutex_destroy(&baza.m);
pthread_cond_destroy(&baza.cond);
pthread_exit(NULL);
exit(1);
}

while(!feof(plik)){
fscanf(plik,"%s" , baza.slownik[iter]);
//printf("%s\n", baza.slownik[iter]);
iter++;
}

iter = 0;
fclose(plik);

plik = fopen(nazwa, "r");  //otworzenie pliku hasel

if(plik == NULL){
perror("Blad wczytania pliku");
pthread_attr_destroy(&attr);
pthread_mutex_destroy(&baza.m);
pthread_cond_destroy(&baza.cond);
pthread_exit(NULL);
exit(1);
}
while(!feof(plik)){
fscanf(plik,"%s", kosz);
fscanf(plik,"%s", baza.szyfr[iter]);
while(!feof(plik) && fgetc(plik) != '\n');

//printf("%s\n", baza.szyfr[iter]);
iter++;
}
fclose(plik);

for(int i=0; i< NTHREDS; i++){
thid[i] = i;
//pthread_create(&th[i], &attr, odszyfrowanie, &thid[i]);
}
pthread_create(&th[0], &attr, konsument, &thid[0]);
pthread_create(&th[1], &attr, odszyfrowanie, &thid[1]);
pthread_create(&th[2], &attr, odszyCyf, &thid[2]);
pthread_create(&th[3], &attr, cyfOdszy, &thid[3]);
//pthread_create(&th[1], &attr, odszyCyf, &thid[1]);


for (int i = 1; i < NTHREDS; i++) {
pthread_join(th[i], NULL);
}

for(int i =0; i<HSIZE; i++){
printf("znalezione hasla: %s\n", baza.znalezione[i]);
}

//printf("%s\n", nazwa);
//scanf("%s", nazwa);
//}

//printf("koniec \n");
//pthread_join(th[0], NULL);
pthread_cancel(th[0]);

pthread_attr_destroy(&attr);
pthread_mutex_destroy(&baza.m);
pthread_cond_destroy(&baza.cond);
pthread_exit(NULL);

return 1;
}