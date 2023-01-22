#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <openssl/evp.h>
#include <signal.h>
#include <ctype.h>

#define NUM_PASSWD 1000
#define LENGTH 33
#define WORD_FILENAME "Slownik.txt"
#define NUM_THREADS 6

int IleSlow=0;
char **Slowa;
int IleHaselZnal=0;
int ZnalHasloID;
int BazaRestart=0;

pthread_mutex_t FindedPassword_mutex;
pthread_cond_t FindedPassword_cond;
pthread_barrier_t Restart_barrier;
pthread_barrier_t Start_barrier;
pthread_mutex_t Restart_mutex;



struct Haslo_baza{
	int id;
	char Haslo[LENGTH];
	char Mail[LENGTH];
	char Name[LENGTH];
	char RealHaslo[LENGTH];
	int Spr;
} Baza[NUM_PASSWD];


void bytes2md5(const char *data, int len, char *md5buf){
	EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
	const EVP_MD *md = EVP_md5();
	unsigned char md_value[EVP_MAX_MD_SIZE];
	unsigned int md_len, i;
	EVP_DigestInit_ex(mdctx, md, NULL);
	EVP_DigestUpdate(mdctx, data, len);
	EVP_DigestFinal_ex(mdctx, md_value, &md_len);
	EVP_MD_CTX_free(mdctx);
	for (i=0; i<md_len; i++) {
		snprintf(&(md5buf[i*2]), 16 * 2, "%02x", md_value[i]);
	}
}

void WczytajHasla (char * Filename){
	FILE *passwordFile;
	passwordFile = fopen(Filename, "rt");
	if(passwordFile!=NULL){
		for(int i=0; i<NUM_PASSWD; i++){
			fscanf(passwordFile, "%d %s %s %[^\n]s", &(Baza[i].id), Baza[i].Haslo, Baza[i].Mail, Baza[i].Name);
			Baza[i].Spr=0;
		}
	}
	else{
		printf("Plik: %s nie zostal otwarty\n", Filename);
	}

	fclose(passwordFile);
}

void CzyscBaze(){
	for(int i=0; i<NUM_PASSWD; i++){
	Baza[i].id=0;
	Baza[i].Spr=0;
	strcpy(Baza[i].Haslo, "");
	strcpy(Baza[i].Mail, "");
	strcpy(Baza[i].Name, "");
	strcpy(Baza[i].RealHaslo, "");
	}
}

void WczytajSlowa(){
	FILE *wordFile;
	char bin;

	wordFile=fopen(WORD_FILENAME, "rt");
	//ILE JEST SLOW W SLOWNIKU
	for(bin=getc(wordFile); bin!=EOF; bin=getc(wordFile)){
		if(bin=='\n'){
		++IleSlow;
		}
	}
	rewind(wordFile);

	Slowa=malloc(IleSlow*sizeof(char*));
	for(int i=0; i<IleSlow; i++){
		Slowa[i]=malloc(LENGTH * sizeof(char));
		fscanf(wordFile,"%s",Slowa[i]);
	}

	fclose(wordFile);
}

int SprawdzHaslo(char* Haslo1, char* Haslo2){
	long int Dlugosc = strlen(Haslo1);
	char md5Haslo[32];
	bytes2md5(Haslo1, Dlugosc, md5Haslo);
	if(strcmp(md5Haslo, Haslo2) == 0){
		return 1;
	}
	return 0;
}


void GenerujNumerString(char* String, int Numer, int Dlugosc){
	int i;
	int NumDlugosc =1;
	int NumerCopy = Numer;
	char NumerString[Dlugosc];
	if(Numer == -1){
		strcpy(String,"");
	}
	else{
		sprintf(NumerString, "%d", Numer);
		strcpy(String,"");
		while(NumerCopy>=10){
			++NumDlugosc;
			NumerCopy=NumerCopy/10;
		}
		for(i=0; i<Dlugosc-NumDlugosc; i++){
			String[i]='0';
		}

		for(i; i<Dlugosc; i++){
			String[i]=NumerString[i-(Dlugosc-NumDlugosc)];
		}
		String[i]='\0';
	}
}

