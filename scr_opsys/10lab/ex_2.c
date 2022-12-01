#include <openssl/evp.h>
#include <stdio.h>
#include <string.h>

// Należy użyć flag -lssl -lcrypto
// gcc ex_2.c -o ex_2 -lssl -lcrypto


#define HASLA "test.txt"
#define HASZ "hasla_2.txt"



typedef struct
{
    char **tablica;
    int lon;

} STRUKT;

STRUKT dane;

int czytaj(){

    FILE *f;
    int l = 0;
    int chr;
    char **tab;
    int j = 0;
    int licznik = 0;
    int c = 0;

    f = fopen(HASLA,"r");
    if(f){
        while((chr = getc(f)) != EOF){
            if(chr == '\n'){
               l++;
            }
        }
        fclose(f);
    }
    dane.lon = l;

    printf("czytaj() - %d\n",l);

    tab = malloc(l*sizeof(char*));
    for(int i = 0; i < l; i++){
        tab[i] = malloc((l + 1)*sizeof(char));
    }

    f = fopen(HASLA,"r");

    if(f){
        while((c = getc(f)) != EOF){
            tab[licznik][j] = c;
            j++;
            if(c == '\n'){
                licznik++;
                j = 0;
            }
        }
        fclose(f);
    }
    dane.tablica = tab;

    return l;
}


void bytes2md5(const char *data, int len, char *md5buf) {
  // Based on https://www.openssl.org/docs/manmaster/man3/EVP_DigestUpdate.html
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



int main(void) {

  FILE *fw;
  char md5[33]; // 32 characters + null terminator
  int wiersz = czytaj();


  fw = fopen(HASZ,"w");
    if(fw){
        for(int j=0;j<wiersz;j++){
            bytes2md5(dane.tablica[j], strlen(dane.tablica[j]), md5);
            fprintf(fw,"%s\n", md5);
            printf("%s\n",md5);

        }
        fclose(fw);
    }


}