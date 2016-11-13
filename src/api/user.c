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
#include "weibo_datatype.h"
#include "http_action.h"
#include "constants.h"
#include "debug_util.h"
#include "weibo_util.h"

PTR_USER_ENTITY show_user_byid(const char* access_token, const char* uid)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "ss", access_token, uid);

  cJSON* root = NULL;
  char* s[20];
  PTR_USER_ENTITY user = NULL;
  PTR_HTTP_REQUEST request = alloc_http_request(2, 0, 0, 0);
  PTR_HTTP_RESPONSE response = NULL;
  request->params[0].name = "access_token";
  request->params[0].value = access_token;
  request->params[1].name = "uid";
  request->params[1].value = uid;
  
  response = https_get(WEIBO_SHOW_USER_URL, request);
  if (response->status_code != 200) {
    free_http_request(request);
    free_http_response(response);
    return NULL;
  }

  root = cJSON_Parse((char*)(response->body));
  if (check_api_error(root)) {
    free_http_request(request);
    free_http_response(response);
    cJSON_Delete(root);
    return NULL;
  }

  user = create_user_entity_from_json(root);
  free_http_request(request);
  free_http_response(response);
  cJSON_Delete(root);
  debug_log_exit(FINE, func_name);
  return user;
}

PTR_USER_ENTITY show_user_byname(const char* access_token, const char* screen_name)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "ss", access_token, screen_name);

  cJSON* root = NULL;
  char* s[20];
  PTR_USER_ENTITY user = NULL;
  PTR_HTTP_REQUEST request = alloc_http_request(2, 0, 0, 0);
  PTR_HTTP_RESPONSE response = NULL;
  request->params[0].name = "access_token";
  request->params[0].value = access_token;
  request->params[1].name = "screen_name";
  request->params[1].value = screen_name;

  response = https_get(WEIBO_SHOW_USER_URL, request);
  if (response->status_code != 200) {
    free_http_request(request);
    free_http_response(response);
    return NULL;
  }

  root = cJSON_Parse((char*)(response->body));
  if (check_api_error(root)) {
    free_http_request(request);
    free_http_response(response);
    cJSON_Delete(root);
    return NULL;
  }
  user = create_user_entity_from_json(root);
  free_http_request(request);
  free_http_response(response);
  cJSON_Delete(root);
  debug_log_exit(FINE, func_name);
  return user;
}

uint32_t show_user_rank(const char* access_token, const char* uid)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "ss", access_token, uid);

  cJSON* root = NULL;
  char*s[20];
  uint32_t rank = 0;
  PTR_USER_ENTITY user = NULL;
  PTR_HTTP_REQUEST request = alloc_http_request(2, 0, 0, 0);
  PTR_HTTP_RESPONSE response = NULL;
  request->params[0].name = "access_token";
  request->params[0].value = access_token;
  request->params[1].name = "uid";
  request->params[1].value = uid;

  response = https_get(WEIBO_SHOW_USER_RANK_URL, request);
  if (response->status_code != 200) {
    free_http_request(request);
    free_http_response(response);
    return 0;
  }
  root = cJSON_Parse((char*)(response->body));
  if (check_api_error(root)) {
    free_http_request(request);
    free_http_response(response);
    cJSON_Delete(root);
    return 0;
  }
  rank = (int)(cJSON_GetObjectItem(root, "rank")->valueint); 
  free_http_request(request);
  free_http_response(response);
  cJSON_Delete(root);
  debug_log_exit(FINE, func_name);
  return rank;
}
