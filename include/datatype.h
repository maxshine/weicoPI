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

#include <stdint.h>

#ifndef DATATYPE_H

#define DATATYPE_H

typedef enum BOOL {False=0, True=1} BOOL;

typedef enum http_form_type {
  STRING = 0,
  BINARY = 1,
  FILENAME = 2
} ENUM_HTTP_FORM_TYPE;

typedef struct http_header {
  const char *name;
  const char *value;
  struct http_header* next;
} HTTP_HEADER, *PTR_HTTP_HEADER;

typedef struct http_form {
  const char *name;
  const char *value;
  ENUM_HTTP_FORM_TYPE type;
  struct http_form* next;
} HTTP_FORM, *PTR_HTTP_FORM;

typedef struct http_param {
  const char *name;
  const char *value;
  struct http_form* next;
} HTTP_PARAM, *PTR_HTTP_PARAM;

typedef struct http_request {
  PTR_HTTP_PARAM params;
  uint32_t params_qty;
  PTR_HTTP_HEADER headers;
  uint32_t headers_qty;
  PTR_HTTP_FORM form;
  uint32_t form_length;
  void* body;
  uint32_t body_length;
} HTTP_REQUEST, *PTR_HTTP_REQUEST;

typedef struct http_response {
  long status_code;
  PTR_HTTP_HEADER headers;
  uint32_t headers_qty;
  void* body;
  uint32_t body_length;
  char* error;
} HTTP_RESPONSE, *PTR_HTTP_RESPONSE;

#endif
