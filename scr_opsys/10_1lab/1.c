#include <openssl/evp.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "passwords.h"

var variables;


void bytes2md5(const char *data, int len, char *md5buf) {
        EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
        const EVP_MD *md = EVP_md5();
        unsigned char md_value[EVP_MAX_MD_SIZE];
        unsigned int md_len, i;
        EVP_DigestInit_ex(mdctx, md, NULL);
        EVP_DigestUpdate(mdctx, data, len);
        EVP_DigestFinal_ex(mdctx, md_value, &md_len);
        EVP_MD_CTX_free(mdctx);
        for (i = 0; i < md_len; i++) {
                snprintf(&(md5buf[i * 2]), 16 * 2, "%02x", md_value[i]);
        }
}


void read_hasla() {
int indeks[1000];
char name[1000][30];
FILE* fp;
char str[150];
int i =0;

fp = fopen(variables.plik,"r");
if(fp==NULL) {
        printf("Zly plik - wczytuje domyslny\n");
        exit(1);
        strcpy(variables.plik,"Hasla.txt");
        fp=fopen(variables.plik,"r");
}

while (fgets(str,150, fp)) {
        variables.first_count_hasla++;
        sscanf(str,"%d %s %s %s",&indeks[i],&variables.hasz[i],&variables.email[i],&name[i]);
        i++;
}
//printf("%d\n",first_count_hasla);
fclose(fp);
}
void read_slownik() {
FILE* fp;
char str[150];
char tmp_buf[30];
int i=0;
int second_count = 1;
fp = fopen("Slownik.txt","r");
while (fgets(str,150, fp)) {
        variables.first_count_slownik++;
}
fclose(fp);
variables.slownik = (char**)malloc(variables.first_count_slownik * sizeof(char*));
variables.tmp= (char**)malloc(variables.first_count_slownik * sizeof(char*));
fp = fopen("Slownik.txt","r");

while (fgets(str,150, fp)) {
        for(int i=0; tmp_buf[i]!='\0';i++) {
                second_count++;
        }
        variables.slownik[i]=(char*)malloc(second_count*sizeof(char));
        variables.tmp[i]=(char*)malloc(second_count*sizeof(char));
        sscanf(str,"%s",tmp_buf);
        strcpy(variables.slownik[i],tmp_buf);
        i++;
        second_count=1;
}
//printf("%d\n",first_count_slownik);
fclose(fp);

}
int newfile() {
        char tmp_plik[20];
        printf("Mozesz podac nowy plik do wczytania: \n");
        scanf("%s",tmp_plik);
        strcpy(variables.plik,tmp_plik);
        printf("Nowy plik:%s\n",variables.plik);
        variables.k=0;
        variables.number=1;

}


