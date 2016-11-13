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

extern uint32_t NEXT_FRIEND_CURSOR;
extern uint32_t PREVIOUS_FRIEND_CURSOR;
extern uint32_t NEXT_FOLLOWER_CURSOR;
extern uint32_t PREVIOUS_FOLLOWER_CURSOR;

PTR_USER_ENTITY get_friend_byid(const char* access_token, const char* uid, int cursor)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "ss", access_token, uid);
  char s[20] = {};
  int i=0, cnt = 0;
  cJSON* root = NULL;
  PTR_USER_ENTITY list_head = NULL;
  PTR_USER_ENTITY friend = NULL;
  PTR_HTTP_REQUEST request = alloc_http_request(2, 0, 0, 0);
  PTR_HTTP_RESPONSE response = NULL;
  request->params[0].name = "access_token";
  request->params[0].value = access_token;
  request->params[1].name = "uid";
  request->params[1].value = uid;
  request->params[2].name = "cursor";
  snprintf(s, 20, "%d", cursor);
  request->params[2].value = s;

  response = https_get(WEIBO_GET_FRIEND_URL, request);
  if (response->status_code != 200) {
    return NULL;
  }

  root = cJSON_Parse((char*)(response->body));
  if (check_api_error(root)) {
    cJSON_Delete(root);
    return NULL;
  }

  cnt = cJSON_GetArraySize(cJSON_GetObjectItem(root, "users"));
  for (i=0; i<cnt; i++) {
    friend = create_user_entity_from_json(cJSON_GetArrayItem(cJSON_GetObjectItem(root, "users"), i));
    friend->next = list_head;
    friend->prev = NULL;
    if (list_head != NULL) {
      list_head->prev = friend;
    }
    list_head = friend;
  }

  NEXT_FRIEND_CURSOR = cJSON_GetObjectItem(root, "next_cursor")->valueint;
  PREVIOUS_FRIEND_CURSOR = cJSON_GetObjectItem(root, "previous_cursor")->valueint;

  free_http_request(request);
  free_http_response(response);
  cJSON_Delete(root);
  debug_log_exit(FINE, func_name);
  return list_head;

}

PTR_USER_ENTITY get_friend_byname(const char* access_token, const char* screen_name, int cursor)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "ss", access_token, screen_name);
  char s[20] = {};
  int i=0, cnt = 0;
  cJSON* root = NULL;
  PTR_USER_ENTITY list_head = NULL;
  PTR_USER_ENTITY friend = NULL;
  PTR_HTTP_REQUEST request = alloc_http_request(2, 0, 0, 0);
  PTR_HTTP_RESPONSE response = NULL;
  request->params[0].name = "access_token";
  request->params[0].value = access_token;
  request->params[1].name = "screen_name";
  request->params[1].value = screen_name;
  request->params[2].name = "cursor";
  snprintf(s, 20, "%d", cursor);
  request->params[2].value = s;

  response = https_get(WEIBO_GET_FRIEND_URL, request);
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

  cnt = cJSON_GetArraySize(cJSON_GetObjectItem(root, "users"));
  for (i=0; i<cnt; i++) {
    friend = create_user_entity_from_json(cJSON_GetArrayItem(cJSON_GetObjectItem(root, "users"), i));
    friend->next = list_head;
    friend->prev = NULL;
    if (list_head != NULL) {
      list_head->prev = friend;
    }
    list_head = friend;
  }

  NEXT_FRIEND_CURSOR = cJSON_GetObjectItem(root, "next_cursor")->valueint;
  PREVIOUS_FRIEND_CURSOR = cJSON_GetObjectItem(root, "previous_cursor")->valueint;

  free_http_request(request);
  free_http_response(response);
  cJSON_Delete(root);
  debug_log_exit(FINE, func_name);
  return list_head;

}

PTR_USER_ENTITY get_follower_byid(const char* access_token, const char* uid, int cursor)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "ss", access_token, uid);
  char s[20] = {};
  int i=0, cnt = 0;
  cJSON* root = NULL;
  PTR_USER_ENTITY list_head = NULL;
  PTR_USER_ENTITY follower = NULL;
  PTR_HTTP_REQUEST request = alloc_http_request(2, 0, 0, 0);
  PTR_HTTP_RESPONSE response = NULL;
  request->params[0].name = "access_token";
  request->params[0].value = access_token;
  request->params[1].name = "uid";
  request->params[1].value = uid;
  request->params[2].name = "cursor";
  snprintf(s, 20, "%d", cursor);
  request->params[2].value = s;

  response = https_get(WEIBO_GET_FOLLOWER_URL, request);
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

  cnt = cJSON_GetArraySize(cJSON_GetObjectItem(root, "users"));
  for (i=0; i<cnt; i++) {
    follower = create_user_entity_from_json(cJSON_GetArrayItem(cJSON_GetObjectItem(root, "users"), i));
    follower->next = list_head;
    follower->prev = NULL;
    if (list_head != NULL) {
      list_head->prev = follower;
    }
    list_head = follower;
  }

  NEXT_FOLLOWER_CURSOR = cJSON_GetObjectItem(root, "next_cursor")->valueint;
  PREVIOUS_FOLLOWER_CURSOR = cJSON_GetObjectItem(root, "previous_cursor")->valueint;

  free_http_request(request);
  free_http_response(response);
  cJSON_Delete(root);
  debug_log_exit(FINE, func_name);
  return list_head;

}

PTR_USER_ENTITY get_follower_byname(const char* access_token, const char* screen_name, int cursor)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "ss", access_token, screen_name);
  char s[20] = {};
  int i=0, cnt = 0;
  cJSON* root = NULL;
  PTR_USER_ENTITY list_head = NULL;
  PTR_USER_ENTITY follower = NULL;
  PTR_HTTP_REQUEST request = alloc_http_request(2, 0, 0, 0);
  PTR_HTTP_RESPONSE response = NULL;
  request->params[0].name = "access_token";
  request->params[0].value = access_token;
  request->params[1].name = "screen_name";
  request->params[1].value = screen_name;
  request->params[2].name = "cursor";
  snprintf(s, 20, "%d", cursor);
  request->params[2].value = s;

  response = https_get(WEIBO_GET_FOLLOWER_URL, request);
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

  cnt = cJSON_GetArraySize(cJSON_GetObjectItem(root, "users"));
  for (i=0; i<cnt; i++) {
    follower = create_user_entity_from_json(cJSON_GetArrayItem(cJSON_GetObjectItem(root, "users"), i));
    follower->next = list_head;
    follower->prev = NULL;
    if (list_head != NULL) {
      list_head->prev = follower;
    }
    list_head = follower;
  }

  NEXT_FOLLOWER_CURSOR = cJSON_GetObjectItem(root, "next_cursor")->valueint;
  PREVIOUS_FOLLOWER_CURSOR = cJSON_GetObjectItem(root, "previous_cursor")->valueint;

  free_http_request(request);
  free_http_response(response);
  cJSON_Delete(root);
  debug_log_exit(FINE, func_name);
  return list_head;

}
