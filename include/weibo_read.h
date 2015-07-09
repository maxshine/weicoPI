#ifndef WEIBO_READ_H
#define WEIBO_READ_H

PTR_WEIBO_ENTITY get_user_timeline(const char* access_token, const char* uid, const char* page, const char* since_id, const char* count);
PTR_WEIBO_ENTITY get_friends_timeline(const char* access_token, const char* page, const char* since_id, const char* count);
PTR_WEIBO_ENTITY get_home_timeline(const char* access_token, const char* page, const char* since_id, const char* count);
PTR_WEIBO_ENTITY get_public_timeline(const char* access_token, const char* page, const char* since_id, const char* count);

#endif