void* JednoSlowoProd(void* arg){
	long ProdNumer= (long)arg;
	int i,j,k;
	int bufSize=10, PrefixSize=5, PostfixSize=5;
	char* buf = (char*) malloc(bufSize*sizeof(char));
	char* Prefix = (char*) malloc(PrefixSize*sizeof(char));
	char* Postfix = (char*) malloc(PostfixSize*sizeof(char));
	char* Word = (char*) malloc(LENGTH*sizeof(char));
	int PrefixNum=-1, PostfixNum=-1;
	int Limit=10;
	int Length=1;
	strcpy(Prefix,"");
	strcpy(Postfix,"");

	while(1){
		for(i=0; i<IleSlow; i++){
			//Tworzenie potencjalnego hasla
			if(bufSize>=(strlen(Slowa[i])+PostfixSize+PrefixSize)){
				buf=realloc(buf,(strlen(Slowa[i])+PostfixSize+PrefixSize+1));
			}
			strcpy(Word,Slowa[i]);
			switch(ProdNumer){
				case 1:
				Word[0]=(char)toupper(Word[0]);
				break;
				case 2:
					for(j=0; j<strlen(Word); j++){
						Word[j]=(char)toupper(Word[j]);
					}
				break;
			}
			strcpy(buf,"");
            strcat(buf,Prefix);
            strcat(buf,Word);
            strcat(buf,Postfix);
			if(BazaRestart==0){
				for(k=0; k<NUM_PASSWD;k++){
					if((Baza[k].Spr)!=1){
						if(SprawdzHaslo(buf,Baza[k].Haslo)){
							//Znaleziono haslo
							pthread_mutex_lock(&FindedPassword_mutex);
							strcpy(Baza[k].RealHaslo,buf);
							Baza[k].Spr=1;
							++IleHaselZnal;
							ZnalHasloID=k;
							pthread_cond_signal(&FindedPassword_cond);
							pthread_mutex_unlock(&FindedPassword_mutex);
						}
					}
				}
			}
		}
		//Polaczenie pre i post-fix
		if(Length>=PostfixSize){
            ++PostfixSize;
            ++PrefixSize;
            Postfix = realloc(Postfix,PostfixSize*sizeof(char));
            Prefix = realloc(Prefix,PrefixSize*sizeof(char));
        }

        GenerujNumerString(Prefix,PrefixNum,Length);
        GenerujNumerString(Postfix,PostfixNum,Length);
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

		if(BazaRestart!=0){
         pthread_barrier_wait(&Restart_barrier);
         pthread_mutex_lock(&Restart_mutex);
         --BazaRestart;
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

void* DwaSlowaProd(void *arg){
	long ProdNumber= (long)arg;
	int i,j,k,l;
	char* Word1 = (char*) malloc(LENGTH*sizeof(char));
	char* Word2 = (char*) malloc(LENGTH*sizeof(char));
	char* buf = (char*) malloc(LENGTH*sizeof(char));
	int Word1Length, Word2Length;

	for(i=0;i<IleSlow;i++){
        strcpy(Word1, Slowa[i]);

        for(j=0;j<IleSlow;j++){
            strcpy(Word2, Slowa[j]);

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

			if(BazaRestart==0){
            for(l=0;l<NUM_PASSWD;l++){
                if((Baza[l].Spr)!=1){
                    if(SprawdzHaslo(buf,Baza[l].Haslo)){
                        //Haslo znalezione
                        pthread_mutex_lock(&FindedPassword_mutex);
                        strcpy(Baza[l].RealHaslo,buf);
                        Baza[l].Spr=1;
                        ZnalHasloID=k;
                        ++IleHaselZnal;
                        pthread_cond_signal(&FindedPassword_cond);
                        pthread_mutex_unlock(&FindedPassword_mutex);
                    }
                }
            }
            }

        }

		if(BazaRestart!=0){
            pthread_barrier_wait(&Restart_barrier);
            pthread_mutex_lock(&Restart_mutex);
            --BazaRestart;
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

void* Konsument(void *arg){
    while(1){

        pthread_mutex_lock(&FindedPassword_mutex);
        pthread_cond_wait(&FindedPassword_cond, &FindedPassword_mutex);
        printf("Haslo dla: %s to %s\n", Baza[ZnalHasloID].Mail, Baza[ZnalHasloID].RealHaslo);
        pthread_mutex_unlock(&FindedPassword_mutex);
    }
}

void SumUp(){
    pthread_mutex_lock(&FindedPassword_mutex);
    printf("Ilosc znalezionych hasel: %d\n",IleHaselZnal);
    pthread_mutex_unlock(&FindedPassword_mutex);
}


int main(){

	signal(SIGHUP, SumUp);

    WczytajHasla("Hasla.txt");
    WczytajSlowa();
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

    pthread_create(&threads[0], &attr, JednoSlowoProd, (void *)0);
    pthread_create(&threads[1], &attr, JednoSlowoProd, (void *)1);
    pthread_create(&threads[2], &attr, JednoSlowoProd, (void *)2);
    pthread_create(&threads[3], &attr, DwaSlowaProd, (void *)0);
    pthread_create(&threads[4], &attr, DwaSlowaProd, (void *)1);
    pthread_create(&threads[5], &attr, DwaSlowaProd, (void *)2);
    pthread_create(&threads[6], &attr, Konsument, NULL);
    // pthread_create(&threads[1], &attr, Konsument, NULL);
    // pthread_create(&threads[2], &attr, Konsument, NULL);


	while(strcmp(Filename,"exit")){
    scanf("%s",Filename);
    if(strcmp(Filename,"exit")){
        pthread_mutex_lock(&Restart_mutex);
        BazaRestart=NUM_THREADS-1;
        pthread_mutex_unlock(&Restart_mutex);
        printf("Restartowanie haseł\n");
        pthread_barrier_wait(&Restart_barrier);
        CzyscBaze();
        WczytajHasla(Filename);
        pthread_barrier_wait(&Start_barrier);
        printf("Koniec restartowania haseł\n");
    }
    }

	for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&FindedPassword_mutex);
    pthread_cond_destroy(&FindedPassword_cond);
    pthread_barrier_destroy(&Restart_barrier);
    pthread_barrier_destroy(&Start_barrier);
    pthread_mutex_destroy(&Restart_mutex);
    free(Slowa);
    pthread_exit(NULL);
}

