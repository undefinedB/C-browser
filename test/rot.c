#include <stdio.h>
#include <string.h>
char rotr(char x, int k)
{
    k = k%8;
    return ((x << (8-k)) | (x >> k));
}

char rotl(char x, int k)
{
    k = k%8;
    return ((x >> (8-k)) | (x << k));
}

char rot(char x, int k)
{
    if(k%2) return rotl(x, k);
    return rotr(x, k);
}

char checksum(char *s)
{
    char check = rot(s[0],0);
    for(int i=1; i!=0; i++)
    {
        if(s[i] == '\0') return check;
        else check = check ^ rot(s[i], i);
    }
}

int main()
{
    char x = 101;
    char s[1000];
    fgets(s, 1000, stdin);
    int y = strlen(s);
    s[y-1]='\0';
    printf("%d",checksum(s));
    return 0;
}