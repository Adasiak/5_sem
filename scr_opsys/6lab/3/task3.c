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
