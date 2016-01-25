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
    return NULL;
  }

  root = cJSON_Parse((char*)(response->body));
  if (check_api_error(root)) {
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
    return NULL;
  }

  root = cJSON_Parse((char*)(response->body));
  if (check_api_error(root)) {
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

int show_user_rank(const char* access_token, const char* uid)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "ss", access_token, uid);

  cJSON* root = NULL;
  char*s[20];
  int rank = 0;
  PTR_USER_ENTITY user = NULL;
  PTR_HTTP_REQUEST request = alloc_http_request(2, 0, 0, 0);
  PTR_HTTP_RESPONSE response = NULL;
  request->params[0].name = "access_token";
  request->params[0].value = access_token;
  request->params[1].name = "uid";
  request->params[1].value = uid;

  response = https_get(WEIBO_SHOW_USER_RANK_URL, request);
  if (response->status_code != 200) {
    return 0;
  }
  root = cJSON_Parse((char*)(response->body));
  if (check_api_error(root)) {
    cJSON_Delete(root);
    return NULL;
  }
  rank = cJSON_GetObjectItem(root, "rank")->valueint; 
  free_http_request(request);
  free_http_response(response);
  cJSON_Delete(root);
  debug_log_exit(FINE, func_name);
  return user;
}

