#include "siteuri.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

unsigned char rotr(unsigned char x, int k)
{
    k = k%8;
    return ((x << (8-k)) | (x >> k));
}

unsigned char rotl(unsigned char x, int k)
{
    k = k%8;
    return ((x >> (8-k)) | (x << k));
}

unsigned char rot(unsigned char x, int k)
{
    if(!(k%2)) return rotl(x, k);
    return rotr(x, k);
}

unsigned char checksum(char *s)
{
    unsigned char check = rot(s[0],0);
    for(int i=1; i!=0; i++)
    {
        if(s[i] == '\0') return check;
        else check = check ^ rot(s[i], i);
    }
    return 0;
}

void afisare(vsite *v, char *numesite)
{
    int n = v->indice;
    int ok = 0;
    for(int i=0; i<n; i++)
    {
        if(!strcmp((v->v)[i].URL, numesite)) {
            if((v->v)[i].checksum == checksum((v->v)[i].continut))
                printf("Website safe!\n");
            else 
                printf("Malicious website! Official key: %d. Found key: %d\n",(v->v)[i].checksum,checksum((v->v)[i].continut));
            return;
        }
    }
    if(!ok) 
        printf("Website not found!\n");
    return;
}

int main()
{
    vsite *v;
    char s[200];
    v = malloc(sizeof(vsite));
    incarca(v);
    while(fgets(s, 200, stdin))
        { if(s[0]=='\n') return 0;
        int x = strlen(s);
        s[x-1] = '\0';
        afisare(v, s);}

    descarca(v);
    return 0;
}