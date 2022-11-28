#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO "FIFO"


int main(){
        const int rozmiar = 128;
        int potok_fd;
        int deskryptory[20];
        int licznik;                            // Liczba odczytanych bajtów
        char bufor[rozmiar];                    // Partie plików do przesyłania
                                                        // przez potok
        char nazwa[64];                     // Nazwa otwieranego pliku
        char *potok = "FIFO";          // Wskaznik
        int tab[5];                     // Tablica

        printf("Tworzenie potoku\n");
        mkfifo(FIFO, 0666);             // Funkcaja mkfifo tworzy potok
        printf("Potok został poprawnie utworzony\n");
        potok_fd = open(FIFO, O_WRONLY);    //Otwiera potok
        printf("%d \n", potok_fd);      // Wyswietla wartosc zwrocona przez open()
        int i = 0;                      // Licznik
        while(1){
        printf("Opcja - \" q \" zamyka odczyt\n");
                printf("Podaj nazwe pliku :\t");
                scanf("%s", nazwa);                         // Wczytuje nazwe pliku
                if(nazwa[0] == 'q')                         // Jezeli zostaje podana opcja 'q' zostaje przerwane
                        break;
                deskryptory[i] = open(nazwa, O_RDONLY);     // Sprawdza poprawnosc otworzenia pliku
                if(deskryptory[i] < 0)                      // Jezeli open zwrocil wartosc mniejsza od 0 to
                        puts("Nie udalo sie otworzyc pliku");   // zwraca komunikat

                while ((licznik=read(deskryptory[i], bufor, rozmiar)) > 0){

                        write(potok_fd, bufor, licznik);        // Zapisuje bufor do potoku
                }
                ++i;                                        // Zwieksza zakres
        }
        close(potok_fd);                                //Zamyka potok
        return 0;
}