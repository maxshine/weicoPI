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