char *build_haslo(int flaga1, int flaga2, int version,char* tmp_slownik) {
        char result[30];
        char c1[3],c2[3];
        if(version==1) {
                sprintf(c1,"%d",flaga1);
                result[0] = '\0';
                strcat(result,c1);
                strcat(result,tmp_slownik);
                strcpy(tmp_slownik,result);
        }
        else if(version==2) {
                sprintf(c1,"%d",flaga1);
                result[0] = '\0';
                strcat(result,tmp_slownik);
                strcat(result,c1);
                strcpy(tmp_slownik,result);
        }
        else if(version==3) {
                sprintf(c1,"%d",flaga1);
                sprintf(c2,"%d",flaga2);
                result[0] = '\0';
                strcat(result,c1);
                strcat(result,tmp_slownik);
                strcat(result,c2);
                strcpy(tmp_slownik,result);
        }
        return tmp_slownik;
}
int check(int i,char*md5,char*tmp_slownik) {
        if(strcmp(md5,variables.hasz[i])==0) {
                pthread_mutex_lock(&variables.count_mutex);
                strcpy(variables.tmp[variables.k],tmp_slownik);
                strcpy(variables.tmp_email[variables.k],variables.email[i]);
                variables.k++;
                variables.flaga = true;
                pthread_mutex_unlock(&variables.count_mutex);
                return 1;
        }
        return 0;
}
int compare(char* tmp_slownik,int i) {
        char md5[33];
        int flaga1,flaga2;//prefixy
        char c1[3],c2[3];
        for (flaga1=0;flaga1<100;flaga1++) {
                                bytes2md5(tmp_slownik, strlen(tmp_slownik), md5);
                                if(check(i,md5,tmp_slownik)==1) {
                                        return 1;
                                }
                                        if(flaga1) {
                                        memmove(tmp_slownik,tmp_slownik+1,strlen(tmp_slownik));//kopia z jednej jednej do drugiej
        }
                                        if(flaga1 >10) {
                                                memmove(tmp_slownik,tmp_slownik+1,strlen(tmp_slownik));
                                        }
                                        strcpy(tmp_slownik,build_haslo(flaga1,0,1,tmp_slownik));//lancuch znakow do do tablicy
                                }
                                memmove(tmp_slownik,tmp_slownik+1,strlen(tmp_slownik));
                                memmove(tmp_slownik,tmp_slownik+1,strlen(tmp_slownik));
                                for (flaga1=0;flaga1<100;flaga1++) {
                                bytes2md5(tmp_slownik, strlen(tmp_slownik), md5);
                                if(check(i,md5,tmp_slownik)==1) {
                                        return 1;
                                }
                                        if(flaga1) {
                                        tmp_slownik[strlen(tmp_slownik)-1] = '\0';
                                        }
                                        if(flaga1 >10) {
                                                tmp_slownik[strlen(tmp_slownik)-1] = '\0';
                                        }
                                        strcpy(tmp_slownik,build_haslo(flaga1,0,2,tmp_slownik));

                                }
                                tmp_slownik[strlen(tmp_slownik)-1] = '\0';
                                tmp_slownik[strlen(tmp_slownik)-1] = '\0';
                                for (flaga1=0;flaga1<100;flaga1++) {
                                        for (flaga2=0;flaga2<100;flaga2++) {
                                        strcpy(tmp_slownik,build_haslo(flaga1,flaga2,3,tmp_slownik));
                                bytes2md5(tmp_slownik, strlen(tmp_slownik), md5);
                                if(check(i,md5,tmp_slownik)==1) {
                                        return 1;
                                }
                                        tmp_slownik[strlen(tmp_slownik)-1] = '\0';
                                        memmove(tmp_slownik,tmp_slownik+1,strlen(tmp_slownik));
                                        if(flaga1 >=10) {
                                                memmove(tmp_slownik,tmp_slownik+1,strlen(tmp_slownik));
                                        }
                                        if(flaga2 >=10) {
                                                tmp_slownik[strlen(tmp_slownik)-1] = '\0';
                                        }
                                        }
                                }
                                return 0;
}

void *producent0 () {
        char tmp_slownik[100];
        for (int i=0; i<variables.first_count_hasla; i++) {
                for(int j=0;j<variables.first_count_slownik-1;j++) {
                        pthread_mutex_lock(&variables.count_mutex);
                        strcpy(tmp_slownik,variables.slownik[j]);
                        pthread_mutex_unlock(&variables.count_mutex);
                        if(variables.check_producent1) {
                                pthread_mutex_lock(&variables.count_mutex);
                                variables.check_producent1 = false;
                                pthread_mutex_unlock(&variables.count_mutex);
                                break;
                        }
                        if(variables.check_producent2) {
                                pthread_mutex_lock(&variables.count_mutex);
                                variables.check_producent2 = false;
                                pthread_mutex_unlock(&variables.count_mutex);
                                break;
                        }
                        if(compare(tmp_slownik,i)==1) {
                                pthread_mutex_lock(&variables.count_mutex);
                                variables.check_producent0 = true;
                                pthread_mutex_unlock(&variables.count_mutex);
                                break;
                        }
                }
        }
        pthread_exit(NULL);
}
void *producent1 () {
        char tmp_slownik[100];
        for (int i=0; i<variables.first_count_hasla; i++) {
                for(int j=0;j<variables.first_count_slownik-1;j++) {
                        pthread_mutex_lock(&variables.count_mutex);
                        strcpy(tmp_slownik,variables.slownik[j]);
                        pthread_mutex_unlock(&variables.count_mutex);
                        tmp_slownik[0] = toupper(tmp_slownik[0]);
                        if(variables.check_producent0) {
                                pthread_mutex_lock(&variables.count_mutex);
                                variables.check_producent0 = false;
                                pthread_mutex_unlock(&variables.count_mutex);
                                break;
                        }
                        if(variables.check_producent2) {
                                pthread_mutex_lock(&variables.count_mutex);
                                variables.check_producent2 = false;
                                pthread_mutex_unlock(&variables.count_mutex);
                                break;
                        }
                        if(compare(tmp_slownik,i)==1) {
                                pthread_mutex_lock(&variables.count_mutex);
                                variables.check_producent1 = true;
                                pthread_mutex_unlock(&variables.count_mutex);
                                break;
                        }
                }
        }
pthread_exit(NULL);
}
void *producent2 () {
        char tmp_slownik[100];
        int h=0;
        for (int i=0; i<variables.first_count_hasla; i++) {
                for(int j=0;j<variables.first_count_slownik-1;j++) {
                        pthread_mutex_lock(&variables.count_mutex);
                        strcpy(tmp_slownik,variables.slownik[j]);
                        pthread_mutex_unlock(&variables.count_mutex);
                        while(tmp_slownik[h]) {
                                tmp_slownik[h] = toupper(tmp_slownik[h]);
                                h++;
                        }
                        h=0;
                        if(variables.check_producent0) {
                                pthread_mutex_lock(&variables.count_mutex);
                                variables.check_producent0 = false;
                                pthread_mutex_unlock(&variables.count_mutex);
                                break;
                        }
                        if(variables.check_producent1) {
                                pthread_mutex_lock(&variables.count_mutex);
                                variables.check_producent1 = false;
                                pthread_mutex_unlock(&variables.count_mutex);
                                break;
                        }
                        if(compare(tmp_slownik,i)==1) {
                                pthread_mutex_lock(&variables.count_mutex);
                                variables.check_producent2 = true;
                                pthread_mutex_unlock(&variables.count_mutex);
                                break;
                        }
                }
        }

pthread_exit(NULL);
}

