#ifndef DATATYPE_H
#define DATATYPE_H 1
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

#endif
