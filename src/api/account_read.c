#include <curl/curl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
#include "datatype.h"
#include "http_action.h"
#include "constants.h"
#include "debug_util.h"

long get_account_userid(const char* access_token)
{
        debug_log_enter(FINE, __func__, "s", access_token);

	cJSON* root = NULL;
	long uid = 0L;

	PTR_HTTP_REQUEST request = alloc_http_request(1, 0, 1, 0);
	PTR_HTTP_RESPONSE response = NULL;
	request->params[0].name = "access_token";
	request->params[0].value = access_token;
	response = https_get(WEIBO_GET_USERID_URL, request);

	root = cJSON_Parse((char*)(response->body));
	uid = (long) cJSON_GetObjectItem(root, "uid")->valuedouble;

	cJSON_Delete(root);
        debug_log_exit(FINE, __func__);
        return uid;
}

