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

#include <limits.h>

#ifndef CONSTANTS_H
#define CONSTANTS_H

/*#define APP_KEY "3773253995"
#define APP_AUTH_REDIRECT_URL "http://"
#define APP_SECRET "fafbd170e8e9dd42f38214c64177f4be"
#define APP_URL "http://twitter2weibo1.appspot.com"*/


#define APP_KEY "4127323521"
#define APP_SECRET "e862110ed024b5938d71ecdee6d6d19f"
#define APP_AUTH_REDIRECT_URL "https://api.weibo.com/oauth2/default.html"
#define APP_URL "http://twitter2weibo1.appspot.com"
#define APP_AUTHORIZE_URL "https://api.weibo.com/oauth2/authorize"
#define APP_FETCH_TOKEN_URL "https://api.weibo.com/oauth2/access_token"
#define APP_REVOKE_TOKEN_URL "https://api.weibo.com/oauth2/revokeoauth2"
#define APP_GET_TOKEN_INFO_URL "https://api.weibo.com/oauth2/get_token_info"

#define WEIBO_CREATE_URL "https://api.weibo.com/2/statuses/update.json"
#define WEIBO_CREATE_UPLOAD_URL "https://upload.api.weibo.com/2/statuses/upload.json"
#define WEIBO_REPOST_URL "https://api.weibo.com/2/statuses/repost.json"
#define WEIBO_DESTROY_URL "https://api.weibo.com/2/statuses/destroy.json"

#define WEIBO_GET_USER_TIMELINE_URL "https://api.weibo.com/2/statuses/user_timeline.json"
#define WEIBO_GET_FRIEND_TIMELINE_URL "https://api.weibo.com/2/statuses/friends_timeline.json"
#define WEIBO_GET_PUBLIC_TIMELINE_URL "https://api.weibo.com/2/statuses/public_timeline.json"
#define WEIBO_GET_HOME_TIMELINE_URL "https://api.weibo.com/2/statuses/home_timeline.json"
#define WEIBO_SHOW_WEIBO_URL "https://api.weibo.com/2/statuses/show.json"
#define WEIBO_SHOW_WEIBO_BATCH_URL "https://api.weibo.com/2/statuses/show_batch.json"
#define WEIBO_GET_USER_TIMELINE_IDS_URL "https://api.weibo.com/2/statuses/user_timeline/ids.json"
#define WEIBO_GET_FRIEND_TIMELINE_IDS_URL "https://api.weibo.com/2/statuses/friends_timeline/ids.json"


#define WEIBO_GET_ACCOUNTID_URL "https://api.weibo.com/2/account/get_uid.json"
#define WEIBO_SHOW_USER_URL "https://api.weibo.com/2/users/show.json"
#define WEIBO_SHOW_USER_RANK_URL "https://api.weibo.com/2/users/show_rank.json"

#define WEIBO_GET_FOLLOWER_URL "https://api.weibo.com/2/friendships/followers.json"
#define WEIBO_GET_FOLLOWER_IDS_URL "https://api.weibo.com/2/friendships/followers/ids.json"
#define WEIBO_GET_FRIEND_URL "https://api.weibo.com/2/friendships/friends.json"
#define WEIBO_CREATE_FRIEND_URL "https://api.weibo.com/2/friendships/create.json"
#define WEIBO_DESTROY_FRIEND_URL "https://api.weibo.com/2/friendships/destroy.json"

#define WEIBO_CREATE_COMMENT_URL "https://api.weibo.com/2/comments/create.json"
#define WEIBO_DESTROY_COMMENT_URL "https://api.weibo.com/2/comments/destroy.json"
#define WEIBO_REPLY_COMMENT_URL "https://api.weibo.com/2/comments/reply.json"
#define WEIBO_SHOW_COMMENTS_URL "https://api.weibo.com/2/comments/show.json"

#define WEIBO_POST_BUFFER_LENGTH 280
#define WEIBO_CONTENT_LIMIT 280

#endif
