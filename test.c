#include <stdio.h>
#include <string.h>
int main ()
{
    char *ch = "Hello world";
    char *ch2 = strndup(ch, 2);
    printf(" ch  %s\n", ch);
    printf(" ch2 %s", ch2);
}
