#include <limits.h>

#ifndef CONSTANTS_H
#define CONSTANTS_H 1

/*#define APP_KEY "3773253995"
#define APP_AUTH_REDIRECT_URL "http://"
#define APP_SECRET "fafbd170e8e9dd42f38214c64177f4be"
#define APP_URL "http://twitter2weibo1.appspot.com"*/

#define APP_KEY "4127323521"
#define APP_AUTH_REDIRECT_URL "http://"
#define APP_SECRET "e862110ed024b5938d71ecdee6d6d19f"
#define APP_URL "http://twitter2weibo1.appspot.com"

#define INIT_AUTHORIZE_URL "https://api.weibo.com/oauth2/authorize"
#define WEIBO_POST_URL "https://api.weibo.com/2/statuses/update.json"
#define WEIBO_UPLOAD_URL "https://upload.api.weibo.com/2/statuses/upload.json"
#define WEIBO_REPOST_URL "https://api.weibo.com/2/statuses/repost.jason"
#define WEIBO_DESTROY_URL "https://api.weibo.com/2/statuses/destroy.json"

#define WEIBO_GET_USER_TIMELINE_URL "https://api.weibo.com/2/statuses/user_timeline.json"
#define WEIBO_GET_FRIENDS_TIMELINE_URL "https://api.weibo.com/2/statuses/friends_timeline.json"
#define WEIBO_GET_PUBLIC_TIMELINE_URL "https://api.weibo.com/2/statuses/public_timeline.json"
#define WEIBO_GET_HOME_TIMELINE_URL "https://api.weibo.com/2/statuses/home_timeline.json"

#define WEIBO_GET_USERID_URL "https://api.weibo.com/2/account/get_uid.json"

#define WEIBO_POST_BUFFER_LENGTH 280

#endif
