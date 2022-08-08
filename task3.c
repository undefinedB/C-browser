#include "siteuri.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int cmp (site v1, site v2)
{
  return (v1.accesari > v2.accesari);
}

void afisare(vsite *v)
{
    int n = v->indice;
    for(int i=0;i<n;i++)
    {
        printf("%s\n", (v->v)[i].URL);
    }
    return;
}

int main()
{
    vsite *v = malloc (sizeof(vsite));
    char setcuvinte[100];
    fgets(setcuvinte, 100, stdin);
    incarcaR(v, setcuvinte);
    sortare(v, &cmp);
    afisare(v);
    descarca(v);
    return 0;
}