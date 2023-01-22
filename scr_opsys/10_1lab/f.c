#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <openssl/evp.h>
#include <signal.h>
#include <ctype.h>

#define NUM_PASSWORDS 1000
#define LENGTH 33
#define WORD_FILENAME "Slownik.txt"
#define NUM_THREADS 6

pthread_mutex_t FindedPassword_mutex;
pthread_cond_t  FindedPassword_cond;
pthread_barrier_t Restart_barrier;
pthread_barrier_t Start_barrier;
pthread_mutex_t Restart_mutex;

int NumOfWords=0;
char **Words;
int NumOfFindedPasswords=0;
int FindedPasswordId;
int BaseRestart=0;

struct Passwords_base{
    int id;
    char Password[LENGTH];
    char Mail[LENGTH];
    char Name[LENGTH];
    char RealPassword[LENGTH];
    int Checked;
} Base[NUM_PASSWORDS];

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

void ClearBase(){
    for(int i=0;i<NUM_PASSWORDS;i++){
        Base[i].id=0;
        Base[i].Checked=0;
        strcpy(Base[i].Password,"");
        strcpy(Base[i].Mail,"");
        strcpy(Base[i].Name,"");
        strcpy(Base[i].RealPassword,"");
    }
}

void DownloadPasswords(char *Filename){
    FILE *passwordFile;
    passwordFile=fopen(Filename, "rt");
    if(passwordFile!=NULL){
        for(int i=0; i<NUM_PASSWORDS; i++){
            fscanf(passwordFile, "%d  %s  %s  %[^\n]s",&(Base[i].id),Base[i].Password,Base[i].Mail,Base[i].Name);
            Base[i].Checked=0;
        }
    }
    else{
        printf("Plik: %s nie został otwarty\n",Filename);
    }
    
    fclose(passwordFile);
}



void DownloadWords(){
    FILE *wordFile;
    char bin;

    wordFile=fopen(WORD_FILENAME, "rt");
    
    //Znalezienie liczby słów w słowniku
    for(bin=getc(wordFile); bin!=EOF; bin=getc(wordFile)){
        if(bin=='\n'){
            ++NumOfWords;
        }
    }
    rewind(wordFile);

    Words=malloc(NumOfWords*sizeof(char*));
    for(int i=0; i<NumOfWords; i++){
        Words[i]=malloc(LENGTH*sizeof(char));
        fscanf(wordFile,"%s",Words[i]);
    }

    fclose(wordFile);
}

int CheckPassword(char* Password1, char* Password2){

    long int Length=strlen(Password1);
    char md5Password[32];
    bytes2md5(Password1, Length, md5Password);
    if(strcmp(md5Password,Password2)==0){
        return 1;
    }
    return 0;
}

void GenerateNumberString(char *String, int Number, int Length){
    int i;
    int NumLength=1;
    int NumberCopy=Number;
    char NumberString[Length];
    if(Number==-1){
        strcpy(String,"");
    }
    else {
        sprintf(NumberString,"%d",Number);
        strcpy(String,"");
        while(NumberCopy>=10){
            ++NumLength;
            NumberCopy=NumberCopy/10;
        }
        for(i=0;i<Length-NumLength; i++){
            String[i]='0';
        }
    
        for(i; i<Length;i++){
            String[i]=NumberString[i-(Length-NumLength)];
        }
        String[i]='\0';
    }

}

