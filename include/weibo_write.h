#ifndef WEIBO_WRITE_H
#define WEIBO_WRITE_H

int post_new_weibo(const char* access_token, const char* buffer);
int post_new_weibo_upload(const char* access_token, const char* picture_file_name, const char* buffer);
int repost_weibo(const char* access_token, const char* id, const char* buffer, const int isComment);
int destroy_weibo(const char* access_token, const char* id);

#endif
