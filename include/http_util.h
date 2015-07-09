#ifndef HTTP_UTIL_H
#define HTTP_UTIL_H

size_t write_body(char *ptr, size_t size, size_t nmemb, void *userdata);
size_t parse_header( void *ptr, size_t size, size_t nmemb, void  *userdata);
char* get_header_field_from_file(FILE* input_file, const char* prefix, char* buffer);

#endif
