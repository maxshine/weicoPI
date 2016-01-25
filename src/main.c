#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "gui.h"
#include "gui_datatype.h"
#include "account.h"
#include "weibo.h"
#include "friend.h"
#include "user.h"
#include "init.h"
#include "constants.h"
#include "debug_util.h"

char* ACCESS_TOKEN = NULL;
char* USERID = NULL;
char* ACCOUNTID = NULL;
char* FRIENDID = NULL;
char* AUTH_FILENAME = "/home/pi/weicoPi/config/authorization_code";
uint32_t PAGE = 0;
uint32_t WEIBO_TYPE = 0; /* 0 --  public timeline; 1 -- user; 2 -- friend timeline*/

void main(int argc, char *argv[])
{
  PTR_WND weibo_wnd = NULL;
  
  curl_global_init(CURL_GLOBAL_ALL);
  init_debug_log("weicoPi.log", FINEST);
  ACCESS_TOKEN = get_auth_code(AUTH_FILENAME);
  USERID = (char*)malloc(20*sizeof(char));
  ACCOUNTID = (char*)malloc(20*sizeof(char));
  FRIENDID = (char*)malloc(20*sizeof(char));
  memset((void*)USERID, 0, 20*sizeof(char));
  memset((void*)FRIENDID, 0, 20*sizeof(char));
  memset((void*)ACCOUNTID, 0, 20*sizeof(char));
 
  sprintf(USERID, "%ld", get_account_userid(ACCESS_TOKEN));
  PTR_WND_MANAGER wm_mgr = wm_init();
  weibo_wnd = wnd_init(wm_mgr, NULL, "weibo", 0, 0, wm_mgr->height, wm_mgr->width/2);
  weibo_wnd->handler = wnd_weibo_handler;
  weibo_wnd->initializer = wnd_weibo_initializer;
  weibo_wnd->destroyer = wnd_weibo_destroyer;
  weibo_wnd->initializer(weibo_wnd);
  refresh();
  wm_runloop(wm_mgr);
  deinit_debug_log();
  free((void*)ACCESS_TOKEN);
  free((void*)USERID);
  free((void*)ACCOUNTID);
  free((void*)FRIENDID);
  curl_global_cleanup();
}

