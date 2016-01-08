#include "weibo_datatype.h"

#ifndef API_COMMENT_H

#define API_COMMENT_H

BOOL create_comment(const char *access_token, const char *weibo_id, const char *comment);
BOOL destroy_comment(const char *access_token, const char *comment_id);
BOOL reply_comment(const char *access_token, const char *weibo_id, const char *comment_id, const char *comment);
PTR_COMMENT_ENTITY show_comments(const char *access_token, const char *weibo_id, int page);

#endif
