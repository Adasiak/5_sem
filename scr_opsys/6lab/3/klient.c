#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO "FIFO"

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
