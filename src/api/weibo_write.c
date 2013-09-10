#include <curl/curl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "constants.h"
#include "debug_util.h"

BOOL post_new_weibo(const char* access_token, char* buffer) 
{
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
	return TRUE;
}

BOOL post_new_weibo_upload(const char* access_token, const char* picture_file_name, char* buffer)
{
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
        return TRUE;
}

int repost_weibo(const char* access_token, const char* id, char* buffer, int isComment)
{
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
        return TRUE;
}

int destroy_weibo(const char* access_token, char* id)
{
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
        return TRUE;
}
