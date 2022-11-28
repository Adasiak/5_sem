#include <stdio.h>
#include <unistd.h> 
#include <string.h> 
#include <fcntl.h> 
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <pthread.h>


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
    int fd,i;
    char *addr ,c;
    fd = shm_open(BABBLE_NAME, O_RDWR, BABBLE_MODE);

    if(fd < 0)
    {
      printf("Error opening");
      perror(*shm_open);
      exit(-1);
    }
    if(mmap(NULL, BABBLE_LENGTH, PROT_READ) < 0)
    { 
      perror(*mmap);
      exit(-1);
    }


}