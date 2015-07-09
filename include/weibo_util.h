#include "cJSON.h"

#ifndef WEIBO_UTIL_H
#define WEIBO_UTIL_H

PTR_WEIBO_ENTITY init_weibo_entity();
PTR_USER_ENTITY init_user_entity();
PTR_GEO_ENTITY init_geo_entity();
void destroy_user_entity(PTR_USER_ENTITY ptr_user_entity);
void destroy_geo_entity(PTR_GEO_ENTITY ptr_geo_entity);
void destroy_weibo_entity(PTR_WEIBO_ENTITY ptr_weibo_entity);
PTR_USER_ENTITY create_user_from_json(cJSON* json_object);
PTR_GEO_ENTITY create_geo_from_json(cJSON* json_object);
PTR_WEIBO_ENTITY create_weibo_from_json(cJSON* json_object);

#endif
