#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//extrage urmatorul cuvant din set, modifica set
char *extrage(char *set, char *cuvant, int *exclude)
{
    int minus=0;
    int secventa=0;
    char *cuv;
    int x=0;
    //daca s-a terminat sirul, returnez NULL
    if(set[0]=='\0' || (set[1]=='\0' && set[0]=='"')) return NULL;
    if(set[0]=='-') {
        minus++;
        if(set[1]=='"') {
            secventa++;
        }
    }
    if(set[0]=='"') {
        secventa++;
     }
    if(!secventa) {
        for(int i=0; set[i]!='\0'; i++) {
            if(set[i]!=' ') x++;//cresc numarul de caractere ale cuvantului    
            else break;   
         }
    }
    else {
        x+=minus+secventa;
        for(int i=minus+secventa; set[i]!='\0'; i++) {
            if(set[i]!='"') x++;//cresc numarul de caractere ale cuvantului
            else break;//ma opresc atunci cand gasesc sfarsitul secventei
        }
    }
    //creez cuvantul
    cuv = malloc(x+1);
    strncpy(cuv,set,x);
    cuv[x]='\0';
    //in functie de caracteristici, modific cuvant
    strcpy(cuvant, cuv+minus+secventa);
    cuvant[x-minus-secventa]='\0';
    //scot din set restul spatiilor libere
    //scot si ultimele ghilimele
    if(secventa) x++;
    while(set[x]==' ') x++;
    strcpy(set, set+x);
    free(cuv);
    *exclude = minus;
    return set;
}

int main()
{
    char *linie;
    int exclude;
    linie = malloc(100);
    fgets(linie, 100, stdin);
    char cuvant[50];
    int x = strlen(linie);
    linie[x-1] = '\0';
    while(extrage(linie, cuvant, &exclude)!=NULL)
    {
        printf("%s %d\n",cuvant, exclude);
    }
    return 0;
}