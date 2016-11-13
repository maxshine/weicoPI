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

#include <stdint.h>
#include "datatype.h"

#ifndef WEIBO_DATATYPE_H

#define WEIBO_DATATYPE_H

#define MAX_PIC_URL_QTY 9

typedef struct weibo_entity {

  char *created_at;
  uint64_t id;
  char *idstr;
  char *mid;
  char *text;
  char *source;
  int comments_count;
  int reposts_count;
  struct user_entity *user;
  struct weibo_entity *retweeted_status;
  struct weibo_entity *prev;
  struct weibo_entity *next;

} WEIBO_ENTITY, *PTR_WEIBO_ENTITY;

typedef struct comment_entity {
  char *created_at;
  uint64_t id;
  char *idstr;
  char *mid;
  char *text;
  char *source;
  struct user_entity* user;
  struct weibo_entity* status;
  struct comment_entity* prev;
  struct comment_entity* next;
} COMMENT_ENTITY, *PTR_COMMENT_ENTITY;


typedef struct geo_entity {

  char *longitude;
  char *latitude;
  char *city;
  char *province;
  char *city_name;
  char *province_name;
  char *address;
  char *pinyin;
  char *more;

} GEO_ENTITY, *PTR_GEO_ENTITY;

typedef struct user_entity {

  uint64_t id;
  char *idstr;
  char *screen_name;
  char *name;
  char *description;
  char *location;
  char *profile_url;
  char *gender;
  int followers_count;
  int friends_count;
  int statuses_count;
  int favorites_count;
  int online_status;
  BOOL verified;
  struct user_entity *prev;
  struct user_entity *next;

} USER_ENTITY, *PTR_USER_ENTITY;

#endif
