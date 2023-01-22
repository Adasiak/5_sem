#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>
#include <openssl/evp.h>
#include <string.h>
#include <sys/shm.h>
#include <signal.h>

#define ID_PAMIEC 2342

pthread_mutex_t mutex;
int shmid;

// struktury dla watkow
struct pamiec_struct
{
    bool dzialanie;
    int ile_zlamanych;
    char zlamane_hashe[100][33];
    char zlamane_hasla[100][100];
};

// algorytm md5
void bytes2md5(const char *data, int len, char *md5buf)
{
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    const EVP_MD *md = EVP_md5();
    unsigned char md_value[EVP_MAX_MD_SIZE];
    unsigned int md_len, i;
    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, data, len);
    EVP_DigestFinal_ex(mdctx, md_value, &md_len);
    EVP_MD_CTX_free(mdctx);
    for (i = 0; i < md_len; i++)
    {
        snprintf(&(md5buf[i * 2]), 16 * 2, "%02x", md_value[i]);
    }
}

char skroty[1000][33] = {0};
int ile_skrotow = 0;
char *slownik;
int slownik_rozmiar;

// wczytywanie danych z plikow
bool wczytaj_dane(char *plik_skroty, char *plik_slownik)
{
    char skrot[33];

    ile_skrotow = 0;
    // ladowanie pliku z hashami
    FILE *f = fopen(plik_skroty, "r");
    if (f)
    {
        while (!feof(f))
        {
            int n = fscanf(f, "%s", skrot);
            if (n != 1)
                continue;
            strcpy(skroty[ile_skrotow++], skrot);
        }
        fclose(f);
    }
    else
    {
        printf("Blad odczytu pliku z hashami\n");
        return false;
    }

    // ladowanie pliku slownika
    f = fopen(plik_slownik, "r");
    if (f)
    {
        int pos = ftell(f);
        fseek(f, 0, SEEK_END);
        slownik_rozmiar = ftell(f);
        fseek(f, pos, SEEK_SET);
        slownik = (char *)malloc(slownik_rozmiar);
        fread(slownik, slownik_rozmiar, 1, f);
        fclose(f);
    }
    else
    {
        printf("Blad odczytu pliku slownika\n");
        return false;
    }
    return true;
}

bool md5_oblicz_i_porownaj(char *haslo, char *md5)
{
    char hmd5[33];
    bytes2md5(haslo, strlen(haslo), hmd5);

    if (strcmp(md5, hmd5) == 0)
        return true;
    return false;
}

// zamiana na male litery
char *zamien0(char *txt)
{
    for (int i = 0; i < strlen(txt); i++)
    {
        txt[i] = tolower(txt[i]);
    }
}

// zamiana pierwszej litery na duza a reszty na male
char *zamien1(char *txt)
{
    for (int i = 0; i < strlen(txt); i++)
    {
        if (i == 0)
            txt[i] = toupper(txt[i]);
        else
            txt[i] = tolower(txt[i]);
    }
}

// zamiana wszystkich liter na duze
char *zamien2(char *txt)
{
    for (int i = 0; i < strlen(txt); i++)
    {
        txt[i] = toupper(txt[i]);
    }
}

// lamanie hasha
bool lamanie(char *slowo, char *md5)
{

    char slowo_buf[100];
    strcpy(slowo_buf, slowo);
    // sprawdzenie hasla
    if (md5_oblicz_i_porownaj(slowo_buf, md5))
    {
        strcpy(slowo, slowo_buf);
        return true;
    }
    // sprawdzenie hasel z liczbami na poczatku
    for (int j = 0; j < 100; j++)
    {
        sprintf(slowo_buf, "%d%s", j, slowo);
        if (md5_oblicz_i_porownaj(slowo_buf, md5))
        {
            strcpy(slowo, slowo_buf);
            return true;
        }
    }

    // sprawdzenie hasel z liczbami na koncu
    for (int j = 0; j < 100; j++)
    {
        sprintf(slowo_buf, "%s%d", slowo, j);
        if (md5_oblicz_i_porownaj(slowo_buf, md5))
        {
            strcpy(slowo, slowo_buf);

            return true;
        }
    }

    // sprawdzenie  hasla z liczbami na poczatku i na koncu
    for (int j = 0; j < 100; j++)
    {
        for (int k = 0; k < 100; k++)
        {

            sprintf(slowo_buf, "%d%s%d", j, slowo, k);
            if (md5_oblicz_i_porownaj(slowo_buf, md5))
            {
                strcpy(slowo, slowo_buf);

                return true;
            }
        }
    }

    return false;
}

// dodanie zlamanego hasha do tablicy
void dodaj_zlamany_hash(char *hash, char *haslo, struct pamiec_struct *p)
{
    pthread_mutex_lock(&mutex);
    strcpy(p->zlamane_hashe[p->ile_zlamanych], hash);
    strcpy(p->zlamane_hasla[p->ile_zlamanych], haslo);
    p->ile_zlamanych++;
    pthread_mutex_unlock(&mutex);
}

