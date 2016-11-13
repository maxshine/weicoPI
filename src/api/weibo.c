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

#define MAX_BATCH_SIZE 50

BOOL create_weibo_text(const char* access_token, const char* text)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "ss", access_token, text);
  cJSON* root = NULL;
  PTR_HTTP_REQUEST request = alloc_http_request(0, 0, 0, (50+strlen(text))*sizeof(char));
  PTR_HTTP_RESPONSE response = NULL;
  /*
  request->form[0].name = "access_token";
  request->form[0].value = access_token;
  request->form[0].type = STRING;
  request->form[1].name = "status";
  request->form[1].value = text;
  request->form[1].type = STRING;
  */
  sprintf((char*)(request->body), "access_token=%s&status=%s", access_token, text);
  request->body_length = strlen((char*)(request->body));
  response = https_post(WEIBO_CREATE_URL, request);
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

BOOL destroy_weibo(const char* access_token, const char* weibo_id)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "ss", access_token, weibo_id);
  cJSON* root = NULL;
  PTR_HTTP_REQUEST request = alloc_http_request(0, 0, 0, 100);
  PTR_HTTP_RESPONSE response = NULL;
  /*
  request->form[0].name = "access_token";
  request->form[0].value = access_token;
  request->form[0].type = STRING;
  request->form[1].name = "id";
  request->form[1].value = weibo_id;
  request->form[1].type = STRING;
  */
  sprintf((char*)(request->body), "access_token=%s&id=%s", access_token, weibo_id);
  request->body_length = strlen((char*)(request->body));
  response = https_post(WEIBO_CREATE_URL, request);
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

BOOL repost_weibo(const char* access_token, const char* text, const char* weibo_id)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "sss", access_token, text, weibo_id);
  cJSON* root = NULL;
  PTR_HTTP_REQUEST request = alloc_http_request(0, 0, 0, (100+strlen(text))*sizeof(char));
  PTR_HTTP_RESPONSE response = NULL;
  /*
  request->form[0].name = "access_token";
  request->form[0].value = access_token;
  request->form[0].type = STRING;
  request->form[1].name = "status";
  request->form[1].value = text;
  request->form[1].type = STRING;
  request->form[2].name = "id";
  request->form[2].value = weibo_id;
  request->form[2].type = STRING;
  */
  sprintf((char*)(request->body), "access_token=%s&id=%s&status=%s", access_token, weibo_id, text);
  request->body_length = strlen((char*)(request->body));
  response = https_post(WEIBO_REPOST_URL, request);
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
  request->form[2].type = FILENAME;

  response = https_post(WEIBO_CREATE_URL, request);
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

PTR_WEIBO_ENTITY show_single_weibo_byid(const char* access_token, const char* weibo_id)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "ss", access_token, weibo_id);
  char s[20] = {};
  cJSON* root = NULL;
  PTR_WEIBO_ENTITY weibo = NULL;
  PTR_HTTP_REQUEST request = alloc_http_request(2, 0, 0, 0);
  PTR_HTTP_RESPONSE response = NULL;
  request->params[0].name = "access_token";
  request->params[0].value = access_token;
  request->params[1].name = "id";
  request->params[1].value = weibo_id;

  response = https_get(WEIBO_SHOW_WEIBO_URL, request);
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
  weibo = create_weibo_entity_from_json(root);
  free_http_request(request);
  free_http_response(response);
  cJSON_Delete(root);
  debug_log_exit(FINE, func_name);
  return weibo;
}

