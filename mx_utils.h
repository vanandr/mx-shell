#ifndef __MX_UTILS_H__
#define __MX_UTILS_H__

#define KNRM  "\x1B[0m"
//#define KRED  "\x1B[31m"
#define KRED  "\033[1m\033[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
//#define KCYN  "\x1B[36m"
#define KCYN  "\033[1m\033[36m"
#define KWHT  "\x1B[37m"

extern void remove_white_spaces_until_character(char *str, 
                                                int offset, 
                                                int *_len);
extern void print_cmd_error(char* inputcmdbuff, int errorindex);
#endif //__MX_UTILS_H__

