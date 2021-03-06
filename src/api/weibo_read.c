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
#include "cJSON.h"
#include "http_util.h"
#include "constants.h"
#include "debug_util.h"
#include "datatype.h"
#include "weibo_util.h"

PTR_WEIBO_ENTITY get_user_timeline(const char* access_token, const char* uid, const char* page, const char* since_id, const char* count)
{
        debug_log_enter(FINE, "get_user_timeline", "sssss", access_token, uid, page, since_id, count);

	cJSON *weibo_json_root = NULL;
	cJSON *weibo_json_array = NULL;
	int weibo_qty = 0;
	int i = 0;
	PTR_WEIBO_ENTITY root = NULL;
	PTR_WEIBO_ENTITY tail = NULL;
	PTR_WEIBO_ENTITY entity = NULL;
        CURL *curl = NULL;
        CURLcode ret;
	int url_buffer_length = 40+strlen(access_token)+strlen(uid)+strlen(page)+strlen(since_id)+strlen(count)+sizeof(WEIBO_GET_USER_TIMELINE_URL);
        char *url = (char*) malloc(url_buffer_length*sizeof(char));
        char **userdata = (char**) malloc(1*sizeof(char*)); 

        *userdata = (char*) malloc(1*sizeof(char*)); 
        memset(url, '\0', url_buffer_length*sizeof(char));
        sprintf(url, "%s?access_token=%s&uid=%s&page=%s&since_id=%s&count=%s", WEIBO_GET_USER_TIMELINE_URL, access_token, uid, page, since_id, count);

        curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, userdata);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_body);
        ret = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

	weibo_json_root = cJSON_Parse(*userdata);
	weibo_json_array = cJSON_GetObjectItem(weibo_json_root, "statuses");
	weibo_qty = cJSON_GetArraySize(weibo_json_array);
	i = 0;
	while(i++ < weibo_qty)
	{
		entity = create_weibo_from_json(cJSON_GetArrayItem(weibo_json_array, i-1));
		if(i-1 == 0)
		{
			root = entity;
		}
		else
		{
			tail->next = entity;
			entity->prev = tail;
			tail = entity;
		}
	}
        debug_log(INFO, "get_user_timeline", "User Timeline is got");
        debug_log(FINEST, "get_user_timeline", *userdata);

	cJSON_Delete(weibo_json_root);
        free(url);
	free(*userdata);
	free(userdata);
        debug_log_exit(FINE, "get_user_timeline");
        return root;
}

PTR_WEIBO_ENTITY get_friends_timeline(const char* access_token, const char* page, const char* since_id, const char* count)
{
        debug_log_enter(FINE, "get_friends_timeline", "ssss", access_token, page, since_id, count);

	cJSON *weibo_json_root = NULL;
	cJSON *weibo_json_array = NULL;
	int weibo_qty = 0;
	int i = 0;
	PTR_WEIBO_ENTITY root = NULL;
	PTR_WEIBO_ENTITY tail = NULL;
	PTR_WEIBO_ENTITY entity = NULL;
        CURL *curl = NULL;
        CURLcode ret;
	int url_buffer_length = 40+strlen(access_token)+strlen(page)+strlen(since_id)+strlen(count)+sizeof(WEIBO_GET_FRIENDS_TIMELINE_URL);
        char *url = (char*) malloc(url_buffer_length*sizeof(char));
        char **userdata = (char**) malloc(1*sizeof(char*)); 

        *userdata = (char*) malloc(1*sizeof(char*)); 
        memset(url, '\0', url_buffer_length*sizeof(char));
        sprintf(url, "%s?access_token=%s&page=%s&since_id=%s&count=%s", WEIBO_GET_FRIENDS_TIMELINE_URL, access_token, page, since_id, count);

        curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, userdata);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_body);
        ret = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

	weibo_json_root = cJSON_Parse(*userdata);
	weibo_json_array = cJSON_GetObjectItem(weibo_json_root, "statuses");
	weibo_qty = cJSON_GetArraySize(weibo_json_array);
	i = 0;
	while(i++ < weibo_qty)
	{
		entity = create_weibo_from_json(cJSON_GetArrayItem(weibo_json_array, i-1));
		if(i-1 == 0)
		{
			root = entity;
		}
		else
		{
			tail->next = entity;
			entity->prev = tail;
			tail = entity;
		}
	}
        debug_log(INFO, "get_friends_timeline", "Friends Timeline is got");
        debug_log(FINEST, "get_friends_timeline", *userdata);

	cJSON_Delete(weibo_json_root);
        free(url);
	free(*userdata);
	free(userdata);
        debug_log_exit(FINE, "get_friends_timeline");
        return root;
}

