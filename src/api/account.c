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
	  return NULL;
	}

	root = cJSON_Parse((char*)(response->body));
	uid = (uint64_t) cJSON_GetObjectItem(root, "uid")->valuedouble;

	cJSON_Delete(root);
        debug_log_exit(FINE, func_name);
        return uid;
}
