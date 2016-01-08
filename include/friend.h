#include "weibo_datatype.h"

#ifndef API_FRIEND_H

#define API_FRIEND_H

PTR_USER_ENTITY get_friends_byid(const char* access_token, const char* uid, int cursor);
PTR_USER_ENTITY get_friends_byname(const char* access_token, const char* screen_name, int cursor);
PTR_USER_ENTITY get_followers_byid(const char* access_token, const char* uid, int cursor);
PTR_USER_ENTITY get_followers_byname(const char* access_token, const char* screen_name, int cursor);

#endif