void *producent3() {
        char tmp_slownik[100];
        bool flag = false;
        char md5[33];
        char space[3];
        size_t length = strlen(space);
        space[length] = ' ';
        space[length+1] = '\0';
        for (int i=0; i<variables.first_count_hasla; i++) {
                for(int j=0;j<variables.first_count_slownik-1 && !flag;j++) {
                        for(int h=0;h<variables.first_count_slownik-1;h++) {
                                char *result=(char*)malloc(100*sizeof(char));
                                pthread_mutex_lock(&variables.count_mutex);
                                strcat(result,variables.slownik[j]);
                                strcat(result,space);
                                strcat(result,variables.slownik[h]);
                                strcpy(tmp_slownik,result);
                                free(result);
                                pthread_mutex_unlock(&variables.count_mutex);
                                //printf("%s\n",tmp_slownik);
                                bytes2md5(tmp_slownik, strlen(tmp_slownik), md5);
                                if(variables.check_producent4) {
                                        pthread_mutex_lock(&variables.count_mutex);
                                        variables.check_producent4 = false;
                                        pthread_mutex_unlock(&variables.count_mutex);
                                        break;
                                }
                                if(variables.check_producent5) {
                                        pthread_mutex_lock(&variables.count_mutex);
                                        variables.check_producent5 = false;
                                        pthread_mutex_unlock(&variables.count_mutex);
                                        break;
                                }
                                if(check(i,md5,tmp_slownik)==1) {
                                        pthread_mutex_lock(&variables.count_mutex);
                                        variables.check_producent3 = true;
                                        pthread_mutex_unlock(&variables.count_mutex);
                                        flag = true;
                                        break;
                        }
                        }
                }
                flag = false;
        }

        pthread_exit(NULL);
}
void *producent4() {
        char tmp_slownik[100];
        bool flag = false;
        char md5[33];
        char space[3];
        size_t length = strlen(space);
        space[length] = ' ';
        space[length+1] = '\0';
        for (int i=0; i<variables.first_count_hasla; i++) {
                for(int j=0;j<variables.first_count_slownik-1 && !flag;j++) {
                        for(int h=0;h<variables.first_count_slownik-1;h++) {
                                char *result=(char*)malloc(100*sizeof(char));
                                pthread_mutex_lock(&variables.count_mutex);
                                strcat(result,variables.slownik[j]);
                                strcat(result,space);
                                strcat(result,variables.slownik[h]);
                                strcpy(tmp_slownik,result);
                                pthread_mutex_unlock(&variables.count_mutex);
                                tmp_slownik[0] = toupper(tmp_slownik[0]);
                                free(result);
                                //printf("%s\n",tmp_slownik);
                                bytes2md5(tmp_slownik, strlen(tmp_slownik), md5);
                                if(variables.check_producent3) {
                                        pthread_mutex_lock(&variables.count_mutex);
                                        variables.check_producent3 = false;
                                        pthread_mutex_unlock(&variables.count_mutex);
                                        break;
                                }
                                if(variables.check_producent5) {
                                        pthread_mutex_lock(&variables.count_mutex);
                                        variables.check_producent5 = false;
                                        pthread_mutex_unlock(&variables.count_mutex);
                                        break;
                                }
                                if(check(i,md5,tmp_slownik)==1) {
                                        pthread_mutex_lock(&variables.count_mutex);
                                        variables.check_producent4 = true;
                                        pthread_mutex_unlock(&variables.count_mutex);
                                        flag = true;
                                        break;
                        }
                        }
                        }
                }
                flag = false;
        pthread_exit(NULL);
}
void *producent5() {
        char tmp_slownik[100];
        bool flag = false;
        char md5[33];
        char space[3];
        int m=0;
        size_t length = strlen(space);
        space[length] = ' ';
        space[length+1] = '\0';
        for (int i=0; i<variables.first_count_hasla; i++) {
                for(int j=0;j<variables.first_count_slownik-1 && !flag;j++) {
                        for(int h=0;h<variables.first_count_slownik-1;h++) {
                                char *result=(char*)malloc(100*sizeof(char));
                                pthread_mutex_lock(&variables.count_mutex);
                                strcat(result,variables.slownik[j]);
                                strcat(result,space);
                                strcat(result,variables.slownik[h]);
                                strcpy(tmp_slownik,result);
                                pthread_mutex_unlock(&variables.count_mutex);
                                while(tmp_slownik[m]) {
                                tmp_slownik[m] = toupper(tmp_slownik[m]);
                                m++;
                        }
                                m=0;
                                free(result);
                                //printf("%s\n",tmp_slownik);
                                bytes2md5(tmp_slownik, strlen(tmp_slownik), md5);
                                if(variables.check_producent3) {
                                        pthread_mutex_lock(&variables.count_mutex);
                                        variables.check_producent3 = false;
                                        pthread_mutex_unlock(&variables.count_mutex);
                                        break;
                                }
                                if(variables.check_producent4) {
                                        pthread_mutex_lock(&variables.count_mutex);
                                        variables.check_producent4 = false;
                                        pthread_mutex_unlock(&variables.count_mutex);
                                        break;
                                }
                                if(check(i,md5,tmp_slownik)==1) {
                                        pthread_mutex_lock(&variables.count_mutex);
                                        variables.check_producent5 = true;
                                        pthread_mutex_unlock(&variables.count_mutex);
                                        flag = true;
                                        break;
                        }
                        }
                }
                flag = false;
        }

        pthread_exit(NULL);
}

