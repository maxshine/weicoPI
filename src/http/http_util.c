/****************************************************************************
 * Copyright (c) 2013-now                                                   *
 * This software is subjected to Apache License Version 2.0, January 2004   *
 * http://www.apache.org/licenses/                                          *
 * Permission is hereby granted, free of charge, to any person obtaining a  *
 * copy of this software and associated documentation files (the            *
 * "Software"), to deal in the Software without restriction, including      *
 * without limitation the rights to use, copy, modify, merge, publish,      *
 * distribute, distribute with modifications, sublicense, and/or sell       *
 * copies of the Software, and to permit persons to whom the Software is    *
 * furnished to do so, subject to the following conditions:                 *
 *                                                                          *
 * The above copyright notice and this permission notice shall be included  *
 * in all copies or substantial portions of the Software.                   *
 *                                                                          *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS  *
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF               *
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   *
 * IN NO EVENT SHALL THE ABOVE COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,   *
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR    *
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR    *
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.                               *
 *                                                                          *
 * Except as contained in this notice, the name(s) of the above copyright   *
 * holders shall not be used in advertising or otherwise to promote the     *
 * sale, use or other dealings in this Software without prior written       *
 * authorization.                                                           *
 ****************************************************************************/

/****************************************************************************
 *  Author: Yang, Gao  <maxshine@gmail.com> 2013-on                         *
 ****************************************************************************/

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
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  debug_log(FINEST, func_name, ptr);
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
    response->headers[0].name = (char*) malloc(30*sizeof(char));
    response->headers[0].value = (char*) malloc(size*nmemb*sizeof(char)+1);
    memset((void*)(response->headers[0].name), '\0', 15*sizeof(char));
    memset((void*)(response->headers[0].value), '\0', size*nmemb*sizeof(char)+1);
    response->headers_qty = 1;
  } else {
    response->headers = (PTR_HTTP_HEADER) realloc(response->headers, sizeof(HTTP_HEADER)*(response->headers_qty+1));
    response->headers_qty += 1;
    response->headers[response->headers_qty-1].name =(char*) malloc(30*sizeof(char));
    response->headers[response->headers_qty-1].value = (char*) malloc(size*nmemb*sizeof(char)+1);
    memset((void*)(response->headers[response->headers_qty-1].name), '\0', 30*sizeof(char));
    memset((void*)(response->headers[response->headers_qty-1].value), '\0', size*nmemb*sizeof(char)+1);
  }
  strncpy((char*)(response->headers[response->headers_qty-1].name), trim(p), 30);
  strncpy((char*)(response->headers[response->headers_qty-1].value), trim(i+1), size*nmemb);
  debug_log_exit(FINE, func_name);
  return size*nmemb;
}

size_t write_body(char *ptr, size_t size, size_t nmemb, void *userdata)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  debug_log(FINEST, func_name, ptr);
  PTR_HTTP_RESPONSE response = userdata;
  if (response->body == NULL) {
    response->body = (void*) malloc(size*nmemb);
    memset(response->body, '\0', size*nmemb);
    response->body_length = (uint32_t) size*nmemb;
    memcpy(response->body, ptr, size*nmemb);
  }else {
    response->body = (void*) realloc(response->body, size*nmemb*+response->body_length);
    memset(response->body+response->body_length, '\0', size*nmemb);
    memcpy(response->body+response->body_length, ptr, size*nmemb);
    response->body_length += size*nmemb;
  }
  debug_log_exit(FINE, func_name);
  return size*nmemb;
}