PTR_WEIBO_ENTITY show_multiple_weibo_byids(const char* access_token, char** weibo_ids, uint32_t count)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "spd", access_token, weibo_ids, count);
  char s[2048] = {};
  uint32_t i = 0;
  uint32_t cnt = count>MAX_BATCH_SIZE?MAX_BATCH_SIZE:count;
  cJSON* root = NULL;
  cJSON* statuses = NULL;
  PTR_WEIBO_ENTITY list_head = NULL;
  PTR_WEIBO_ENTITY weibo = NULL;
  PTR_HTTP_REQUEST request = alloc_http_request(2, 0, 0, 0);
  PTR_HTTP_RESPONSE response = NULL;

  memset(s, 0, sizeof(char)*sizeof(s));
  sprintf(s, "%s", weibo_ids[0]);
  for (i=1; i<cnt; i++) {
    sprintf(s, "%s,%s", s, weibo_ids[i]);
  }
  s[strlen(s)] = '\0';

  request->params[0].name = "access_token";
  request->params[0].value = access_token;
  request->params[1].name = "ids";
  request->params[1].value = s;

  response = https_get(WEIBO_SHOW_WEIBO_BATCH_URL, request);
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

  statuses = cJSON_GetObjectItem(root, "statuses");
  cnt = cJSON_GetArraySize(statuses);

  for (i=0; i<cnt; i++) {
    weibo = create_weibo_entity_from_json(cJSON_GetArrayItem(statuses, i));
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

PTR_WEIBO_ENTITY get_user_timeline_byids(const char* access_token, const char* uid, int page)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "ssd", access_token, uid, page);
  char s[20] = {};
  char **weibo_ids = NULL;
  int i = 0, cnt = 0;
  cJSON* root = NULL;
  PTR_WEIBO_ENTITY list_head = NULL;
  PTR_WEIBO_ENTITY weibo = NULL;
  PTR_HTTP_REQUEST request = alloc_http_request(3, 0, 0, 0);
  PTR_HTTP_RESPONSE response = NULL;
  request->params[0].name = "access_token";
  request->params[0].value = access_token;
  request->params[1].name = "uid";
  request->params[1].value = uid;
  snprintf(s, 20, "%d", page);
  request->params[2].name="page";
  request->params[2].value=s;

  response = https_get(WEIBO_GET_USER_TIMELINE_IDS_URL, request);
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
  cnt = cJSON_GetArraySize(cJSON_GetObjectItem(root, "statuses"));
  weibo_ids = (char**) malloc(sizeof(char*)*cnt);
  for (i=0; i<cnt; i++) {
    weibo_ids[i] = (cJSON_GetArrayItem(cJSON_GetObjectItem(root, "statuses"), i))->valuestring;
  }
  list_head = show_multiple_weibo_byids(access_token, weibo_ids, cnt);
  free_http_request(request);
  free_http_response(response);
  free(weibo_ids);
  cJSON_Delete(root);
  debug_log_exit(FINE, func_name);
  return list_head;
}

PTR_WEIBO_ENTITY get_friend_timeline_byids(const char* access_token, int page)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "ssd", access_token, page);
  char s[20] = {};
  char **weibo_ids = NULL;
  int i = 0, cnt = 0;
  cJSON* root = NULL;
  PTR_WEIBO_ENTITY list_head = NULL;
  PTR_WEIBO_ENTITY weibo = NULL;
  PTR_HTTP_REQUEST request = alloc_http_request(2, 0, 0, 0);
  PTR_HTTP_RESPONSE response = NULL;
  request->params[0].name = "access_token";
  request->params[0].value = access_token;
  snprintf(s, 20, "%d", page);
  request->params[1].name="page";
  request->params[1].value=s;

  response = https_get(WEIBO_GET_FRIEND_TIMELINE_IDS_URL, request);
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
  cnt = cJSON_GetArraySize(cJSON_GetObjectItem(root, "statuses"));
  weibo_ids = (char**) malloc(sizeof(char*)*cnt);
  for (i=0; i<cnt; i++) {
    weibo_ids[i] = (cJSON_GetArrayItem(cJSON_GetObjectItem(root, "statuses"), i))->valuestring;
  }
  list_head = show_multiple_weibo_byids(access_token, weibo_ids, cnt);
  free_http_request(request);
  free_http_response(response);
  free(weibo_ids);
  cJSON_Delete(root);
  debug_log_exit(FINE, func_name);
  return list_head;
}


