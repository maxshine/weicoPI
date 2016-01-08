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
  char *mid;
  char *text;
  char *source;
  struct user_entity* user;
  struct weibo_entity* status;
} COMMENT_ENTITY, PTR_COMMENT_ENTITY;


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
