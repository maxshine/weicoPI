#include <stdint.h>

#ifndef DATATYPE_H
#define DATATYPE_H
#define MAX_PIC_URL_QTY 9
struct weibo_entity;
struct geo_entity;
struct user_entity;

typedef struct weibo_entity {

	char *created_at;
	long id;
	char *idstr;
	char *mid;
	char *text;
	char *source; 
	char *pic_urls[MAX_PIC_URL_QTY];
	int pic_urls_qty;
	int comments_count;
	int reposts_count;
	int attitudes_count;
	struct geo_entity *geo;
	struct user_entity *user;	
	struct weibo_entity *retweeted_status;
	struct weibo_entity *prev;
	struct weibo_entity *next;

} WEIBO_ENTITY, *PTR_WEIBO_ENTITY;

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

	long id;
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
	struct user_entity *prev;
	struct user_entity *next;

} USER_ENTITY, *PTR_USER_ENTITY;


typedef enum http_form_type {
  STRING = 0,
  BINARY = 1,
  FILENAME = 2
} ENUM_HTTP_FORM_TYPE;

typedef struct http_header {
  char *name;
  char *value;
  struct http_header* next;
} HTTP_HEADER, *PTR_HTTP_HEADER;

typedef struct http_form {
  char *name;
  char *value;
  ENUM_HTTP_FORM_TYPE type;
  struct http_form* next;
} HTTP_FORM, *PTR_HTTP_FORM;

typedef struct http_param {
  char *name;
  char *value;
  struct http_form* next;
} HTTP_PARAM, *PTR_HTTP_PARAM;

typedef struct http_request {
  PTR_HTTP_PARAM params;
  uint32_t params_qty;
  PTR_HTTP_HEADER headers;
  uint32_t headers_qty;
  PTR_HTTP_FORM form;
  uint32_t form_length;
  void* body;
  uint32_t body_length;
} HTTP_REQUEST, *PTR_HTTP_REQUEST;

typedef struct http_response {
  long status_code;
  PTR_HTTP_HEADER headers;
  uint32_t headers_qty;
  void* body;
  uint32_t body_length;
  char* error;
} HTTP_RESPONSE, *PTR_HTTP_RESPONSE;

#endif
