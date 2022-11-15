#include <stdio.h> 
#include <unistd.h> 
#include <string.h> 
#include <fcntl.h> 
#define MaxSize 16 


int main(int argc,char *argv[]){
    
    int pipefd[2],filefd,countWords; 
    char buf[MaxSize]; 
    
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
    
        while((countWords=read(pipefd[0],buf,MaxSize))>0){
    
            write(1," #",2*sizeof(char))<0?fprintf(stderr,"Blad write"):0; 
            write(1,buf,countWords)<0?fprintf(stderr,"Blad write"):0; 
            write(1,"# ",2*sizeof(char))<0?fprintf(stderr,"Blad write"):0;
    
        } 
        close(pipefd[0]);
    }
    else{ 
        close(pipefd[0]);
        if((filefd=open(argv[1],O_RDONLY)) < 0){ 
            fprintf(stderr,"Blad open \n");
            return-1;
    
        } 
        while((countWords=read(filefd,buf,MaxSize))>0){ 
    
            if(write(pipefd[1],buf,countWords)<0){ 
                fprintf(stderr,"Blad write"); 
    
            } 
        } 
        close(pipefd[1]); 
    }
}