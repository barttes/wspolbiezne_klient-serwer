#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#define MAX 512

int main() {
    
    int fd;
    int n, e;
    char input[MAX];
    char buf[MAX]; memset(buf, 0, sizeof(buf));
    char output[MAX]; memset(output, 0, sizeof(output));
    
    //otwarcie kolejki serwera
    if ((fd = open("s_fifo", O_WRONLY)) == -1) {
        perror("błąd otwarcia kolejki serwera");
        exit(1);
    }
    
    //utworzenie kolejki klienta
    char c_fifoStr[MAX]; memset(c_fifoStr, 0, sizeof(c_fifoStr));
    printf("Podaj nazwę kolejki klienta: ");
    fgets(c_fifoStr, MAX, stdin);
    c_fifoStr[strlen(c_fifoStr) - 1] = 0;
    
    int cfd;
    int c_fifo;
    if ((c_fifo = mkfifo(c_fifoStr, S_IRUSR | S_IWUSR)) == -1) {
        perror("błąd utworzenia kolejki klienta");
        exit(1);
    }
    
    
    printf("Wpisz \'close\' w celu zamknięcia programu ... \n");
    
    while (1) {
        
        printf("> ");
        
        //pobieranie linii od użytkownika
        fgets(input, MAX, stdin);
        input[strlen(input) - 1] = 0;
        
        
        //przygotowanie tekstu wyjściowego do potoku
        //printf("Input: %s\n", input);
        //printf("Kolejka: %s\n", c_fifoStr);
        strcat(output, c_fifoStr);
        strcat(output, " ");
        strcat(output, input);
        //printf("Output: %s\n", output);
        
        //zamknięcie konsoli, gdy input == close
        if (!strcmp(input, "close")) {
            break;
        }
        
        //zapis ciągu znaków do kolejki serwera
        if (write(fd, output, sizeof(output)) == -1) {
            perror("błąd zapisu");
            exit(1);
        }
        
        //otwarcie kolejki klienta
        if ((cfd = open(c_fifoStr, O_RDONLY)) == -1 ) {
            perror("błąd otwarcia kolejki klienta");
            exit(1);
        }
        
        while ((n = read(cfd, buf, sizeof(buf))) > 0) {
            write(1, buf, sizeof(buf));
        }
        if (n == -1) {
            perror("błąd odczytu z bufora");
            exit(1);
        }
        if ((e = close(cfd)) == -1){
            perror("błąd zamknięcia pliku");
        }
        
        
        
        memset(buf, 0, sizeof(buf));
        memset(output, 0, sizeof(output));
        
    }
    
    if ((e = close(fd)) == -1){
        perror("błąd zamknięcia pliku");
    }
    
    if ((e = unlink(c_fifoStr)) == -1){
        perror("błąd usunięcia pliku");
    }
    
    return 0;
}
