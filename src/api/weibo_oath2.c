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
#include "debug_util.h"
#include <stdint.h>
#include "cJSON.h"
#include "datatype.h"
#include "http_action.h"
#include "constants.h"
#include "debug_util.h"
#include "weibo_util.h"

char* get_authorize_code(char* buffer) 
{
	CURL *curl = NULL;
	CURLcode ret;
	int wr_error;
	char data[100];
	char *post_data=NULL;

	memset(data, '\0', 100*sizeof(char));
	sprintf(data, "client_id=%s&redirect_uri=%s&response_type=code&display=client", APP_KEY, APP_AUTH_REDIRECT_URL);
	FILE* tmp_body_file = tmpfile();
	FILE* tmp_header_file = tmpfile();

	curl = curl_easy_init();
	post_data = curl_easy_escape(curl, data, strlen(data));
	curl_easy_setopt(curl, CURLOPT_URL, APP_AUTHORIZE_URL);
	curl_easy_setopt(curl, CURLOPT_POST, 1L);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(data));
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, tmp_body_file);
	curl_easy_setopt(curl, CURLOPT_WRITEHEADER, tmp_header_file);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
	ret = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	debug_log_text_file(INFO, "get_authorize_code", tmp_header_file);
	debug_log_text_file(INFO, "get_authorize_code", tmp_body_file);

	get_header_field_from_file(tmp_header_file, "Location: ", buffer);
	fclose(tmp_body_file);
	fclose(tmp_header_file);
	return buffer+13;
}

char* fetch_access_token(const char* code, char* token)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "sp", code, token);
  cJSON* root = NULL;
  PTR_HTTP_REQUEST request = alloc_http_request(5, 0, 0, 0);
  PTR_HTTP_RESPONSE response = NULL;
  request->params[0].name = "client_id";
  request->params[0].value = APP_KEY;
  request->params[1].name = "client_secret";
  request->params[1].value = APP_SECRET;
  request->params[2].name = "grant_type";
  request->params[2].value = "authorization_code";
  request->params[3].name = "code";
  request->params[3].value = code;
  request->params[4].name = "redirect_uri";
  request->params[4].value = APP_AUTH_REDIRECT_URL;

  response = https_post(APP_FETCH_TOKEN_URL, request);
  if (response->status_code != 200) {
    free_http_request(request);
    free_http_response(response);
    return NULL;
  }

  root = cJSON_Parse((char*)(response->body));
  sprintf(token, "%s", cJSON_GetObjectItem(root, "access_token")->valuestring);

  free_http_request(request);
  free_http_response(response);
  cJSON_Delete(root);
  debug_log_exit(FINE, func_name);
  return token;
}

BOOL revoke_access_token(const char* access_token)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "s", access_token);
  cJSON* root = NULL;
  PTR_HTTP_REQUEST request = alloc_http_request(1, 0, 0, 0);
  PTR_HTTP_RESPONSE response = NULL;
  request->params[0].name = "access_token";
  request->params[0].value = access_token;

  response = https_post(APP_REVOKE_TOKEN_URL, request);
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

BOOL get_access_token_info(const char* access_token)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "s", access_token);
  cJSON* root = NULL;
  PTR_HTTP_REQUEST request = alloc_http_request(1, 0, 0, 0);
  PTR_HTTP_RESPONSE response = NULL;
  request->params[0].name = "access_token";
  request->params[0].value = access_token;

  response = https_post(APP_GET_TOKEN_INFO_URL, request);
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
