#include <curl/curl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "header_util.h"
#include "constants.h"
#include "debug_util.h"

char* get_authorize_code(char* buffer) 
{
	CURL *curl = NULL;
	CURLcode ret;
	int wr_error;
	char data[100];
	char *post_data=NULL;

	memset(data, '\0', 100*sizeof(char));
	sprintf(data, "client_id=%s&redirect_uri=%s&display=client&response_type=code&display=client", APP_KEY, APP_AUTH_REDIRECT_URL);
	FILE* tmp_body_file = tmpfile();
	FILE* tmp_header_file = tmpfile();

	curl = curl_easy_init();
	post_data = curl_easy_escape(curl, data, strlen(data));
	curl_easy_setopt(curl, CURLOPT_URL, INIT_AUTHORIZE_URL);
	curl_easy_setopt(curl, CURLOPT_POST, 1L);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(data));
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, tmp_body_file);
	curl_easy_setopt(curl, CURLOPT_WRITEHEADER, tmp_header_file);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
	ret = curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	get_header_field_from_file(tmp_header_file, "Location: ", buffer);
	fclose(tmp_body_file);
	fclose(tmp_header_file);
	return buffer+13;
}
