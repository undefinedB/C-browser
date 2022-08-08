#ifndef siteuri
#define siteuri
#include <ncurses.h>
//structura de retinere a unui site in memorie
typedef struct {
    char URL[200];
    char titlu[200];
    char numesite[200];
    int accesari;
    unsigned char checksum;
    char *continut;
    char *paragraf;
} site;
//structura ajutatoare de tip vector(de siteuri)
typedef struct{
    site *v;
    int cap;//capacitatea maxima
    int indice;//indicele maxim(nr de siteuri incarcare efectiv)
} vsite;
//enumerarile pentru culori
typedef enum{
    white = COLOR_WHITE,
    black = COLOR_BLACK,
    red = COLOR_RED,
    green = COLOR_GREEN,
    blue = COLOR_BLUE,
    yellow = COLOR_YELLOW,
} culoare;
//initializarea cu 3 elemente
void init(vsite *v);
//realocare in caz ca nu este suficienta capacitatea actuala
void realoc(vsite *v);
//functie care extrage titlul din continut(il pune in string)
void extragetitlu(char *continut, char *string);
//declararea functiei de incarcare in memorie
void incarca(vsite *v);
//functie pentru incarcarea siteurilor ce au in continut
//cel putin un cuvant dintr-o lista data 
void incarcaR(vsite *v, char *lista);
//declararea functiei de eliberare
void descarca(vsite *v);
//functie care verifica daca un string contine un cuvant dintr-o lista(de cuvinte)
int contine(char *s, char *lista);
//functi de swap, pentru sortare
void swap(site *v1, site *v2);
//functie de sortare pentru un vector de tip site
void sortare(vsite *v, int (*cmp)(site v1, site v2));
#endif