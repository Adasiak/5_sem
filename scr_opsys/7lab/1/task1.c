#include <stdio.h>
#include <unistd.h> 
#include <string.h> 
#include <fcntl.h> 
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>

#define BABBLE_NAME "/Y01-42e"
#define BABBLE_MODE 0777
#define BABBLE_LIMIT 32
#define BABBLE_LENGTH 80

struct babblespace {
  pthread_mutex_t babble_mutex;
  pthread_cond_t babble_cond;
  int babble_first, babble_total;
  char babbles[BABBLE_LIMIT][BABBLE_LENGTH];
};


int main()
{
    struct babblespace * segment;
    
    int fd,i;
    char *addr ,c;
    fd = shm_open(BABBLE_NAME, O_RDONLY, BABBLE_MODE);
    int mm;
    
    mm = mmap(NULL, BABBLE_LENGTH, PROT_READ, MAP_PRIVATE, MAP_SHARED, NULL);

    if(fd < 0){
      printf("\nError opening\n");
    //   exit(-1);
        printf("%d\n", fd);
    }
    printf("%d\n", fd);

    segment = (struct babblespace *) mmap(NULL, sizeof(struct babblespace),PROT_READ,MAP_SHARED,fd,0);
    if(segment == MAP_FAILED){
      printf("\nError map\n");
    //   exit(-1);
    }
    printf("%s\n", segment-> babbles[1]);
    return 0;


}
