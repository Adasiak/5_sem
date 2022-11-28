#include <errno.h> 
#include <fcntl.h> 
#include <limits.h> 
#include <stdio.h> 
#include <string.h> 
#include <unistd.h> 
#define fifoPipe "pipe2" 
#define packageSize 16 

int main(int argc,char *argv[]){ 
    
    int pipefd,filefd,countWords; 
    char buf[255]; 
    int texts=argc; 
    mode_t mask = umask(0); 
    
    if((pipefd=open(fifoPipe,O_WRONLY)) ==-1){ 
        
        fprintf(stderr,"Cos sie popsulo w potoku \n"); 
        return-1; 
    } 
    while(--texts>0){ 
        if((filefd=open(argv[texts],O_RDONLY)) < 0){ 
            fprintf(stderr,"Blad open \n"); 
            return-1; 
        } 
        while((countWords=read(filefd,buf,packageSize))>0){ 
            if(write(pipefd,buf,countWords)<0){ 
                fprintf(stderr,"Blad write"); 
            } 
        } 
        fprintf(stdout,"Pomiędzy przesyłaniem zawartości poszczególnych plików program ma czekać zauważalny czas"); 
        sleep(5); 
    } 
    close(pipefd); 

}

//klient

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO "potok"

int main(){
        int potok_fd;
        int licznik;                    // Liczba odczytanych bajtów
        char bufor[8];                  // Partie plików do przesyłania
                                                // przez potok

    printf("Tworzenie potoku\n");
        mkfifo(FIFO, 0666);                                 // Funkcaja mkfifo tworzy potok
    printf("Potok został poprawnie utworzony\n");
        potok_fd = open(FIFO, O_RDONLY);                    //Otwiera potok

        while ((licznik=read(potok_fd, bufor, 1)) > 0){     // Odczytuje z potoku i zapisuje do bufora
                write(1, bufor, licznik);                       // Wypisuje zawartosc bufora
        }

        close(potok_fd);                                    // Zamyka potok
        return 0;

}



//serwer

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO "potok"


int main(){
        const int rozmiar = 128;
        int potok_fd;
        int deskryptory[20];
        int licznik;                            // Liczba odczytanych bajtów
        char bufor[rozmiar];                    // Partie plików do przesyłania
                                                        // przez potok
        char nazwa[64];                     // Nazwa otwieranego pliku
        char *potok = "potok";          // Wskaznik
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