void* OneWordProd(void *arg){
  long ProdNumber= (long)arg;
    int i,j,k;
    int bufSize=10, PrefixSize=5, PostfixSize=5;
    char* buf = (char*) malloc(bufSize*sizeof(char));
    char* Prefix = (char*) malloc(PrefixSize*sizeof(char));
    char* Postfix = (char*) malloc(PostfixSize*sizeof(char));
    char* Word = (char*) malloc(LENGTH*sizeof(char));
    int  PrefixNum=-1, PostfixNum=-1;
    int  Limit=10;
    int  Length=1;
    strcpy(Prefix,"");
    strcpy(Postfix,"");
    
    
    while(1){
        for(i=0; i<NumOfWords; i++){
            //Tworzenie potencjalnego hasla
            if(bufSize>=(strlen(Words[i])+PostfixSize+PrefixSize)){
                buf=realloc(buf,(strlen(Words[i])+PostfixSize+PrefixSize+1));
            }
            strcpy(Word,Words[i]);
            switch (ProdNumber){
                case 1:
		  Word[0]=(char)toupper(Word[0]);

                break;
                case 2:
                for(j=0;j<strlen(Word);j++){
		  Word[j]=(char)toupper(Word[j]);
                }
                break;
            }
            strcpy(buf,"");
            strcat(buf,Prefix);
            strcat(buf,Word);
            strcat(buf,Postfix);
            if(BaseRestart==0){
            for(k=0;k<NUM_PASSWORDS;k++){
                if((Base[k].Checked)!=1){
                    if(CheckPassword(buf,Base[k].Password)){
                        //Haslo znalezione
                        pthread_mutex_lock(&FindedPassword_mutex);
                        strcpy(Base[k].RealPassword,buf);
                        Base[k].Checked=1;
                        ++NumOfFindedPasswords;
                        FindedPasswordId=k;
                        pthread_cond_signal(&FindedPassword_cond);
                        pthread_mutex_unlock(&FindedPassword_mutex);
                    }
                }
            }
            }
        }
        
        
        //POLACZONY PREFIX I POSTFIX
        if(Length>=PostfixSize){
            ++PostfixSize;
            ++PrefixSize;
            Postfix = realloc(Postfix,PostfixSize*sizeof(char));
            Prefix = realloc(Prefix,PrefixSize*sizeof(char));
        }

        GenerateNumberString(Prefix,PrefixNum,Length);
        GenerateNumberString(Postfix,PostfixNum,Length);
        ++PostfixNum;
        if(PostfixNum==Limit){
            PostfixNum=-1;
            ++PrefixNum;
        }
        if(PrefixNum==Limit){
            PrefixNum=-1;
            PostfixNum=-1;
            ++Length;
            Limit=Limit*10;
        }
        
        if(BaseRestart!=0){
         pthread_barrier_wait(&Restart_barrier);
         pthread_mutex_lock(&Restart_mutex);
         --BaseRestart;
         pthread_mutex_unlock(&Restart_mutex);
         //Restartowanie wątku
         i=j=k=0;
         bufSize=10;
         PrefixSize=5; 
         PostfixSize=5;
         strcpy(Prefix,"");
         strcpy(Postfix,"");
         buf = realloc(buf,bufSize*sizeof(char));
         Prefix = realloc(Prefix,PrefixSize*sizeof(char));
         Postfix = realloc(Postfix,PostfixSize*sizeof(char));
         Word = realloc(Word,LENGTH*sizeof(char));
         PrefixNum=-1;
         PostfixNum=-1;
         Limit=10;
         Length=1;
         pthread_barrier_wait(&Start_barrier);
        }
        
    }
}

