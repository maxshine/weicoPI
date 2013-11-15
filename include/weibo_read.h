#ifndef WEIBO_READ_H
#define WEIBO_READ_H 1

BOOL get_user_timeline(const char* access_token, const char* uid, const char* since_id, const char* count);

#endif