PTR_WEIBO_ENTITY get_home_timeline(const char* access_token, const char* page, const char* since_id, const char* count)
{
        debug_log_enter(FINE, "get_home_timeline", "ssss", access_token, page, since_id, count);

	cJSON *weibo_json_root = NULL;
	cJSON *weibo_json_array = NULL;
	int weibo_qty = 0;
	int i = 0;
	PTR_WEIBO_ENTITY root = NULL;
	PTR_WEIBO_ENTITY tail = NULL;
	PTR_WEIBO_ENTITY entity = NULL;
        CURL *curl = NULL;
        CURLcode ret;
	int url_buffer_length = 40+strlen(access_token)+strlen(page)+strlen(since_id)+strlen(count)+sizeof(WEIBO_GET_HOME_TIMELINE_URL);
        char *url = (char*) malloc(url_buffer_length*sizeof(char));
        char **userdata = (char**) malloc(1*sizeof(char*)); 

        *userdata = (char*) malloc(1*sizeof(char*)); 
        memset(url, '\0', url_buffer_length*sizeof(char));
        sprintf(url, "%s?access_token=%s&page=%s&since_id=%s&count=%s", WEIBO_GET_HOME_TIMELINE_URL, access_token, page, since_id, count);

        curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, userdata);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_body);
        ret = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

	weibo_json_root = cJSON_Parse(*userdata);
	weibo_json_array = cJSON_GetObjectItem(weibo_json_root, "statuses");
	weibo_qty = cJSON_GetArraySize(weibo_json_array);
	i = 0;
	while(i++ < weibo_qty)
	{
		entity = create_weibo_from_json(cJSON_GetArrayItem(weibo_json_array, i-1));
		if(i-1 == 0)
		{
			root = entity;
		}
		else
		{
			tail->next = entity;
			entity->prev = tail;
			tail = entity;
		}
	}
        debug_log(INFO, "get_home_timeline", "Home Timeline is got");
        debug_log(FINEST, "get_home_timeline", *userdata);

	cJSON_Delete(weibo_json_root);
        free(url);
	free(*userdata);
	free(userdata);
        debug_log_exit(FINE, "get_home_timeline");
        return root;
}

PTR_WEIBO_ENTITY get_public_timeline(const char* access_token, const char* page, const char* since_id, const char* count)
{
        debug_log_enter(FINE, "get_public_timeline", "ssss", access_token, page, since_id, count);

	cJSON *weibo_json_root = NULL;
	cJSON *weibo_json_array = NULL;
	int weibo_qty = 0;
	int i = 0;
	PTR_WEIBO_ENTITY root = NULL;
	PTR_WEIBO_ENTITY tail = NULL;
	PTR_WEIBO_ENTITY entity = NULL;
        CURL *curl = NULL;
        CURLcode ret;
	int url_buffer_length = 40+strlen(access_token)+strlen(page)+strlen(since_id)+strlen(count)+sizeof(WEIBO_GET_USER_TIMELINE_URL);
        char *url = (char*) malloc(url_buffer_length*sizeof(char));
        char **userdata = (char**) malloc(1*sizeof(char*)); 

        *userdata = (char*) malloc(1*sizeof(char*)); 
        memset(url, '\0', url_buffer_length*sizeof(char));
        sprintf(url, "%s?access_token=%s&page=%s&since_id=%s&count=%s", WEIBO_GET_PUBLIC_TIMELINE_URL, access_token, page, since_id, count);

        curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, userdata);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_body);
        ret = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

	weibo_json_root = cJSON_Parse(*userdata);
	weibo_json_array = cJSON_GetObjectItem(weibo_json_root, "statuses");
	weibo_qty = cJSON_GetArraySize(weibo_json_array);
	i = 0;
	while(i++ < weibo_qty)
	{
		entity = create_weibo_from_json(cJSON_GetArrayItem(weibo_json_array, i-1));
		if(i-1 == 0)
		{
			root = entity;
		}
		else
		{
			tail->next = entity;
			entity->prev = tail;
			tail = entity;
		}
	}
        debug_log(INFO, "get_public_timeline", "Public Timeline is got");
        debug_log(FINEST, "get_public_timeline", *userdata);

	cJSON_Delete(weibo_json_root);
        free(url);
	free(*userdata);
	free(userdata);
        debug_log_exit(FINE, "get_public_timeline");
        return root;
}
