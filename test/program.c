#include <stdio.h>
#include "program.h"

int main()
{
    punct a,b;
    scanf("%lf%lf%lf%lf",&a.x,&a.y,&b.x,&b.y);
    printf("%lf",distanta2(a,b));
    return 0;
}