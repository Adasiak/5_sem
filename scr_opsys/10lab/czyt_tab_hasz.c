//  Program do łamania haseł
//
//  W programie został zaimpementowany:
//  - Producent_0 - słowa z małymi literami haszuje i porównuje z hasłem
//  - Producent_1 - słowa z pierwszą dużą literą haszuje i porównuje z hasłem
//  - Producent_2 - słowa z dużymi literami haszuje i porównuje z hasłem
//  - Konsument - Wyświetla znalezione hasła
//
//      Wszyscy producenci są w stanie dodawać na początek i koniec słowa kilkucyfrowe liczby.
//      Słowa z słownika jak i hasła są zapisywane w dynamicznej tablicy.
//      Wysłanie sygnału powoduje wyświetlenie się liczby znalezionych haseł.
//      Zadanie zostało wykonane w podstawowej wersji bez dodatkowych wymagań.
//
//
// 

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <openssl/evp.h>
#include <signal.h>
#include <pthread.h>

#define SLOWNIK "test.txt"
#define HASLA   "hasla_2.txt"
//#define HASLA   "hasla12.txt"

#define SIZE 33



typedef struct
{
    char **tablica;         // Przechowuje słowa z słownika
    char **wynik;           // Przechowuje oodhaszowane hasła w takiej samej kolejności jak podane hasła
    char **haslo;           // Przechowuje zahaszowane hasła
    char **extra;           // Przechowuje odhaszowane hasła w kolejności ich odszyfrowania
    int licz_sl;            // Zmienna przechowuje ilość słów w słowniku
    int licz_has;           // Zmienna przechwuje ilość haseł do odhaszowania
    int znalazl;            // Zmienna przechowuje ilość odhaszowanych haseł podczas pracy programu

} STRUKT;

STRUKT dane;

pthread_mutex_t count_mutex;

// Funkcja policz() zapisuje do tablicy dynamicznej wszystkie słowa z słownika oraz zahaszowane hasła

void policz(){

    FILE *f1;
    FILE *f2;
    int l_1 = 0;        //Zmienne pomocnicze
    int l_2 = 0;
    int c_1;
    int c_2;
    int m_1 = 0;
    int m_2 = 0;
    int d_1 = 0;
    int d_2 = 0;

    f1 = fopen(SLOWNIK,"r");                // Otwiera plik z słownikiem dla odczytu
    if(f1){
        while((c_1 = getc(f1)) != EOF){     //Zlicza ilość słów w słowniku
            m_1++;
            if(c_1 == '\n'){
               l_1++;
               if(d_1 < m_1){               //Zlicza maksymalną długość słowa
                    d_1 = m_1;
                }
                m_1 = 0;
            }
        }
        fclose(f1);                         // Dynamicznie zaalokowuje pamięć dla słownika
        dane.tablica = malloc(l_1*sizeof(char*));
        for(int i = 0; i < l_1; i++){
            dane.tablica[i] = malloc(l_1*sizeof(char));
        }
    }
    dane.licz_sl = l_1;                     // Zapisuje w strukturze ilość słów
    l_1 = 0;
    m_1 = 0;

    f1 = fopen(SLOWNIK,"r");
    if(f1){
        while((c_1 = getc(f1)) != EOF){

            if(c_1 == '\n'){
                l_1++;
                m_1 = 0;
            }else{
                dane.tablica[l_1][m_1] = c_1;  // Zapisuje do dynamicznej tablicy słowa z słownika
                m_1++;
            }
        }

        fclose(f1);
    }

    f2 = fopen(HASLA,"r");                      // Otwiera plik z hasłami do odczytu
    if(f2){
        while((c_2 = getc(f2)) != EOF){      // Odczytuje z pliku aż nie dojdzie do końca pliku
            m_2++;
            if(c_2 == '\n'){
               l_2++;                       // Zliczana jest ilość haseł do rozkodowania
               if(d_2 < m_2){
                    d_2 = m_2;              // Zliczana jest maksymalna długość haseł
                }
                m_2 = 0;
            }
        }                                   // Dynamicznie zaalokowuje pamięć
        dane.haslo = malloc(l_2*sizeof(char*));
        for(int i = 0; i < l_2; i++){
            dane.haslo[i] = malloc((d_2+1)*sizeof(char));
        }
    }
    dane.licz_has = l_2;                    // Zapisuje w strukturze ilość haseł
    l_2 = 0;
    m_2 = 0;
    fclose(f2);


    f2 = fopen(HASLA,"r");
    if(f2){
        while((c_2 = getc(f2)) != EOF){   // Czyta z pliku aż nie dojdze do jego końca
            dane.haslo[l_2][m_2] = c_2;    // Zapisuje do dynamicznej tablicy hasła

            m_2++;
            if(c_2 == '\n'){

                l_2++;
                m_2 = 0;
            }
        }

        fclose(f2);
    }                            // Dynamicznie zaalokowuje pamięć dla tablicy odhaszowanych haseł
    dane.wynik = malloc(l_1*sizeof(char*)); // Zapisywanych zgodnie z kolejnością zahaszowanych haseł
    for(int i = 0; i < l_1; i++){
        dane.wynik[i] = malloc(l_1*sizeof(char));
    }                           // Dynamicznie zaalokowuje pamięć dla tablicy odhaszowanych haseł
    dane.extra = malloc(l_1*sizeof(char*));     // Zapisywanych zgodnie z kolejnością znalezienia
    for(int i = 0; i < l_1; i++){
        dane.extra[i] = malloc(l_1*sizeof(char));
    }

}

