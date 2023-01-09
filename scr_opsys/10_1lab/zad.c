#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <openssl/evp.h>
#include <signal.h>

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
	long int Dlugosc = strlen(Haslo2);
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
			String='0';
		}

		for(i; i<Dlugosc; i++){
			String[i]=NumerString[i-Numdlugosc)};
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
	char* Word = (char*) malloc(LENGTH*siezeof(char));
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
		}

	}


int main(){

char* md5test;
WczytajSlowa();
int dlugosctest=strlen(Slowa[1]);
bytes2md5(Slowa[1], dlugosctest, md5test);

printf("Slowo: %s\n", Slowa[1]);
printf("MD5: %s\n", md5test);



return 0;
}