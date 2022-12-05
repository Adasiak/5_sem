#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>
#include <errno.h>


#define BABBLE_NAME "/Y01-42e"
#define BABBLE_MODE 0777
#define BABBLE_LIMIT 32
#define BABBLE_LENGTH 80

struct babblespace
{
        pthread_mutex_t babble_mutex,m2;
        pthread_cond_t babble_cond;
        int babble_first, babble_total;
        char babbles [BABBLE_LIMIT][BABBLE_LENGTH];
};

int main()
{
        struct babblespace * segment;
        int fd;
        fd = shm_open(BABBLE_NAME , O_RDWR , BABBLE_MODE);

        char buf[BABBLE_LENGTH] = "kkruszyn";


        if(fd == -1)
        {
                printf ("Open failed:%s\n", strerror(errno));
                exit(0);
        }

        segment  = mmap(NULL,sizeof(struct babblespace),PROT_READ | PROT_WRITE,
MAP_SHARED ,fd,0);

        if(segment == MAP_FAILED)
        {
                printf ("Map failed: %s\n", strerror(errno));
                exit(0);
        }

printf("Babble total: %d\n",segment->babble_total);
printf("Babble first: %d\n", segment-> babble_first);


pthread_mutex_lock (&segment->babble_mutex);

for(int i = 1; i < segment->babble_total; i++)

        printf("Wiersz %d%s\n",i,segment -> babbles[i]);

memcpy(segment->babbles[13],buf,BABBLE_LENGTH);

pthread_mutex_unlock(&segment->babble_mutex);


        return(0);
}