void trap_signal (int sig) {
        for(int i=0; i<variables.number-1;i++) {
                pthread_mutex_lock(&variables.count_mutex);
                printf("Haslo # dla %s to  %s\n",variables.tmp_email[i],variables.tmp[i]);
                pthread_mutex_unlock(&variables.count_mutex);
        }
}

void *konsument () {
        signal(SIGHUP,trap_signal);
        while (1) {
        if (variables.flaga) {
        pthread_mutex_lock(&variables.count_mutex);
        printf("Zlamano hasel: %d\n",variables.number);
        //printf("Haslo dla %s to  %s\n\n",variables.tmp_email,variables.tmp);
        variables.number++;
        variables.flaga = false;
        pthread_mutex_unlock(&variables.count_mutex);
        }
        }
        pthread_exit(NULL);
}
int main() {
        variables.k = 0;
        variables.flaga =false;
        variables.number =1;
        variables.first_count_slownik =1;
        variables.first_count_hasla =1;
        variables.check_producent0 = false;
        variables.check_producent1 = false;
        variables.check_producent2 = false;
        variables.check_producent3 = false;
        variables.check_producent4 = false;
        variables.check_producent5 = false;

        strcpy(variables.plik,"Hasla.txt");
        pthread_t threads[6];
        pthread_t consume_thread;
        pthread_mutex_init(&variables.count_mutex, NULL);

        read_hasla();
        read_slownik();
        pthread_create(&threads[0],NULL,producent0,NULL);
        pthread_create(&threads[1],NULL,producent1,NULL);
        pthread_create(&threads[2],NULL,producent2,NULL);
        pthread_create(&threads[3],NULL,producent3,NULL);
        pthread_create(&threads[4],NULL,producent4,NULL);
        pthread_create(&threads[5],NULL,producent5,NULL);
        pthread_create(&consume_thread,NULL,konsument,NULL);
        newfile();

        // newfile();

        // pthread_cancel(threads[0]);
        // pthread_cancel(threads[1]);
        // pthread_cancel(threads[2]);
        // pthread_cancel(threads[3]);
        // pthread_cancel(threads[4]);
        // pthread_cancel(threads[5]);
        // pthread_cancel(consume_thread);

        // read_hasla();

        // pthread_create(&threads[0],NULL,producent0,NULL);
        // pthread_create(&threads[1],NULL,producent1,NULL);
        // pthread_create(&threads[2],NULL,producent2,NULL);
        // pthread_create(&threads[3],NULL,producent3,NULL);
        // pthread_create(&threads[4],NULL,producent4,NULL);
        // pthread_create(&threads[5],NULL,producent5,NULL);
        // pthread_create(&consume_thread,NULL,konsument,NULL);

        pthread_join(consume_thread,NULL);


        pthread_mutex_destroy(&variables.count_mutex);
        pthread_exit(NULL);
        for(int j=0; j<variables.first_count_slownik-1;j++) {
                free(variables.slownik[j]);
                free(variables.tmp[j]);
        }
        free(variables.slownik);
        free(variables.tmp);

    return 0;
}
