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
#include "constants.h"
#include "debug_util.h"

BOOL post_new_weibo(const char* access_token, const char* buffer) 
{
	debug_log_enter(FINE, "post_new_weibo", "ss", access_token, buffer);

	CURL *curl = NULL;
	CURLcode ret;
	char *data = (char*) malloc((50+WEIBO_POST_BUFFER_LENGTH)*sizeof(char));

	memset(data, '\0', (50+WEIBO_POST_BUFFER_LENGTH)*sizeof(char));
	sprintf(data, "access_token=%s&visible=0&status=%s", access_token, buffer);
	FILE* tmp_body_file = tmpfile();
	FILE* tmp_header_file = tmpfile();

	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, WEIBO_POST_URL);
	curl_easy_setopt(curl, CURLOPT_POST, 1L);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(data));
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, tmp_body_file);
	curl_easy_setopt(curl, CURLOPT_WRITEHEADER, tmp_header_file);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
	ret = curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	debug_log(INFO, "post_new_weibo", "Weibo Posted");
	debug_log_text_file(FINEST, "post_new_weibo", tmp_header_file);
	debug_log_text_file(FINEST, "post_new_weibo", tmp_body_file);
	
	fclose(tmp_body_file);
	fclose(tmp_header_file);
	free(data);
	debug_log_exit(FINE, "post_new_weibo");
	return TRUE;
}

BOOL post_new_weibo_upload(const char* access_token, const char* picture_file_name, const char* buffer)
{
	debug_log_enter(FINE, "post_new_weibo_upload", "sss", access_token, picture_file_name, buffer);

        CURL *curl = NULL;
        CURLcode ret;
	struct curl_httppost *post = NULL;
	struct curl_httppost *last = NULL;

        FILE* tmp_body_file = tmpfile();
        FILE* tmp_header_file = tmpfile();
	
	curl_formadd(&post, &last, CURLFORM_PTRNAME, "access_token", CURLFORM_PTRCONTENTS, access_token, CURLFORM_END); 
	curl_formadd(&post, &last, CURLFORM_PTRNAME, "status", CURLFORM_PTRCONTENTS, buffer, CURLFORM_END); 
	curl_formadd(&post, &last, CURLFORM_PTRNAME, "pic", CURLFORM_FILE, picture_file_name, CURLFORM_END); 

        curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL, WEIBO_UPLOAD_URL);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, tmp_body_file);
        curl_easy_setopt(curl, CURLOPT_WRITEHEADER, tmp_header_file);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        ret = curl_easy_perform(curl);
	curl_formfree(post);
        curl_easy_cleanup(curl);

        fclose(tmp_body_file);
        fclose(tmp_header_file);
	debug_log_exit(FINE, "post_new_weibo_upload"); 
        return TRUE;
}

int repost_weibo(const char* access_token, const char* id, const char* buffer, const int isComment)
{
	debug_log_enter(FINE, "repost_weibo", "sssd", access_token, id, buffer, isComment);
        CURL *curl = NULL;
        CURLcode ret;
        char *data = (char*) malloc((50+WEIBO_POST_BUFFER_LENGTH)*sizeof(char));

        memset(data, '\0', (50+WEIBO_POST_BUFFER_LENGTH)*sizeof(char));

	if(isComment) {
	        sprintf(data, "access_token=%s&id=%s&status=%s&is_comment=3", access_token, id, buffer);
	} else {
		sprintf(data, "access_token=%s&id=%s&status=%s", access_token, id, buffer);
	}

        FILE* tmp_body_file = tmpfile();
        FILE* tmp_header_file = tmpfile();

        curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL, WEIBO_REPOST_URL);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(data));
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, tmp_body_file);
        curl_easy_setopt(curl, CURLOPT_WRITEHEADER, tmp_header_file);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        ret = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        fclose(tmp_body_file);
        fclose(tmp_header_file);
        free(data);
	debug_log_exit(FINE, "repost_weibo");
        return TRUE;
}

int destroy_weibo(const char* access_token, const char* id)
{
	debug_log_enter(FINE, "destroy_weibo", "ss", access_token, id);
        CURL *curl = NULL;
        CURLcode ret;
        char *data = (char*) malloc((50+WEIBO_POST_BUFFER_LENGTH)*sizeof(char));

        memset(data, '\0', (50+WEIBO_POST_BUFFER_LENGTH)*sizeof(char));
        sprintf(data, "access_token=%s&id=%s", access_token, id);
        FILE* tmp_body_file = tmpfile();
        FILE* tmp_header_file = tmpfile();

        curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL, WEIBO_POST_URL);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(data));
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, tmp_body_file);
        curl_easy_setopt(curl, CURLOPT_WRITEHEADER, tmp_header_file);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        ret = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        fclose(tmp_body_file);
        fclose(tmp_header_file);
        free(data);
	debug_log_exit(FINE, "destroy_weibo");
        return TRUE;
}
