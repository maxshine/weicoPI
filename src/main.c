#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "weibo_oath2.h"
#include "weibo_write.h"
#include "account.h"
#include "init.h"
#include "constants.h"
#include "debug_util.h"

const char* ACCESS_TOKEN = NULL;
const char* USERID = NULL;
const char* ACCOUNTID = NULL;
const char* FRIENDID = NULL;
uint32_t PAGE = 0;
uint32_t WEIBO_TYPE = 0; /* 0 --  public timeline; 1 -- user; 2 -- friend timeline*/

void main(int argc, char *argv[])
{
	curl_global_init(CURL_GLOBAL_ALL);
	char *weibo= (char*)malloc((size_t)(WEIBO_POST_BUFFER_LENGTH*sizeof(char)));
	memset(weibo, 0, WEIBO_POST_BUFFER_LENGTH*sizeof(char));
	sprintf(weibo, "%s", argv[1]);
	init_debug_log("weicoPi.log", FINEST);
	ACCESS_TOKEN = get_auth_code("/home/pi/weicoPi/config/authorization_code");
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
	sprintf(USERID, "%ld", get_account_userid(AUTHCODE));
	printf(USERID);
	deinit_debug_log();
	free((char*)AUTHCODE); 
	free((char*)USERID); 
	free((char*)weibo); 
	curl_global_cleanup();
}

