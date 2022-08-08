#include "siteuri.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void swap(site *v1, site *v2)
{
    char aux[200];
    strcpy(aux,v1->URL);
    strcpy(v1->URL,v2->URL);
    strcpy(v2->URL,aux);
    strcpy(aux,v1->titlu);
    strcpy(v1->titlu,v2->titlu);
    strcpy(v2->titlu,aux);
    char *continutaux;
    int x1 = strlen(v1->continut);
    int x2 = strlen(v2->continut);
    continutaux = calloc(x1+1,1);
    strcpy(continutaux, v1->continut);
    v1->continut = realloc(v1->continut,x2+1);
    strcpy(v1->continut,v2->continut);
    v2->continut = realloc(v2->continut,x1+1);
    strcpy(v2->continut,continutaux);
    free(continutaux);
    char *paragrafaux;
    x1 = strlen(v1->paragraf);
    x2 = strlen(v2->paragraf);
    paragrafaux = calloc(x1+1,1);
    strcpy(paragrafaux, v1->paragraf);
    v1->paragraf = realloc(v1->paragraf,x2+1);
    strcpy(v1->paragraf,v2->paragraf);
    v2->paragraf = realloc(v2->paragraf,x1+1);
    strcpy(v2->paragraf,paragrafaux);
    free(paragrafaux);
    int aux1;
    aux1 = v1->accesari;
    v1->accesari = v2 -> accesari;
    v2->accesari = aux1;
    unsigned char aux2;
    aux2 = v1->checksum;
    v1->checksum = v2 -> checksum;
    v2->checksum = aux2;
    return;
}

void init(vsite *v)
{
    (v->cap) = 3;
    (v->v) = malloc((v->cap)* sizeof(site));
    return;
}

void realoc(vsite *v)
{
    (v->cap) = (v->cap)+3;
    (v->v) = realloc((v->v), (v->cap)*sizeof(site));
    return;
}

//functie ce extrage un cuvant, iar in exclude imi indica daca acesta
//avea - in fata sau nu; el modifica permanent set si la final returneaza
//NULL, dupa ce a extras toate cuvintele/secventele din set
char* extrage(char *set, char *cuvant, int *exclude)
{
    int minus=0;
    int secventa=0;
    char *cuv;
    int x=0;
    //daca s-a terminat sirul, returnez NULL
    if(set[0]=='\0' || (set[1]=='\0' && set[0]=='"') || set==NULL) return NULL;
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
    memmove(set, set+x, strlen(set)-x+1);
    free(cuv);
    *exclude = minus;
    return set;
}

//functie care imi arata daca un pointer pointeaza catre o litera

int pointeaza(char *point)
{
    if(*point >=97 &&*point <=122) return 1;
    if(*point >=65 &&*point <=90) return 1;
    return 0;
}


int contine(char *s, char *lista)
{
    //schimb \n in terminator de sir
    //stiu ca lista va fi citita pana la \n
    int x = strlen(lista);
    lista[x-1] = '\0';
    int exclude; //retine daca o secventa/un cuvant e exclus
    int contine1 = 0;//retine daca s contine macar un cuvant din cele ne-excluse
    char cuvant[100];
    while(extrage(lista, cuvant, &exclude)!=NULL)
    {
        //un cuvant nu va avea dupa el o litera, va avea altceva(si inaintea lui la fel)
        char * pointer = strstr(s, cuvant);
        int alfa = strlen(cuvant);
        //printf("%s %s %d\n", s, cuvant, exclude);
        if(exclude && (strstr(s, cuvant)!=NULL)) {return 0;}
        if((!exclude) && (strstr(s, cuvant)!=NULL) && !pointeaza(pointer-1) && !pointeaza(pointer + alfa ) ) {contine1=1;}
    }
    if(contine1) return 1;
    return 0;
}

void extragetitlu(char *continut, char *string)
{
    int x = strlen(continut);
    char *ccontinut;
    //creez o copie a continutului
    ccontinut = malloc(x+1);
    strcpy(ccontinut, continut);
    //pointeri catre inceputul celor doua taguri
    char *ptr1 = strstr(ccontinut, "<title>");
    //pointer nou pe care nu il stric, pointeaza la inceputul titlului
    char *ptr3 = ptr1 + 7;
    ptr1 = ptr3;
    char *ptr2 = strstr(ccontinut, "</title>");
    //reinitializez x cu 0, si voi retine lungimea titlului in el
    x = 0;
    //mut pozitia lui ptr1 pana dau de ptr2 pentru a afla lungimea
    while(ptr1 != ptr2)
    {
        x++;
        ptr1 = ptr1 + 1;
    }
    //copiez in string de la ptr3, x caractere
    strncpy(string,ptr3,x);
    //adaug manual terminator de sir
    string[x]='\0';
    //free nu e necesar
    free(ccontinut);
    return;
}

