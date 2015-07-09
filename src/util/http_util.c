#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include "datatype.h"
#include "debug_util.h"

char* get_header_field_from_file(FILE* input_file, const char* prefix, char* buffer)
{
  int flag_scanf = 0;
  char* str = NULL;
  char* prefix_str = NULL;

  str = (char*)malloc(100*sizeof(char));
  prefix_str = (char*)malloc((strlen(prefix)+3)*sizeof(char));
  memset(str, '\0', 100*sizeof(char));
  memset(prefix_str, '\0', (strlen(prefix)+3)*sizeof(char));
  sprintf(prefix_str, "%s%%s", prefix);

  rewind(input_file);

  while(fgets(str, 100, input_file)) {
    flag_scanf = sscanf(str, prefix_str, buffer);
    if(flag_scanf != 0) {
      break;
    }
  }

  rewind(input_file);
}

char* lstrip(char* ptr)
{
  while(*ptr && isspace(*ptr)) {
      ptr++;
  }
  return ptr;
}

char* rstrip(char* ptr)
{
  char* p = ptr + strlen(ptr) -1;
  while (p >= ptr && *p && isspace(*p)) {
    p--;
  }
  *(p+1) = '\0';
  return ptr;
}

char* trim(char* ptr)
{
  ptr = lstrip(ptr);
  ptr = rstrip(ptr);
  return ptr;
}

size_t parse_header( void *ptr, size_t size, size_t nmemb, void  *userdata)
{
  char* i = NULL;
  char* p = (char*)ptr;
  i = strchr(ptr, ':');
  if (i == NULL) {
    return size*nmemb;
  }
  *i = '\0';
  PTR_HTTP_RESPONSE response = userdata;
  if (response->headers == NULL) {
    response->headers = (PTR_HTTP_HEADER) malloc(1*sizeof(HTTP_HEADER));
    response->headers[0].name = (char*) malloc(15*sizeof(char));
    response->headers[0].value = (char*) malloc(size*nmemb*sizeof(char)+1);
    memset(response->headers[0].name, '\0', 15*sizeof(char));
    memset(response->headers[0].value, '\0', size*nmemb*sizeof(char)+1);
    response->headers_qty = 1;
  } else {
    response->headers = (PTR_HTTP_HEADER) realloc(response->headers, sizeof(HTTP_HEADER)*(response->headers_qty+1));
    response->headers_qty += 1;
    response->headers[response->headers_qty-1].name =(char*) malloc(15*sizeof(char));
    response->headers[response->headers_qty-1].value = (char*) malloc(size*nmemb*sizeof(char)+1);
    memset(response->headers[response->headers_qty-1].name, '\0', 15*sizeof(char));
    memset(response->headers[response->headers_qty-1].value, '\0', size*nmemb*sizeof(char)+1);
  }
  strncpy(response->headers[response->headers_qty-1].name, trim(p), 15);
  strncpy(response->headers[response->headers_qty-1].value, trim(i+1), size*nmemb);
}

size_t write_body(char *ptr, size_t size, size_t nmemb, void *userdata)
{
  PTR_HTTP_RESPONSE response = userdata;
  if (response->body == NULL) {
    response->body = (void*) malloc(size*nmemb+1);
    memset(response->body, '\0', size*nmemb+1);
    response->body_length = (uint32_t) size*nmemb+1;
  } else {
    response->body = (void*) realloc(response->body, size*nmemb*+response->body_length);
    memset(response->body+response->body_length, '\0', size*nmemb);
    response->body_length += size*nmemb;
  }
  memcpy(response->body, ptr, size*nmemb);
  return size*nmemb;
}
