#include <stdint.h>
#include "datatype.h"

#ifndef HTTP_ACTION_H

#define HTTP_ACTION_H

PTR_HTTP_REQUEST alloc_http_request(uint32_t params_qty, uint32_t headers_qty, uint32_t form_length, uint32_t body_length);
PTR_HTTP_RESPONSE alloc_http_response(uint32_t headers_qty, uint32_t body_length);

void free_http_request(PTR_HTTP_REQUEST request);
void free_http_response(PTR_HTTP_RESPONSE response);
void destroy_http_request(PTR_HTTP_REQUEST request);
void destroy_http_response(PTR_HTTP_RESPONSE response);

PTR_HTTP_RESPONSE http_get(char* url, PTR_HTTP_REQUEST request);
PTR_HTTP_RESPONSE http_post(char* url, PTR_HTTP_REQUEST request);

PTR_HTTP_RESPONSE https_get(char* url, PTR_HTTP_REQUEST request);
PTR_HTTP_RESPONSE https_post(char* url, PTR_HTTP_REQUEST request);


#endif
