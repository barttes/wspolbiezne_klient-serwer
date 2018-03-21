#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#define MAX 512

char * przypiszArgument(char * iM) {
    char * argumentZwracany = malloc(strlen(iM));
    strcpy(argumentZwracany, iM);
    return argumentZwracany;
}

int main() {
    
    int n, e;
    pid_t p;
    int s_fifo;
    int cfd;
    
    //utworzenie kolejki serwera
    if ((s_fifo = mkfifo("s_fifo", S_IRUSR | S_IWUSR)) == -1) {
        perror("błąd tworzenia kolejki serwera");
        exit(1);
    }
    int sfd;
    if ((sfd = open("s_fifo", O_RDONLY)) == -1) {
        perror("błąd otwarcia kolejki serwera");
        exit(1);
    }
    
    //utworzenie bufora
    char buf[MAX]; memset(buf, 0, sizeof(buf));

    //odczyt danych z kolejki fifo serwera
    while ((n = read(sfd, buf, sizeof(buf))) > 0) {

        //tworzenie tablicy argumentów
        char * argumenty[100];
        char c_fifoStr[MAX];
        int argument = -1;
        char * inputZmodyfikowany = strtok(buf, " ");
        while (inputZmodyfikowany!=NULL) {
            if (argument > -1) {
                 argumenty[argument] =   przypiszArgument(inputZmodyfikowany);
                 //printf("arg %d: %s\n", argument-1,  argumenty[argument-1]);
                 inputZmodyfikowany = strtok(NULL, " ");
                 
            } else {
                 strcpy(c_fifoStr, inputZmodyfikowany);
                 inputZmodyfikowany = strtok(NULL, " ");
            }
            argument++;
            
        }
        argumenty[argument] = NULL;
        
        if ((p = fork())==0) {
            
            //otwarcie pliku z kolejką fifo klienta
            if ((cfd = open(c_fifoStr, O_WRONLY)) == -1) {
                perror("błąd otwarcia kolejki klienta");
                exit(1);
            }
            
            dup2(cfd,1);
            dup2(cfd,2);
            //execlp("ls", "ls", "-l", NULL);
            if ((e = execvp(argumenty[0], argumenty)) == -1) {
                
                perror("błąd uruchomienia programu");
                exit(1);
            }
            
            if ((e = close(cfd)) == -1){
                perror("błąd zamknięcia pliku");
            }
            
        } else if (p == -1) {
            perror("błąd utworzenia procesu potomnego");
            exit(1);
        } 
        else {
            wait(NULL);
        }
        
        memset(buf, 0, sizeof(buf));
    }
    if (n == -1) {
        perror("błąd odczytu z bufora");
        exit(1);
    }
    
    if ((e = close(sfd)) == -1){
        perror("błąd zamknięcia pliku");
        exit(1);
    }

    if ((e = unlink("s_fifo")) == -1){
        perror("błąd usunięcia pliku");
        exit(1);
    }
    
    return 0;
}
