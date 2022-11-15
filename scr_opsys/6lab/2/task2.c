#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


int main()
{
    int pipe_fd[2];
    int c;
    FILE *file;


    pipe(pipe_fd);

    if(fork()==0)
    {
        //child
        close(pipe_fd[1]);
        close(0);
        dup(pipe_fd[0]);
        execl("/usr/bin/display","display", (char*)NULL);
        read(pipe_fd[0], &c, 1);
        exit(1);
    }
    // parent
    close(pipe_fd[0]);
    file=fopen("Dame.jpeg","r");
    
    if(file==NULL){
        printf("File error\n");
        exit(1);
    }
    else{
        while(!feof(file))
        {
            c=getc(file);
            write(pipe_fd[1], &c, 1);
        }  
    }
}