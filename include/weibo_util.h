#include "cJSON.h"
#include "datatype.h"
#include "weibo_datatype.h"


#ifndef WEIBO_UTIL_H

#define WEIBO_UTIL_H

BOOL check_api_error(cJSON* json);
PTR_COMMENT_ENTITY init_comment_entity();
PTR_WEIBO_ENTITY init_weibo_entity();
PTR_USER_ENTITY init_user_entity();
PTR_GEO_ENTITY init_geo_entity();

void destroy_comment_entity(PTR_COMMENT_ENTITY ptr_comment_entity);
void destroy_user_entity(PTR_USER_ENTITY ptr_user_entity);
void destroy_geo_entity(PTR_GEO_ENTITY ptr_geo_entity);
void destroy_weibo_entity(PTR_WEIBO_ENTITY ptr_weibo_entity);
void destroy_weibo_entity_list(PTR_WEIBO_ENTITY ptr_weibo_entity);
void destroy_comment_entity_list(PTR_COMMENT_ENTITY ptr_comment_entity);
void destroy_user_entity_list(PTR_USER_ENTITY ptr_user_entity);

PTR_COMMENT_ENTITY create_comment_entity_from_json(cJSON* json_object);
PTR_USER_ENTITY create_user_entity_from_json(cJSON* json_object);
PTR_GEO_ENTITY create_geo_entity_from_json(cJSON* json_object);
PTR_WEIBO_ENTITY create_weibo_entity_from_json(cJSON* json_object);

#endif
