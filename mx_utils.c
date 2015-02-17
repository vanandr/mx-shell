#include <mx_utils.h>
#include <mx_debug.h>
/*
 * Remove all white spaces until encounter a character.
 */
void 
remove_white_spaces_until_character (char *str, int offset, int *_len)
{
    int indexJ;
    int len = *_len;
    log("%s:%d",str, len);
    while (offset < len && str[offset] != '\0' && str[offset] == ' ') {
        indexJ = offset;
        while (indexJ <= len) {
            str[indexJ] = str[indexJ+1];
            indexJ++;
        }
        len--;
    }
    *_len = len;
}

void print_cmd_error (char* inputcmdbuff, int errorindex) 
{
        int i = 0;
        printf("\n%s\n",inputcmdbuff);
                
        while (i < (errorindex-1)) {
            printf(" ");
            i++;
        }
        printf(KRED"^^^"KNRM);
}             
