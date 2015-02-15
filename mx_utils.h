#ifndef __MX_UTILS_H__
#define __MX_UTILS_H__

extern void remove_white_spaces_until_character(char *str, 
                                                int offset, 
                                                int *_len);
extern void print_cmd_error(char* inputcmdbuff, int errorindex);
#endif //__MX_UTILS_H__

