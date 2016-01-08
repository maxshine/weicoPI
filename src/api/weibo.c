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

BOOL create_weibo_text(const char* access_token, const char* text)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "ss", access_token, text);
  cJSON* root = NULL;
  PTR_HTTP_REQUEST request = alloc_http_request(0, 0, 2, 0);
  PTR_HTTP_RESPONSE response = NULL;
  request->form[0].name = "access_token";
  request->form[0].value = access_token;
  request->form[0].type = STRING;
  request->form[1].name = "status";
  request->form[1].value = text;
  request->form[1].type = STRING;

  response = https_post(WEIBO_CREATE_URL, request);
  if (response->status_code != 200) {
    return FALSE;
  }

  root = cJSON_Parse((char*)(response->body));
  if (check_api_error(root)) {
    cJSON_Delete(root);
    return FALSE;
  }

  return TRUE;
}

BOOL destroy_weibo(const char* access_token, const char* weibo_id)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "ss", access_token, weibo_id);
  cJSON* root = NULL;
  PTR_HTTP_REQUEST request = alloc_http_request(0, 0, 2, 0);
  PTR_HTTP_RESPONSE response = NULL;
  request->form[0].name = "access_token";
  request->form[0].value = access_token;
  request->form[0].type = STRING;
  request->form[1].name = "id";
  request->form[1].value = weibo_id;
  request->form[1].type = STRING;

  response = https_post(WEIBO_CREATE_URL, request);
  if (response->status_code != 200) {
    return FALSE;
  }

  root = cJSON_Parse((char*)(response->body));
  if (check_api_error(root)) {
    cJSON_Delete(root);
    return FALSE;
  }

  return TRUE;
}

BOOL repost_weibo(const char* access_token, const char* text, const char* weibo_id)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "sss", access_token, text, weibo_id);
  cJSON* root = NULL;
  PTR_HTTP_REQUEST request = alloc_http_request(0, 0, 3, 0);
  PTR_HTTP_RESPONSE response = NULL;
  request->form[0].name = "access_token";
  request->form[0].value = access_token;
  request->form[0].type = STRING;
  request->form[1].name = "status";
  request->form[1].value = text;
  request->form[1].type = STRING;
  request->form[2].name = "id";
  request->form[2].value = weibo_id;
  request->form[2].type = STRING;

  response = https_post(WEIBO_REPOST_URL, request);
  if (response->status_code != 200) {
    return FALSE;
  }

  root = cJSON_Parse((char*)(response->body));
  if (check_api_error(root)) {
    cJSON_Delete(root);
    return FALSE;
  }

  return TRUE;
}

BOOL create_weibo_pic(const char* access_token, const char* text, const char* pic_name)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "ss", access_token, text);
  cJSON* root = NULL;
  PTR_HTTP_REQUEST request = alloc_http_request(0, 0, 3, 0);
  PTR_HTTP_RESPONSE response = NULL;
  request->form[0].name = "access_token";
  request->form[0].value = access_token;
  request->form[0].type = STRING;
  request->form[1].name = "status";
  request->form[1].value = text;
  request->form[1].type = STRING;
  request->form[2].name = "pic";
  request->form[2].value = pic_name;
  request->form[2].value = FILENAME;

  response = https_post(WEIBO_CREATE_URL, request);
  if (response->status_code != 200) {
    return FALSE;
  }

  root = cJSON_Parse((char*)(response->body));
  if (check_api_error(root)) {
    cJSON_Delete(root);
    return FALSE;
  }

  return TRUE;
}

PTR_WEIBO_ENTITY get_public_timeline(const char* access_token, int page)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "sd", access_token, page);
  char s[20] = {};
  int i=0, cnt = 0;
  cJSON* root = NULL;
  PTR_WEIBO_ENTITY list_head = NULL;
  PTR_WEIBO_ENTITY weibo = NULL;
  PTR_HTTP_REQUEST request = alloc_http_request(2, 0, 0, 0);
  PTR_HTTP_RESPONSE response = NULL;
  request->params[0].name = "access_token";
  request->params[0].value = access_token;
  request->params[1].name = "page";
  snprintf(s, 20, "%d", cursor);
  request->params[1].value = s;

  response = https_get(WEIBO_GET_PUBLIC_TIMELINE_URL, request);
  if (response->status_code != 200) {
    return NULL;
  }

  root = cJSON_Parse((char*)(response->body));
  if (check_api_error(root)) {
    cJSON_Delete(root);
    return NULL;
  }

  cnt = cJSON_GetArraySize(cJSON_GetObjectItem(root, "statuses"));
  for (i=0; i<cnt; i++) {
    weibo = create_weibo_entity_from_json(cJSON_GetArrayItem(cJSON_GetObjectItem(root, "statuses"), i));
    weibo->next = list_head;
    weibo->prev = NULL;
    if (list_head != NULL) {
      list_head->prev = weibo;
    }
    list_head = weibo;
  }

  free_http_request(request);
  free_http_response(response);
  cJSON_Delete(root);
  debug_log_exit(FINE, func_name);
  return list_head;
}
