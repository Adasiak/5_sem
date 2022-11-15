#include <stdio.h> 
#include <unistd.h> 
#include <fcntl.h> 

#define BUF_SIZE 256

int main() {

    pid_t pid;
    int fd[2], in_fd, n;
    char buf[BUF_SIZE];
    char fname[32];
    
    if(pipe(fd) < 0) {

        fprintf(stderr, "Failed creating pipe\n");
        return 1;
    }

    pid = fork();

    if(pid == 0) {
        //child
        close(fd[1]);
        close(0);
        dup(fd[0]);
        close(fd[0]);

        //!!!REALLY IMPORTANT!!!
        //Arguments which you pass to execlp depends on image dispaly program that you are
        //using. Always check what arguments need to be provided in order to read the image
        //from stdin. E.g. ImageMagick accepts NULL or "-".GraphicsMagick on the other hand
        //accepts only "-".
        execlp("display", "display", "-", NULL);
    }
    else {
        //parent
        close(fd[0]);
        printf("Please input your image path along with its extension (e.g. image.jpg | ~/Desktop/image.jpg):\n");
        scanf("%s", fname);

        if((in_fd = open(fname, O_RDONLY)) < 0) {
            
            fprintf(stderr, "Failed opening file\n");
            return 2;
        }
        
        while((n = read(in_fd, &buf, BUF_SIZE)) > 0) {
    
            if(write(fd[1], &buf, n) < 0) {
                fprintf(stderr, "Failed writing to pipe\n");
                return 3;
            }  
        }
        close(in_fd);
    } 
}
