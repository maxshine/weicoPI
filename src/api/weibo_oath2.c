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
#include "http_util.h"
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
	sprintf(data, "client_id=%s&redirect_uri=%s&response_type=code&display=client", APP_KEY, APP_AUTH_REDIRECT_URL);
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
	debug_log_text_file(INFO, "get_authorize_code", tmp_header_file);
	debug_log_text_file(INFO, "get_authorize_code", tmp_body_file);

	get_header_field_from_file(tmp_header_file, "Location: ", buffer);
	fclose(tmp_body_file);
	fclose(tmp_header_file);
	return buffer+13;
}
