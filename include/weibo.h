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

#include "datatype.h"
#include "weibo_datatype.h"

#ifndef API_WEIBO_H

#define API_WEIBO_H

BOOL create_weibo_text(const char* access_token, const char* text);
BOOL destroy_weibo(const char* access_token, const char* weibo_id);
BOOL repost_weibo(const char* access_token, const char* text, const char* weibo_id);
BOOL create_weibo_pic(const char* access_token, const char* text, const char* pic_name);

PTR_WEIBO_ENTITY show_single_weibo_byid(const char* access_token, const char* weibo_id);
PTR_WEIBO_ENTITY show_multiple_weibo_byids(const char* access_token, const char** weibo_ids, uint32_t count);

PTR_WEIBO_ENTITY get_user_timeline_byids(const char* access_token, const char* uid, int page);
PTR_WEIBO_ENTITY get_friend_timeline_byids(const char* access_token, int page);

PTR_WEIBO_ENTITY get_public_timeline(const char* access_token, int page);
PTR_WEIBO_ENTITY get_friend_timeline(const char* access_token, int page);
PTR_WEIBO_ENTITY get_home_timeline(const char* access_token, int page);
PTR_WEIBO_ENTITY get_user_timeline(const char* access_token, const char* uid, int page);
PTR_WEIBO_ENTITY get_self_timeline(const char* access_token, int page);

#endif