// sprawdzenie w tablicy czy hash jest zlamany
bool zlamany_hash(char *hash, struct pamiec_struct *p)
{
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < p->ile_zlamanych; p++)
    {
        if (strcmp(hash, p->zlamane_hashe[i]) == 0)
        {
            pthread_mutex_unlock(&mutex);

            return true;
        }
    }
    pthread_mutex_unlock(&mutex);
    return false;
}

void lamanie_jednowyrazowe(int rodzaj, struct pamiec_struct *p)
{
    //kopiowanie bo strtok modyfikuje dane
    char *slownik_kopia = (char *)malloc(slownik_rozmiar);
    char slowo_buf[100];
    //petla przez skroty
    for (int i = 0; i < ile_skrotow; i++)
    {
        char *md5 = skroty[i];
        memcpy(slownik_kopia, slownik, slownik_rozmiar);
        char *end_str;
        char *slowo = strtok_r(slownik_kopia, "\n", &end_str);
        bool zlamane = false;

        // petla przez slownik
        while (slowo != NULL)
        {

            // koniec petli jesli hash zlamany przez inny watek
            if (zlamany_hash(md5, p))
                break;

            strcpy(slowo_buf, slowo);

            // sprawdzenie rodzaju producenta
            if (rodzaj == 0)
            {
                zamien0(slowo_buf);

                if (lamanie(slowo_buf, md5))
                {
                    dodaj_zlamany_hash(md5, slowo_buf, p);
                    zlamane = true;
                    break;
                }
            }
            else if (rodzaj == 1)
            {

                zamien1(slowo_buf);

                if (lamanie(slowo_buf, md5))
                {
                    dodaj_zlamany_hash(md5, slowo_buf, p);
                    zlamane = true;
                    break;
                }
            }
            else if (rodzaj == 2)
            {

                zamien2(slowo_buf);

                if (lamanie(slowo_buf, md5))
                {
                    dodaj_zlamany_hash(md5, slowo_buf, p);
                    zlamane = true;
                    break;
                }
            }

            slowo = strtok_r(NULL, "\n", &end_str);
        }
    }
    free(slownik_kopia);
    printf("Watek %d zakonczyl lamanie\n", rodzaj);
}

// lamanie hasel dwuwyrazowych
void lamanie_dwuwyrazowe(int rodzaj, struct pamiec_struct *p)
{
    char *slownik_kopia1 = (char *)malloc(slownik_rozmiar);
    char *slownik_kopia2 = (char *)malloc(slownik_rozmiar);

    char slowo_buf[100];

    for (int i = 0; i < ile_skrotow; i++)
    {
        if (!p->dzialanie)
            break;
        char *md5 = skroty[i];
        //kopiowanie bo strtok modyfikuje dane
        memcpy(slownik_kopia1, slownik, slownik_rozmiar);
        char *end_str;
        char *slowo1 = strtok_r(slownik_kopia1, "\n", &end_str); // pierwsze slowo

        bool zlamane = false;
        // petla przez slownik
        while (slowo1 != NULL)
        {
            if (!p->dzialanie) // sprawdzenie czy dzialac dalej
                break;
            // konczenie petli jesli inny watek zlamal hash
            if (zlamany_hash(md5, p))
                break;

            //ponowne kopiowanie
            memcpy(slownik_kopia2, slownik, slownik_rozmiar);
            char *end_str2;
            char *slowo2 = strtok_r(slownik_kopia2, "\n", &end_str2); // drugie slowo w hasle
            // druga petla przez slownik
            while (slowo2 != NULL)
            {

                if (!p->dzialanie)
                    break;

                if (zlamany_hash(md5, p))
                    break;

                sprintf(slowo_buf, "%s %s", slowo1, slowo2);

                //sprawdzanie rodzaju producenta
                if (rodzaj == 3)
                {
                    zamien0(slowo_buf); // male litery
                    if (lamanie(slowo_buf, md5))
                    {
                        dodaj_zlamany_hash(md5, slowo_buf, p);

                        zlamane = true;
                        break;
                    }
                }
                if (rodzaj == 4)
                {
                    zamien1(slowo_buf); // pierwsza litera duza
                    if (lamanie(slowo_buf, md5))
                    {
                        dodaj_zlamany_hash(md5, slowo_buf, p);

                        zlamane = true;
                        break;
                    }
                }

                else if (rodzaj == 5)
                {
                    zamien2(slowo_buf); // wszystkie litery duze
                    if (lamanie(slowo_buf, md5))
                    {
                        dodaj_zlamany_hash(md5, slowo_buf, p);

                        zlamane = true;
                        break;
                    }
                }

                slowo2 = strtok_r(NULL, "\n", &end_str2);
            }
            strcpy(slowo_buf, slowo1);

            slowo1 = strtok_r(NULL, "\n", &end_str);
        }
    }
    free(slownik_kopia1);
    free(slownik_kopia2);
}