void extrageparagraf(char *continut, char *string)
{
    int x = strlen(continut);
    char *ccontinut;
    //creez o copie a continutului
    ccontinut = malloc(x+1);
    strcpy(ccontinut, continut);
    //caut <p
    char *ptr1 = strstr(ccontinut, "<p");
    //pointer nou pe care nu il stric, pointeaza la inceputul paragrafului
    char *ptr3 = strchr(ptr1, '>');
    ptr3 ++;
    ptr1 = ptr3;
    char *ptr2 = strstr(ccontinut, "</p>");
    //reinitializez x cu 0, si voi retine lungimea paragrafului in el
    x = 0;
    //mut pozitia lui ptr1 pana dau de ptr2 pentru a afla lungimea
    while(ptr1 != ptr2)
    {
        x++;
        ptr1 = ptr1 + 1;
    }
    //copiez in string de la ptr3, x caractere
    strncpy(string,ptr3,x);
    //adaug manual terminator de sir
    string[x]='\0';
    //free nu e necesar
    free(ccontinut);
    return;
}

void incarca(vsite *v)
{   
    FILE *master;
    master = fopen("master.txt", "r");
    init(v);
    char numefisier[30];
    v->indice=0;
    while(fgets(numefisier, 30, master))
    {
        if((v->indice) == (v->cap)) {
            realoc(v);
        }
        int x = strlen(numefisier);
        numefisier[x-1] = '\0';
        FILE *fisier;
        fisier = fopen(numefisier, "r");
        char linie[100];
        fgets(linie, 100, fisier);
        char *p;
        p = strtok (linie," ");
        strcpy((v->v)[(v->indice)].URL, p);
        p = strtok (NULL, " ");
        int lg = atoi(p);
        (v->v)[(v->indice)].continut = calloc (lg+3,1);
        (v->v)[(v->indice)].paragraf = calloc (lg+3,1);
        p = strtok (NULL, " ");
        (v->v)[(v->indice)].accesari = atoi(p);
        p = strtok (NULL, " ");
        (v->v)[(v->indice)].checksum = atoi(p);
        strcpy((v->v)[(v->indice)].numesite, numefisier);
        while(fgets(linie, 100, fisier))
        {
            strcat((v->v)[(v->indice)].continut,linie);
        }
        extragetitlu((v->v)[(v->indice)].continut,(v->v)[(v->indice)].titlu);
        extrageparagraf((v->v)[(v->indice)].continut,(v->v)[(v->indice)].paragraf);
        (v->indice)++;
        fclose(fisier);
    }
    fclose(master);
    return ;
}

void incarcaR(vsite *v, char *lista)
{   
    FILE *master;
    master = fopen("master.txt", "r");
    init(v);
    char numefisier[30];
    v->indice=0;
    while(fgets(numefisier, 30, master))
    {
        if((v->indice) == (v->cap)) {
            realoc(v);
        }
        int x = strlen(numefisier);
        numefisier[x-1] = '\0';
        FILE *fisier;
        fisier = fopen(numefisier, "r");
        char linie[100];
        fgets(linie, 100, fisier);
        char *p;
        p = strtok (linie, " ");
        strcpy((v->v)[(v->indice)].URL, p);
        p = strtok (NULL, " ");
        int lg = atoi(p);
        (v->v)[(v->indice)].continut = calloc (lg+3,1);
        (v->v)[(v->indice)].paragraf = calloc (lg+3,1);
        p = strtok (NULL, " ");
        (v->v)[(v->indice)].accesari = atoi(p);
        p = strtok (NULL, " ");
        (v->v)[(v->indice)].checksum = atoi(p);
        while(fgets(linie, 100, fisier))
        {
            strcat((v->v)[(v->indice)].continut,linie);
        }
        extragetitlu((v->v)[(v->indice)].continut,(v->v)[(v->indice)].titlu);
        extrageparagraf((v->v)[(v->indice)].continut,(v->v)[(v->indice)].paragraf);
        //cresc indicele vectorului doar daca in continut gasesc unul din cuvinte
        //astfel, urmatoarul site, in caz ca e bun, va suprascrie siteul curent
        //daca acesta este ultimul site si nu e bun, ramane in memorie, insa
        //nu afecteaza cu nimic programul, intrucat indicele nu creste
        //in cazul in care if-ul da fail, dau free la continut
        //fac si o copie listei
        char *copie = strdup(lista);
        if(contine((v->v)[(v->indice)].paragraf,copie)){
            (v->indice)++;}
            else {free((v->v)[(v->indice)].continut);
                    free((v->v)[(v->indice)].paragraf);}
        fclose(fisier);
        free(copie);
    }
    fclose(master);
    return ;
}

void descarca(vsite *v)
{
    int n = v->indice;
    for(int i=0; i<n; i++){
        free((v->v)[i].continut);
        free((v->v)[i].paragraf);
    }
    free(v->v);
    free(v);
}

void sortare(vsite *v, int (*cmp) (site v1, site v2))
{
    int n = v->indice;
    for(int i=0;i<n;i++)
    {
        for(int j=i+1; j<n; j++)
        {
            if(!cmp((v->v)[i],(v->v)[j])) {
                swap(&(v->v)[i], &(v->v)[j]);
            }
        }
    }
    return;
}