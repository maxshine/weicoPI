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
