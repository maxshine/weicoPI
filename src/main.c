#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "weibo_oath2.h"
#include "weibo_write.h"
#include "user_read.h"
#include "init.h"
#include "constants.h"
#include "debug_util.h"

const char* AUTHCODE = NULL;
char* USERID = NULL;

void main(int argc, char *argv[])
{
	curl_global_init(CURL_GLOBAL_ALL);
	char *weibo= (char*)malloc((size_t)(WEIBO_POST_BUFFER_LENGTH*sizeof(char)));
	memset(weibo, 0, WEIBO_POST_BUFFER_LENGTH*sizeof(char));
	sprintf(weibo, "%s", argv[1]);
	init_debug_log("weicoPi.log", FINEST);
	AUTHCODE = get_auth_code("/home/pi/weicoPi/config/authorization_code");
	USERID = (char*)malloc(20*sizeof(char));
	memset(USERID, 0, 20*sizeof(char));

/*	if(argc == 2) {
		post_new_weibo(AUTHCODE, weibo);
	}
	else {
		post_new_weibo_upload(AUTHCODE, argv[2], weibo);
	}

	sprintf(USERID, "%d", get_userid(AUTHCODE));
	get_user_timeline(AUTHCODE, USERID, "1", "0", "1");
	get_friends_timeline(AUTHCODE, "1", "0", "1");
	get_public_timeline(AUTHCODE, "1", "0", "1");
	get_home_timeline(AUTHCODE, "1", "0", "1");*/
	deinit_debug_log();
	free((char*)AUTHCODE); 
	free((char*)USERID); 
	free((char*)weibo); 
	curl_global_cleanup();
}