// Funkcja byte2md5() pobiera dane słowo i zwraca zahaszowane hasło

void bytes2md5(const char *data, int len, char *md5buf) {
  // Based on https://www.openssl.org/docs/manmaster/man3/EVP_DigestUpdate.html
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


// Funkcja sprawdz() porównuje zahaszowane hasło z zahaszowanym słowem z słownika

int sprawdz(char *md5){

    int licznik = 0;        // Zmienne pomocnicze
    int heh = 0;

    for(int j=0;j < dane.licz_has; j++){

            licznik = 0;

            for(int k=0;k<SIZE;k++){
                if(md5[k]==dane.haslo[j][k]){           //Porównuje każdą literę hasła z zahaszowanym słowem

                    licznik++;
                }
                if(licznik == 32){
                    licznik = 0;
                    heh = j + 1;
                }
            }
        }
    return heh;                                     // Jeżeli hasło i słowo są różne zwraca 0
}                                                   // Jeżeli hasło i słowo są takie same zwraca
                                                    // liczbę wiersza pod którą zapisane jest dane hasło


// Funkcja *prod_1() - sprawdza wszystkie słowa z słownika dla małych liter
void *prod_1(){

    int check = 0;      // Zmienne pomocnicze
    int a = -1;
    int licz = 0;

    char buf[dane.licz_sl];     // bufory
    char md5[33]; // 32 characters + null terminator
    char result[dane.licz_sl];
    char *num;

    //dane.licz_has != dane.znalazl
    while(dane.licz_has-1 != dane.znalazl){         // Pętla działa tak długo aż zostaną znalezione wszystkie hasła
        for(int i=0; i<dane.licz_sl;i++){           // pobierne jest po koleji każde słowo z słownika

             if(a>=0){                              // Jeżeli wszystkie słowa zostały już sprawdzone zaczyna dodawać liczby
                strcpy(buf,dane.tablica[i]);        // Zapisuje słowo z słownika do bufora
                asprintf(&num,"%d",a);              // Zamienia int na char
                snprintf(result, sizeof result,"%d%s",a, buf); //Przypisuje liczbę na samym początku danego słowa
                licz = strlen(result)-1;            // Oblicza długość słowa
                strcat(result,num);                 // Zwiększa długość bufora

                for(int i=0; i< strlen(num) ;i++){
                   result[licz+i] = num[i];         // Wpisywane są liczby na koniec słowa
                }
                result[licz+strlen(num)]  = '\0';   // Dodaje znak końca słowa
             }else{
                 strcpy(result,dane.tablica[i]);    // Przypisuje do bufora słowa bez liczb
             }
             bytes2md5(result, strlen(result), md5);    //Haszuje bufor

             check = sprawdz(md5);                  // Porównuje zahaszowane słowo z hasłem
             if(check > 0){                         // Jeżeli hasło się zgadza
                 pthread_mutex_lock(&count_mutex);  // Mutex unieruchamia inne wątki na czas dostępu do zmiennej globalnej
                 strcpy(dane.wynik[check-1],result); // Zapisuje hasło w pierwszej tablicy
                 strcpy(dane.extra[dane.znalazl],result);      // Zapisuje hasło w drugiej tablicy
                 dane.znalazl = dane.znalazl + 1;              // Zwiększa zakres
                 pthread_mutex_unlock(&count_mutex);    // Odblokowuje Mutex

             }
        }
        a++;

    }
     pthread_exit(NULL);
}

void *prod_2(){

    int check = 0;          // Zmienne pomocnicze
    int a = -1;
    int licz = 0;
    char *num;
    char buf[dane.licz_sl];     // bufory
    char md5[33]; // 32 characters + null terminator
    char result[dane.licz_sl];

    while(dane.licz_has-1 != dane.znalazl){ // Pętla działa tak długo aż zostaną znalezione wszystkie hasła
        for(int i=0; i<dane.licz_sl;i++){   // pobierne jest po koleji każde słowo z słownika

            if(a>=0){                       // Jeżeli wszystkie słowa zostały już sprawdzone zaczyna dodawać liczby
                strcpy(buf,dane.tablica[i]);// Zapisuje słowo z słownika do bufora
                *buf = toupper(*buf);       // Pogrubia pierwszą literę
                asprintf(&num,"%d",a);      // Zamienia int na char
                snprintf(result, sizeof result,"%d%s",a, buf);  //Przypisuje liczbę na samym początku danego słowa
                licz = strlen(result)-1;    // Oblicza długość słowa
                strcat(result,num);         // Zwiększa długość bufora
                for(int i=0; i< strlen(num) ;i++){
                   result[licz+i] = num[i];      // Wpisywane są liczby na koniec słowa
                }
                result[licz+strlen(num)]  = '\0';   // Dodaje znak końca słowa
            }else{
                 strcpy(result,dane.tablica[i]);    // Przypisuje do bufora słowa bez liczb
                 *result = toupper(*result);        // Pogrubia pierszą litere
            }

            bytes2md5(result, strlen(result), md5); //Haszuje bufor
            check = sprawdz(md5);                   // Porównuje zahaszowane słowo z hasłem
            if(check > 0){                          // Jeżeli hasło się zgadza
                pthread_mutex_lock(&count_mutex);   // Mutex unieruchamia inne wątki na czas dostępu do zmiennej globalnej
                strcpy(dane.wynik[check-1],result); // Zapisuje hasło w pierwszej tablicy
                strcpy(dane.extra[dane.znalazl],result);    // Zapisuje hasło w drugiej tablicy
                dane.znalazl= dane.znalazl + 1;     // Zwiększa zakres
                pthread_mutex_unlock(&count_mutex); // Odblokowuje Mutex
            }
        }
        a++;
    }
}

void *prod_3(){

    int check = 0;      // Zmienne pomocnicze
    int a = -1;
    int b = 0;
    int licz = 0;
    char *num;
    char buf[dane.licz_sl];     //Bufory
    char md5[33]; // 32 characters + null terminator
    char result[dane.licz_sl];

    while(dane.licz_has-1 != dane.znalazl){     // Pętla działa tak długo aż zostaną znalezione wszystkie hasła
        for(int i=0; i<dane.licz_sl;i++){       // pobierne jest po koleji każde słowo z słownika
            if(a>=0){                           // Jeżeli wszystkie słowa zostały już sprawdzone zaczyna dodawać liczby
                strcpy(buf,dane.tablica[i]);    // Zapisuje słowo z słownika do bufora
                for(int l=0; l<(strlen(buf)-1);l++){    // Pogrubia wszystkie litery
                    buf[l] = toupper(buf[l]);
                }

                asprintf(&num,"%d",a);          // Zamienia int na char
                snprintf(result, sizeof result,"%d%s",a, buf);  //Przypisuje liczbę na samym początku danego słowa
                licz = strlen(result)-1;        // Oblicza długość słowa
                strcat(result,num);             // Zwiększa długość bufora
                for(int i=0; i< strlen(num) ;i++){
                   result[licz+i] = num[i];      // Wpisywane są liczby na koniec słowa
                }
                result[licz+strlen(num)]  = '\0';   // Dodaje znak końca słowa
            }else{
                strcpy(result,dane.tablica[i]); // Przypisuje do bufora słowa bez liczb
                for(int j=0; j<(strlen(dane.tablica[i])-1);j++){
                    result[j] = toupper(result[j]); // Pogrubia wszystkie litery
                }
            }
            bytes2md5(result, strlen(result), md5); //Haszuje bufor
            check = sprawdz(md5);                   // Porównuje zahaszowane słowo z hasłem
            if(check > 0){                          // Jeżeli hasło się zgadza
                pthread_mutex_lock(&count_mutex);   // Mutex unieruchamia inne wątki na czas dostępu do zmiennej globalnej
                strcpy(dane.wynik[check-1],result); // Zapisuje hasło w pierwszej tablicy
                strcpy(dane.extra[dane.znalazl],result);    // Zapisuje hasło w drugiej tablicy
                dane.znalazl= dane.znalazl + 1;     // Zwiększa zakres
                pthread_mutex_unlock(&count_mutex); // Odblokowuje Mutex

            }

         }
        a++;

    }
     pthread_exit(NULL);
}

// Funkcja wysw() wyświetla komunikat o liczbie znalezionych haseł
void wysw(){
    printf("Znaleziono %d hasel z %d\n",dane.znalazl,dane.licz_has-1);
}


void *kons(){

    int a = 1;

    signal(SIGHUP,wysw);    //Wyświetla komunikat po otrzymaniu sygnału

    while(dane.licz_has != a){  // Pętla działa tak długo aż nie zostaną wyświetlone wszystkie hasła

        if(dane.znalazl == a){  // Jeżeli zostanie znalezione nowe hasło
            pthread_mutex_lock(&count_mutex);   // Blokuje Mutex
            printf("Znalezione haslo %d. %s\n",dane.znalazl ,dane.extra[(dane.znalazl-1)]); // Wyświetla hasło
            a++;
            pthread_mutex_unlock(&count_mutex); // Odblokowyje Mutex
        }
    }
     pthread_exit(NULL);
}

int main(){

    pthread_t threads[4];
    pthread_attr_t attr;

    policz();       // Zapisuje słowa i hasłą do tablicy dynamicznej oraz zlicza ilość słów i haseł

    pthread_attr_init(&attr);   // Inicjalizuje wątki
    pthread_mutex_init(&count_mutex, NULL); // Inicjalizuje Mutex

    printf("Rozpoczyna wyszukiwanie hasel:\n");

    pthread_create(&threads[0], &attr, prod_1, NULL);       // Tworzy pierwszy wątek - haszuje słowa z małymi literami
    pthread_create(&threads[1], &attr, prod_2, NULL);       // Tworzy drugi wątek - haszuje słowa z pierwszą dużą literą
    pthread_create(&threads[2], &attr, prod_3, NULL);       // Tworzy trzeci wątek - haszuje słowa z samymi dużymi literami
    pthread_create(&threads[3], &attr, kons, NULL);         // Tworzy czwarty wątek - wyświetla znaleznioen hasła

    for (int i = 0; i < 4; i++) {                           // Czeka aż wszystkie wątki zostną wykonane
        pthread_join(threads[i], NULL);
    }

    pthread_attr_destroy(&attr);    // Niszczy wątki
    pthread_mutex_destroy(&count_mutex);
    pthread_exit (NULL);
    free(dane.tablica);     // Zwalnia pamięć dla dynamicznych tablic
    free(dane.wynik);
    free(dane.haslo);
    free(dane.extra);
}
