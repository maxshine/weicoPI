#ifndef WEIBO_OATH2_H
#define WEIBO_OATH2_H

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp);
/*void * get_authorize_code(void *userp, size_t (*callback)( char *, size_t, size_t, void *));*/
char* get_authorize_code(char* buffer);
char* get_header_field_from_fime(FILE* input_file, char* buffer);
#endif
