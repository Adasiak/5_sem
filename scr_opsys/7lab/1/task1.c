#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

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

int main(){
  struct babblespace* segment;
  int fd=shm_open(BABBLE_NAME, O_RDONLY , BABBLE_MODE);
  if(fd<0){
    perror("shm_open");
    printf("%d\n",fd);

  }
  printf("%d\n",fd);
  segment = (struct babblespace*) mmap(NULL,sizeof(struct babblespace),
PROT_READ,MAP_SHARED, fd, 0);
  //segment->babble_total=3;
  if(segment == MAP_FAILED) perror("map");
  for(int i;i<5;i++)
  {
    printf("%s\n", segment->babbles[i]);
  }
  return 0;
}
