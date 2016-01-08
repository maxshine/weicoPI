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


PTR_USER_ENTITY get_friends_byid(const char* access_token, const char* uid, int cursor)
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

  response = https_get(WEIBO_GET_FOLLOWERS_URL, request);
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

  free_http_request(request);
  free_http_response(response);
  cJSON_Delete(root);
  debug_log_exit(FINE, func_name);
  return list_head;

}

PTR_USER_ENTITY get_friends_byname(const char* access_token, const char* screen_name, int cursor)
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

  response = https_get(WEIBO_GET_FOLLOWERS_URL, request);
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

  free_http_request(request);
  free_http_response(response);
  cJSON_Delete(root);
  debug_log_exit(FINE, func_name);
  return list_head;

}

PTR_USER_ENTITY get_followers_byid(const char* access_token, const char* uid, int cursor)
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

  response = https_get(WEIBO_GET_FOLLOWERS_URL, request);
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
    follower = create_user_entity_from_json(cJSON_GetArrayItem(cJSON_GetObjectItem(root, "users"), i));
    follower->next = list_head;
    follower->prev = NULL;
    if (list_head != NULL) {
      list_head->prev = follower;
    }
    list_head = follower;
  }

  free_http_request(request);
  free_http_response(response);
  cJSON_Delete(root);
  debug_log_exit(FINE, func_name);
  return list_head;

}

PTR_USER_ENTITY get_followers_byname(const char* access_token, const char* screen_name, int cursor)
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

  response = https_get(WEIBO_GET_FOLLOWERS_URL, request);
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
    follower = create_user_entity_from_json(cJSON_GetArrayItem(cJSON_GetObjectItem(root, "users"), i));
    follower->next = list_head;
    follower->prev = NULL;
    if (list_head != NULL) {
      list_head->prev = follower;
    }
    list_head = follower;
  }

  free_http_request(request);
  free_http_response(response);
  cJSON_Delete(root);
  debug_log_exit(FINE, func_name);
  return list_head;

}
