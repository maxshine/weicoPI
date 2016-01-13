#include "datatype.h"

#ifndef API_WEIBO_H

#define API_WEIBO_H

BOOL create_weibo_text(const char* access_token, const char* text);
BOOL destroy_weibo(const char* access_token, const char* weibo_id);
BOOL repost_weibo(const char* access_token, const char* text, const char* weibo_id);
BOOL create_weibo_pic(const char* access_token, const char* text, const char* pic_name);
PTR_WEIBO_ENTITY show_single_weibo_byid(const char* access_token, const char* weibo_id);
PTR_WEIBO_ENTITY get_user_timeline_byid(const char* access_token, const char* uid, int page);
PTR_WEIBO_ENTITY get_user_timeline_byname(const char* access_token, const char* name, int page);
PTR_WEIBO_ENTITY get_friend_timeline(const char* access_token, int page);
PTR_WEIBO_ENTITY get_public_timeline(const char* access_token, int page);


#endif
