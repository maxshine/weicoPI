#ifndef WEIBO_WRITE_H
#define WEIBO_WRITE_H 1

int post_new_weibo(const char* access_token, char* buffer);
int post_new_weibo_upload(const char* access_token, const char* picture_file_name, char* buffer);
int repost_weibo(const char* access_token, const char* id, char* buffer, int isComment);
int destroy_weibo(const char* access_token, char* id);

#endif
