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


#include <curl/curl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "cJSON.h"
#include "datatype.h"
#include "http_action.h"
#include "constants.h"
#include "debug_util.h"

uint64_t get_account_userid(const char* access_token)
{
        const char* func_name = __func__;
        debug_log_enter(FINE, func_name, "s", access_token);

	cJSON* root = NULL;
	long uid = 0L;

	PTR_HTTP_REQUEST request = alloc_http_request(1, 0, 1, 0);
	PTR_HTTP_RESPONSE response = NULL;
	request->params[0].name = "access_token";
	request->params[0].value = access_token;
	response = https_get(WEIBO_GET_ACCOUNTID_URL, request);
	if (response->status_code != 200) {
	  free_http_request(request);
	  free_http_response(response);
	  return 0;
	}

	root = cJSON_Parse((char*)(response->body));
	uid = (uint64_t) cJSON_GetObjectItem(root, "uid")->valuedouble;
	free_http_request(request);
	free_http_response(response);
	cJSON_Delete(root);
        debug_log_exit(FINE, func_name);
        return uid;
}