void* TwoWordsProd(void *arg){
  long ProdNumber= (long)arg;
int i,j,k,l;
char* Word1 = (char*) malloc(LENGTH*sizeof(char));
char* Word2 = (char*) malloc(LENGTH*sizeof(char));
char* buf = (char*) malloc(LENGTH*sizeof(char));
int Word1Length, Word2Length;

    for(i=0;i<NumOfWords;i++){
        strcpy(Word1, Words[i]);

        for(j=0;j<NumOfWords;j++){
            strcpy(Word2, Words[j]);

            switch(ProdNumber){
                //Małe litery. Spacja pomiedzy słowami
                case 0:
                    strcpy(buf,"");
                    strcat(buf,Word1);
                    strcat(buf," ");
                    strcat(buf,Word2);
                    break;
                //Pierwsza duza litera w obu słowach, spacja pomiedzy
                case 1:
		             Word1[0]=(char)toupper(Word1[0]);
		            Word2[0]=(char)toupper(Word2[0]);
                    strcpy(buf,"");
                    strcat(buf,Word1);
                        strcat(buf," ");
                    strcat(buf,Word2);
                    break;
                //Duze litery. Podkreślnik pomiedzy słowami
                case 2:
                    Word1Length=strlen(Word1);
                    for(k=0;k<Word1Length;k++){
                        Word1[k]=(char)toupper(Word1[k]);
                    }
                    Word2Length=strlen(Word2);
                    for(k=0;k<Word2Length;k++){
                            Word2[k]=(char)toupper(Word2[k]);
                    }
                    strcpy(buf,"");
                    strcat(buf,Word1);
                    strcat(buf,"_");
                    strcat(buf,Word2);
                    break;  
            }
            if(BaseRestart==0){
            for(l=0;l<NUM_PASSWORDS;l++){
                if((Base[l].Checked)!=1){
                    if(CheckPassword(buf,Base[l].Password)){
                        //Haslo znalezione
                        pthread_mutex_lock(&FindedPassword_mutex);
                        strcpy(Base[l].RealPassword,buf);
                        Base[l].Checked=1;
                        FindedPasswordId=k;
                        ++NumOfFindedPasswords;
                        pthread_cond_signal(&FindedPassword_cond);
                        pthread_mutex_unlock(&FindedPassword_mutex);
                    }
                }
            }
            }

        }
        
         if(BaseRestart!=0){
            pthread_barrier_wait(&Restart_barrier);
            pthread_mutex_lock(&Restart_mutex);
            --BaseRestart;
            pthread_mutex_unlock(&Restart_mutex);
            //Restartowanie wątku
            int i=j=k=l=0;
            strcpy(Word1,"");
            strcpy(Word2,"");
            strcpy(buf,"");
            pthread_barrier_wait(&Start_barrier);
        }
        
    }
}
    
void* Consument(void *arg){
    while(1){
        
        pthread_mutex_lock(&FindedPassword_mutex);
        pthread_cond_wait(&FindedPassword_cond, &FindedPassword_mutex);
        printf("Password for: %s is %s\n", Base[FindedPasswordId].Mail, Base[FindedPasswordId].RealPassword);
        pthread_mutex_unlock(&FindedPassword_mutex);
    }
}
void SumUp(){
    pthread_mutex_lock(&FindedPassword_mutex);
    printf("Number of finded passwords: %d\n",NumOfFindedPasswords);
    pthread_mutex_unlock(&FindedPassword_mutex);
}

int main(){
    
    signal(SIGHUP, SumUp);

    DownloadPasswords("Hasla.txt");
    DownloadWords();
    char Filename[LENGTH];
    strcpy(Filename,"");

    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr;

    pthread_mutex_init(&FindedPassword_mutex, NULL);
    pthread_cond_init(&FindedPassword_cond, NULL);
    pthread_barrier_init(&Restart_barrier, NULL, NUM_THREADS);
    pthread_barrier_init(&Start_barrier, NULL, NUM_THREADS);
    pthread_mutex_init(&Restart_mutex, NULL);
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    pthread_create(&threads[0], &attr, OneWordProd, (void *)0);
    pthread_create(&threads[1], &attr, OneWordProd, (void *)1);
    pthread_create(&threads[2], &attr, OneWordProd, (void *)2);
    pthread_create(&threads[3], &attr, TwoWordsProd, (void *)0);
    pthread_create(&threads[4], &attr, TwoWordsProd, (void *)1);
    pthread_create(&threads[5], &attr, TwoWordsProd, (void *)2);
    pthread_create(&threads[6], &attr, Consument, NULL);
    
    
    while(strcmp(Filename,"exit")){
    scanf("%s",Filename);
    if(strcmp(Filename,"exit")){
        pthread_mutex_lock(&Restart_mutex);
        BaseRestart=NUM_THREADS-1;
        pthread_mutex_unlock(&Restart_mutex);
        printf("Restartowanie haseł\n");
        pthread_barrier_wait(&Restart_barrier);
        ClearBase();
        DownloadPasswords(Filename);
        pthread_barrier_wait(&Start_barrier);
        printf("Koniec restartowania haseł\n");
    }
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&FindedPassword_mutex);
    pthread_cond_destroy(&FindedPassword_cond);
    pthread_barrier_destroy(&Restart_barrier);
    pthread_barrier_destroy(&Start_barrier);
    pthread_mutex_destroy(&Restart_mutex);
    free(Words);
    pthread_exit(NULL);
    }