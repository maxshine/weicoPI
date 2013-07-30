#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "weibo_oath2.h"
#include "weibo_write.h"
#include "init.h"
#include "constants.h"

const char* AUTHCODE = NULL;

void main(int argc, char *argv[])
{
	curl_global_init(CURL_GLOBAL_ALL);
	char *weibo= (char*)malloc((size_t)(WEIBO_POST_BUFFER_LENGTH*sizeof(char)));
	memset(weibo, 0, WEIBO_POST_BUFFER_LENGTH*sizeof(char));
	sprintf(weibo, "Picture upload interface Test");
	AUTHCODE = get_auth_code("/home/pi/weicoPi/config/authorization_code");
/*	post_new_weibo(AUTHCODE, weibo);*/
	post_new_weibo_upload(AUTHCODE, "/home/pi/ocr_pi.png", weibo);
	puts(AUTHCODE);
	free((char*)AUTHCODE); 
	free((char*)weibo); 
	curl_global_cleanup();
}

