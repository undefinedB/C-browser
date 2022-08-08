#include "siteuri.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
void afisare(vsite *v)
{
    int n = v->indice;
    for(int i=0;i<n;i++)
    {
        printf("%s %d %s\n", (v->v)[i].URL, (v->v)[i].accesari, (v->v)[i].titlu);
    }
    return;
}
int main()
{
    vsite *v;
    v = malloc(sizeof(vsite));
    incarca(v);
    afisare(v);
    descarca(v);
    return 0;
}