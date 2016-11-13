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

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <locale.h>
#include "gui.h"
#include "gui_alert.h"
#include "gui_popinput.h"
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
const char* AUTH_FILENAME = "/home/pi/weicoPi/config/authorization_code";
uint32_t PAGE = 1;       /* start with page #1 */
uint32_t COMMENT_PAGE = 1;
uint32_t NEXT_FRIEND_CURSOR = 0;
uint32_t PREVIOUS_FRIEND_CURSOR = 0;
uint32_t NEXT_FOLLOWER_CURSOR = 0;
uint32_t PREVIOUS_FOLLOWER_CURSOR = 0;
uint32_t WEIBO_TYPE = 0; /* 0 --  public timeline; 1 -- user timeline; 2 -- friend timeline*/
char COMMENT_WEIBO_ID[30];

void main(int argc, char *argv[])
{
  setlocale(LC_ALL, "");
  PTR_WND weibo_wnd = NULL;
  int i = 1;
  char* p = NULL;
  curl_global_init(CURL_GLOBAL_ALL);
  init_debug_log("weicoPi.log", FINEST);
  ACCESS_TOKEN = get_auth_code(AUTH_FILENAME);
  USERID = (char*)malloc(20*sizeof(char));
  ACCOUNTID = (char*)malloc(20*sizeof(char));
  FRIENDID = (char*)malloc(20*sizeof(char));
  memset((void*)USERID, 0, 20*sizeof(char));
  memset((void*)FRIENDID, 0, 20*sizeof(char));
  memset((void*)ACCOUNTID, 0, 20*sizeof(char));
 
  sprintf(ACCOUNTID, "%ld", get_account_userid(ACCESS_TOKEN));
  PTR_WND_MANAGER wm_mgr = wm_init();
  weibo_wnd = wnd_weibo_create(wm_mgr, NULL, wm_mgr->height-2, wm_mgr->width/2, 0, 0);
  weibo_wnd->show(wm_mgr, weibo_wnd, weibo_wnd->children);
  /*  refresh();*/
  wm_runloop(wm_mgr);
  free((void*)ACCESS_TOKEN);
  free((void*)USERID);
  free((void*)ACCOUNTID);
  free((void*)FRIENDID);
  curl_global_cleanup();
}