// funkcja watku

void *start_thread(void *w)
{
    int i;
    int *nptr = (int *)w; // zamiana na wskaznik do int
    int n = *nptr;        // pobranie liczby

    struct pamiec_struct *p = (struct pamiec_struct *)shmat(shmid, NULL, 0);
    if (p == NULL)
    {
        printf("Blad przylaczania segmentu pamieci wspoldzielonej\n");
        exit(1);
    }

    printf("Startuje watek o numerze %d\n", n);
    if (n > 2)
        lamanie_dwuwyrazowe(n, p);
    else
        lamanie_jednowyrazowe(n, p);

    printf("Watek producenta %d zakonczyl dzialanie\n", n);
    return NULL;
}

void wypisz_zlamane(struct pamiec_struct *p)
{
    printf("Zlamane hashe:\n");
    for (int i = 0; i < p->ile_zlamanych; i++)
    {
        printf("%s - %s\n", p->zlamane_hashe[i], p->zlamane_hasla[i]);
    }
}
void *start_thread_consumer(void *w)
{

    struct pamiec_struct *p = (struct pamiec_struct *)shmat(shmid, NULL, 0);
    if (p == NULL)
    {
        printf("Blad przylaczania segmentu pamieci wspoldzielonej\n");
        exit(1);
    }

    printf("Startuje watek konsumenta\n");

    int ile_zlamanych_hashy = 0;
    while (true) // petla dopoki wszystkie hashe nie zostana obliczone
    {
        if (!p->dzialanie)
            break;

        sleep(1);
        pthread_mutex_lock(&mutex);

        // sprawdzenie czy zmienila sie ilosc zlamanych hashy
        if (p->ile_zlamanych > ile_zlamanych_hashy)
        {
            wypisz_zlamane(p);
            ile_zlamanych_hashy = p->ile_zlamanych;
        }
        pthread_mutex_unlock(&mutex);
    }

    printf("Watek konsumenta zakonczyl dzialanie\n");
    return NULL;
}



int main(int argc, char *argv[])
{
    pthread_mutex_init(&mutex, NULL);

    pthread_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, tc = 0; // wskazniki watkow

    // tworzenie pamieci wspoldzielonej
    int mem_size = sizeof(struct pamiec_struct);
    shmid = shmget(ID_PAMIEC, mem_size, IPC_CREAT | 0600);
    if (shmid == -1)
    {
        printf("Blad tworzenia segmentu pamieci wspoldzielonej\n");
        exit(1);
    }

    // pobranie wskaznika do pamieci wspoldzielonej
    struct pamiec_struct *p = (struct pamiec_struct *)shmat(shmid, NULL, 0);
    if (p == NULL)
    {
        printf("Blad przylaczania segmentu pamieci wspoldzielonej\n");
        exit(1);
    }

    char nazwa_pliku_slownika[100];
    char nazwa_pliku_hashy[100];

    size_t ile;
    while (true)
    {

        printf("Podaj nazwe pliku slownika: ");
        scanf("%s", nazwa_pliku_slownika);
        printf("Podaj nazwe pliku z hashami: ");
        scanf("%s", nazwa_pliku_hashy);


        p->dzialanie = false;
        if (tc) // jeśli watek konsumenta byl wczesniej utworzony
        {
            // to konczy dzialanie
            p->dzialanie = false;

            // czekanie na zakonczenie watkow
            pthread_join(t1, NULL);
            pthread_join(t2, NULL);
            pthread_join(t3, NULL);
            pthread_join(t4, NULL);
            pthread_join(t5, NULL);
            pthread_join(t6, NULL);

            pthread_join(tc, NULL);

            free(slownik);
        }

        // wczytanie danych z plikow
        if (!wczytaj_dane(nazwa_pliku_hashy, nazwa_pliku_slownika))
            return 1;
        // reset zmiennych w pamieci wspoldzielonej
        p->ile_zlamanych = 0;
        p->dzialanie = true;

        // uruchomienie watku konsumenta
        pthread_create(&tc, NULL, &start_thread_consumer, NULL);

        // uruchomienie watkow producentow wraz z numerem oznaczajacym rodzaj producenta
        int n0 = 0;
        pthread_create(&t1, NULL, &start_thread, &n0);
        int n1 = 1;
        pthread_create(&t2, NULL, &start_thread, &n1);
        int n2 = 2;
        pthread_create(&t3, NULL, &start_thread, &n2);

       // wersja dla haseł dwuwyrazowych
//        int n3 = 3;
//        pthread_create(&t4, NULL, &start_thread, &n3);
//        int n4 = 4;
//        pthread_create(&t5, NULL, &start_thread, &n4);
//        int n5 = 5;
//        pthread_create(&t6, NULL, &start_thread, &n5);
    }

    // zwolnienie pamieci wspoldzielonej
    shmdt((struct pamiec_struct *)p);

    free(slownik);

    pthread_mutex_destroy(&mutex);

    return 0;
}