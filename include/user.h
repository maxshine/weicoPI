#include "weibo_datatype.h"

#ifndef API_USER_H
#define API_USER_H

PTR_USER_ENTITY show_user_byid(const char* access_token, const char* uid);
PTR_USER_ENTITY show_user_byname(const char* access_token, const char* screen_name);
int show_user_rank(const char* access_token, const char* uid);

#endif
