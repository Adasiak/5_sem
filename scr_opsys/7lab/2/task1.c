#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>

#define BABBLE_NAME "/Y01-42e" 
#define BABBLE_MODE 0777
#define BABBLE_LIMIT 32
#define BABBLE_LENGTH 80

struct babblespace {
  pthread_mutex_t babble_mutex,m2;
  pthread_cond_t babble_cond;
  int babble_first, babble_total;
  char babbles[BABBLE_LIMIT][BABBLE_LENGTH];
};

int main(){
  struct babblespace* segment;
  char bufor[BABBLE_LENGTH] = "WA";
  int fd=shm_open(BABBLE_NAME, O_RDWR , BABBLE_MODE);
  if(fd<0){
    perror("shm_open");
    printf("%d\n",fd);

  }

  segment = (struct babblespace*) mmap(NULL,sizeof(struct babblespace),
    PROT_READ,MAP_SHARED, fd, 0);
  

  pthread_mutex_lock(&segment->babble_mutex);

    printf("%d\n",segment->babble_first);
    printf("%d\n",segment->babble_total);
    // segment->babble_total=
  if(segment == MAP_FAILED) perror("map");
  for(int i;i<segment->babble_total;i++)
  {
    printf("%s\n", segment->babbles[i]);
  }
  memcpy(segment->babbles,bufor,BABBLE_LENGTH);
  pthread_mutex_unlock(&segment->babble_mutex);




  return 0;
}
