#include <curl/curl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
#include "http_util.h"
#include "constants.h"
#include "debug_util.h"

long get_userid(const char* access_token)
{
        debug_log_enter(FINE, "get_userid", "s", access_token);

	cJSON* root = NULL;
	long uid = 0L;

        CURL *curl = NULL;
        CURLcode ret;
        char *url = (char*) malloc((20+sizeof(WEIBO_GET_USERID_URL)+strlen(access_token))*sizeof(char));
        char **userdata = (char**) malloc(1*sizeof(char*)); 
        *userdata = (char*) malloc(1*sizeof(char*)); 
        memset(url, '\0', (20+sizeof(WEIBO_GET_USERID_URL)+strlen(access_token))*sizeof(char));
        sprintf(url, "%s?access_token=%s", WEIBO_GET_USERID_URL, access_token);

        curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, userdata);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_body);
        ret = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

	root = cJSON_Parse(*(userdata));
	uid = (long) cJSON_GetObjectItem(root, "uid")->valuedouble;

        debug_log(INFO, "get_userid", *userdata);

        free(url);
	free(*userdata);
	free(userdata);
	cJSON_Delete(root);
        debug_log_exit(FINE, "get_userid");
        return uid;
}

