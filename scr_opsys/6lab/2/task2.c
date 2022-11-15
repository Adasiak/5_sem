#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#define MaxSize 16


int main(int argc,char *argv[]){

    int pipefd[2],filefd,countWords,c;
    char buf[MaxSize];
        FILE *file;

    if (pipe(pipefd) ==-1) {
        fprintf(stderr,"Cos sie popsulo w potoku \n");
        return-1;
    }

    pid_t pid = fork();

    if(pid <0){
        fprintf(stderr,"Cos sie popsulo w procesie\n");
        return-1;
    }
    else if(pid==0){

        close(pipefd[1]);
        close(0);
        dup(pipefd[0]);
        execl("/usr/bin/display","display", (char*)NULL);
        read(pipefd[0], &c,1 );
        exit(1);
        close(pipefd[0]);
    }
    else{
        close(pipefd[0]);
        if((filefd=open(argv[1],O_RDONLY)) < 0){
            fprintf(stderr,"Blad open \n");
            return-1;

        }
        file=fopen(argv[1],"r");
        if(file==NULL)
                printf("FILE error\n");
        while(!feof(file)){
                c=getc(file);
                write(pipefd[1], &c, 1);
        }
        close(pipefd[1]);
    }
}