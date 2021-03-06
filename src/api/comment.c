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

BOOL create_comment(const char *access_token, const char *weibo_id, const char *comment)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "sss", access_token, weibo_id, comment);
  cJSON* root = NULL;
  PTR_HTTP_REQUEST request = alloc_http_request(0, 0, 0, (100+strlen(comment))*sizeof(char));
  PTR_HTTP_RESPONSE response = NULL;
  /*
  request->form[0].name = "access_token";
  request->form[0].value = access_token;
  request->form[0].type = STRING;
  request->form[1].name = "comment";
  request->form[1].value = comment;
  request->form[1].type = STRING;
  request->form[2].name = "id";
  request->form[2].value = weibo_id;
  request->form[2].type = STRING;
  */
  sprintf((char*)(request->body), "access_token=%s&id=%s&comment=%s", access_token, weibo_id, comment);
  response = https_post(WEIBO_CREATE_COMMENT_URL, request);
  if (response->status_code != 200) {
    free_http_request(request);
    free_http_response(response);
    return False;
  }

  root = cJSON_Parse((char*)(response->body));
  if (check_api_error(root)) {
    free_http_request(request);
    free_http_response(response);
    cJSON_Delete(root);
    return False;
  }
  free_http_request(request);
  free_http_response(response);
  cJSON_Delete(root);
  debug_log_exit(FINE, func_name);

  return True;

}

BOOL destroy_comment(const char *access_token, const char *comment_id)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "ss", access_token, comment_id);
  cJSON* root = NULL;
  PTR_HTTP_REQUEST request = alloc_http_request(0, 0, 0, 100);
  PTR_HTTP_RESPONSE response = NULL;
  /*
  request->form[0].name = "access_token";
  request->form[0].value = access_token;
  request->form[0].type = STRING;
  request->form[1].name = "cid";
  request->form[1].value = comment_id;
  request->form[1].type = STRING;
  */
  sprintf((char*)(request->body), "access_token=%s&comment_id=%s", access_token, comment_id);
  response = https_post(WEIBO_DESTROY_COMMENT_URL, request);
  if (response->status_code != 200) {
    free_http_request(request);
    free_http_response(response);
    return False;
  }

  root = cJSON_Parse((char*)(response->body));
  if (check_api_error(root)) {
    free_http_request(request);
    free_http_response(response);
    cJSON_Delete(root);
    return False;
  }
  free_http_request(request);
  free_http_response(response);
  cJSON_Delete(root);
  debug_log_exit(FINE, func_name);
  return True;

}

BOOL reply_comment(const char *access_token, const char *weibo_id, const char *comment_id, const char *comment)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "ssss", access_token, weibo_id, comment_id, comment);
  cJSON* root = NULL;
  PTR_HTTP_REQUEST request = alloc_http_request(0, 0, 0, (100+strlen(comment)*sizeof(char)));
  PTR_HTTP_RESPONSE response = NULL;
  /*
  request->form[0].name = "access_token";
  request->form[0].value = access_token;
  request->form[0].type = STRING;
  request->form[1].name = "comment";
  request->form[1].value = comment;
  request->form[1].type = STRING;
  request->form[2].name = "id";
  request->form[2].value = weibo_id;
  request->form[2].type = STRING;
  request->form[3].name = "cid";
  request->form[3].value = comment_id;
  request->form[3].type = STRING;
  */
  sprintf((char*)(request->body), "access_token=%s&id=%s&cid=&comment=%s", access_token, weibo_id, comment_id, comment);
  response = https_post(WEIBO_REPLY_COMMENT_URL, request);
  if (response->status_code != 200) {
    free_http_request(request);
    free_http_response(response);
    return False;
  }

  root = cJSON_Parse((char*)(response->body));
  if (check_api_error(root)) {
    free_http_request(request);
    free_http_response(response);
    cJSON_Delete(root);
    return False;
  }
  free_http_request(request);
  free_http_response(response);
  cJSON_Delete(root);
  debug_log_exit(FINE, func_name);
  return True;

}

PTR_COMMENT_ENTITY show_comment(const char *access_token, const char *weibo_id, int page)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "ssd", access_token, weibo_id, page);
  char s[20] = {};
  int i=0, cnt = 0;
  cJSON* root = NULL;
  PTR_COMMENT_ENTITY list_head = NULL;
  PTR_COMMENT_ENTITY comment = NULL;
  PTR_HTTP_REQUEST request = alloc_http_request(3, 0, 0, 0);
  PTR_HTTP_RESPONSE response = NULL;
  request->params[0].name = "access_token";
  request->params[0].value = access_token;
  request->params[1].name = "page";
  snprintf(s, 20, "%d", page);
  request->params[1].value = s;
  request->params[2].name = "id";
  request->params[2].value = weibo_id;

  response = https_get(WEIBO_SHOW_COMMENTS_URL, request);
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

  cnt = cJSON_GetArraySize(cJSON_GetObjectItem(root, "comments"));
  for (i=0; i<cnt; i++) {
    comment = create_comment_entity_from_json(cJSON_GetArrayItem(cJSON_GetObjectItem(root, "comments"), i));
    comment->next = list_head;
    comment->prev = NULL;
    if (list_head != NULL) {
      list_head->prev = comment;
    }
    list_head = comment;
  }

  free_http_request(request);
  free_http_response(response);
  cJSON_Delete(root);
  debug_log_exit(FINE, func_name);
  return list_head;
}