PTR_WEIBO_ENTITY get_user_timeline_byname(const char* access_token, const char* name, int page)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "ssd", access_token, name, page);
  char s[20] = {};
  char *weibo_id = NULL;
  int i = 0, cnt = 0;
  cJSON* root = NULL;
  PTR_WEIBO_ENTITY list_head = NULL;
  PTR_WEIBO_ENTITY weibo = NULL;
  PTR_HTTP_REQUEST request = alloc_http_request(4, 0, 0, 0);
  PTR_HTTP_RESPONSE response = NULL;
  request->params[0].name = "access_token";
  request->params[0].value = access_token;
  request->params[1].name = "name";
  request->params[1].value = name;
  snprintf(s, 20, "%d", page);
  request->params[2].name="page";
  request->params[2].value=s;
  request->params[3].name="count";
  request->params[3].value="5"; /* hard code as 5 for api invocation limit */

  response = https_get(WEIBO_GET_USER_TIMELINE_IDS_URL, request);
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
  cnt = cJSON_GetArraySize(cJSON_GetObjectItem(root, "statuses"));
  for (i=0; i<cnt; i++) {
    weibo_id = (cJSON_GetArrayItem(cJSON_GetObjectItem(root, "statuses"), i))->valuestring;
    weibo = show_single_weibo_byid(access_token, weibo_id);
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

PTR_WEIBO_ENTITY get_public_timeline(const char* access_token, int page)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "sd", access_token, page);
  char s[20] = {};
  int i=0, cnt = 0;
  cJSON* root = NULL;
  cJSON* statuses = NULL;
  PTR_WEIBO_ENTITY list_head = NULL;
  PTR_WEIBO_ENTITY weibo = NULL;
  PTR_HTTP_REQUEST request = alloc_http_request(2, 0, 0, 0);
  PTR_HTTP_RESPONSE response = NULL;
  request->params[0].name = "access_token";
  request->params[0].value = access_token;
  request->params[1].name = "page";
  snprintf(s, 20, "%d", page);
  request->params[1].value = s;

  response = https_get(WEIBO_GET_PUBLIC_TIMELINE_URL, request);
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

  statuses = cJSON_GetObjectItem(root, "statuses");
  cnt = cJSON_GetArraySize(statuses);

  for (i=0; i<cnt; i++) {
    weibo = create_weibo_entity_from_json(cJSON_GetArrayItem(statuses, i));
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

PTR_WEIBO_ENTITY get_friend_timeline(const char* access_token, int page)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "sd", access_token, page);
  char s[20] = {};
  int i=0, cnt = 0;
  cJSON* root = NULL;
  cJSON* statuses = NULL;
  PTR_WEIBO_ENTITY list_head = NULL;
  PTR_WEIBO_ENTITY weibo = NULL;
  PTR_HTTP_REQUEST request = alloc_http_request(2, 0, 0, 0);
  PTR_HTTP_RESPONSE response = NULL;
  request->params[0].name = "access_token";
  request->params[0].value = access_token;
  request->params[1].name = "page";
  snprintf(s, 20, "%d", page);
  request->params[1].value = s;

  response = https_get(WEIBO_GET_FRIEND_TIMELINE_URL, request);
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

  statuses = cJSON_GetObjectItem(root, "statuses");
  cnt = cJSON_GetArraySize(statuses);

  for (i=0; i<cnt; i++) {
    weibo = create_weibo_entity_from_json(cJSON_GetArrayItem(statuses, i));
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

PTR_WEIBO_ENTITY get_home_timeline(const char* access_token, int page)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "sd", access_token, page);
  char s[20] = {};
  int i=0, cnt = 0;
  cJSON* root = NULL;
  cJSON* statuses = NULL;
  PTR_WEIBO_ENTITY list_head = NULL;
  PTR_WEIBO_ENTITY weibo = NULL;
  PTR_HTTP_REQUEST request = alloc_http_request(2, 0, 0, 0);
  PTR_HTTP_RESPONSE response = NULL;
  request->params[0].name = "access_token";
  request->params[0].value = access_token;
  request->params[1].name = "page";
  snprintf(s, 20, "%d", page);
  request->params[1].value = s;

  response = https_get(WEIBO_GET_HOME_TIMELINE_URL, request);
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

  statuses = cJSON_GetObjectItem(root, "statuses");
  cnt = cJSON_GetArraySize(statuses);

  for (i=0; i<cnt; i++) {
    weibo = create_weibo_entity_from_json(cJSON_GetArrayItem(statuses, i));
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

PTR_WEIBO_ENTITY get_user_timeline(const char* access_token, const char* uid, int page)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "sd", access_token, page);
  char s[20] = {};
  int i=0, cnt = 0;
  cJSON* root = NULL;
  cJSON* statuses = NULL;
  PTR_WEIBO_ENTITY list_head = NULL;
  PTR_WEIBO_ENTITY weibo = NULL;
  PTR_HTTP_REQUEST request = alloc_http_request(3, 0, 0, 0);
  PTR_HTTP_RESPONSE response = NULL;
  request->params[0].name = "access_token";
  request->params[0].value = access_token;
  request->params[1].name = "page";
  snprintf(s, 20, "%d", page);
  request->params[1].value = s;
  request->params[2].name = "uid";
  request->params[2].value = uid;

  response = https_get(WEIBO_GET_USER_TIMELINE_URL, request);
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

  statuses = cJSON_GetObjectItem(root, "statuses");
  cnt = cJSON_GetArraySize(statuses);

  for (i=0; i<cnt; i++) {
    weibo = create_weibo_entity_from_json(cJSON_GetArrayItem(statuses, i));
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

PTR_WEIBO_ENTITY get_self_timeline(const char* access_token, int page)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "sd", access_token, page);
  char s[20] = {};
  int i=0, cnt = 0;
  cJSON* root = NULL;
  cJSON* statuses = NULL;
  PTR_WEIBO_ENTITY list_head = NULL;
  PTR_WEIBO_ENTITY weibo = NULL;
  PTR_HTTP_REQUEST request = alloc_http_request(2, 0, 0, 0);
  PTR_HTTP_RESPONSE response = NULL;
  request->params[0].name = "access_token";
  request->params[0].value = access_token;
  request->params[1].name = "page";
  snprintf(s, 20, "%d", page);
  request->params[1].value = s;

  response = https_get(WEIBO_GET_USER_TIMELINE_URL, request);
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

  statuses = cJSON_GetObjectItem(root, "statuses");
  cnt = cJSON_GetArraySize(statuses);

  for (i=0; i<cnt; i++) {
    weibo = create_weibo_entity_from_json(cJSON_GetArrayItem(statuses, i));
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
