#define _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "weibo_datatype.h"
#include "datatype.h"
#include "constants.h"
#include "cJSON.h"
#include "debug_util.h"

PTR_COMMENT_ENTITY init_comment_entity()
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  int i = 0;
  PTR_COMMENT_ENTITY ptr_comment_entity = (PTR_COMMENT_ENTITY) malloc(sizeof(COMMENT_ENTITY));
  if(ptr_comment_entity != NULL)
    {
      ptr_comment_entity->created_at = NULL;
      ptr_comment_entity->id = 0L;
      ptr_comment_entity->idstr = NULL;
      ptr_comment_entity->mid = NULL;
      ptr_comment_entity->text = NULL;
      ptr_comment_entity->source = NULL;
      ptr_comment_entity->user = NULL;
      ptr_comment_entity->status = NULL;
      ptr_comment_entity->prev = NULL;
      ptr_comment_entity->next = NULL;
    }
  debug_log_exit(FINE, func_name);
  return ptr_comment_entity;
}


PTR_WEIBO_ENTITY init_weibo_entity()
{
        const char *func_name = __func__;
	debug_log_enter(FINE, func_name, NULL);
	int i = 0;
	PTR_WEIBO_ENTITY ptr_weibo_entity = (PTR_WEIBO_ENTITY) malloc(sizeof(WEIBO_ENTITY));
	if(ptr_weibo_entity != NULL)
	{
		ptr_weibo_entity->created_at = NULL;
		ptr_weibo_entity->id = 0L;
		ptr_weibo_entity->idstr = NULL;
		ptr_weibo_entity->mid = NULL;
		ptr_weibo_entity->text = NULL;
		ptr_weibo_entity->source = NULL;
		ptr_weibo_entity->comments_count = 0;
		ptr_weibo_entity->reposts_count = 0;
		ptr_weibo_entity->user = NULL;	
		ptr_weibo_entity->retweeted_status = NULL;
		ptr_weibo_entity->prev = NULL;
		ptr_weibo_entity->next = NULL;

	}
	debug_log_exit(FINE, func_name);
	return ptr_weibo_entity;
}

PTR_USER_ENTITY init_user_entity() 
{
	debug_log_enter(FINE, __func__, NULL);
	PTR_USER_ENTITY ptr_user_entity = (PTR_USER_ENTITY) malloc(sizeof(USER_ENTITY));
	if(ptr_user_entity != NULL)
	{
		ptr_user_entity->idstr = NULL;
		ptr_user_entity->screen_name = NULL;
		ptr_user_entity->name = NULL;
		ptr_user_entity->description = NULL;
		ptr_user_entity->location = NULL;
		ptr_user_entity->profile_url = NULL;
		ptr_user_entity->gender = NULL;
		ptr_user_entity->id = 0L;
		ptr_user_entity->followers_count = 0;
		ptr_user_entity->friends_count = 0;
		ptr_user_entity->statuses_count = 0;
		ptr_user_entity->online_status = 0;
		ptr_user_entity->favorites_count = 0;
		ptr_user_entity->verified = 0;
		ptr_user_entity->prev = NULL;
		ptr_user_entity->next = NULL;
	}
	debug_log_exit(FINE, __func__);
	return ptr_user_entity;
}

PTR_GEO_ENTITY init_geo_entity() 
{
	debug_log_enter(FINE, "init_geo_entity", NULL);
	PTR_GEO_ENTITY ptr_geo_entity = (PTR_GEO_ENTITY) malloc(sizeof(GEO_ENTITY));
	if(ptr_geo_entity != NULL)
	{
		ptr_geo_entity->longitude = NULL;
		ptr_geo_entity->latitude = NULL;
		ptr_geo_entity->city = NULL;
		ptr_geo_entity->province = NULL;
		ptr_geo_entity->city_name = NULL;
		ptr_geo_entity->province_name = NULL;
		ptr_geo_entity->pinyin = NULL;
		ptr_geo_entity->more = NULL;
		ptr_geo_entity->address = NULL;
	}
	debug_log_exit(FINE, "init_geo_entity");
	return ptr_geo_entity;

}


void destroy_user_entity(PTR_USER_ENTITY ptr_user_entity)
{
  debug_log_enter(FINE, __func__, "p", ptr_user_entity);
  if(ptr_user_entity->idstr != NULL)
    {
      free(ptr_user_entity->idstr);
      ptr_user_entity->idstr = NULL;
    }
  if(ptr_user_entity->screen_name != NULL)
    {
      free(ptr_user_entity->screen_name);
      ptr_user_entity->screen_name = NULL;
    }
  if(ptr_user_entity->name != NULL)
    {
      free(ptr_user_entity->name);
      ptr_user_entity->name = NULL;
    }
  if(ptr_user_entity->description != NULL)
    {
      free(ptr_user_entity->description);
      ptr_user_entity->description = NULL;
    }
  if(ptr_user_entity->location != NULL)
    {
      free(ptr_user_entity->location);
      ptr_user_entity->location = NULL;
    }
  if(ptr_user_entity->gender != NULL)
    {
      free(ptr_user_entity->gender);
      ptr_user_entity->gender = NULL;
    }
  if(ptr_user_entity->profile_url != NULL)
    {
      free(ptr_user_entity->profile_url);
      ptr_user_entity->profile_url = NULL;
    }
  free(ptr_user_entity);
  debug_log_exit(FINE, __func__);
}

void destroy_weibo_entity(PTR_WEIBO_ENTITY ptr_weibo_entity)
{
  const char *func_name = __func__;
  debug_log_enter(FINE, func_name, "p", ptr_weibo_entity);
  
  int i = 0;
  
  if(ptr_weibo_entity->created_at != NULL)
    {
      free(ptr_weibo_entity->created_at);
      ptr_weibo_entity->created_at = NULL;
    }
  if(ptr_weibo_entity->idstr != NULL)
    {
      free(ptr_weibo_entity->idstr);
      ptr_weibo_entity->idstr = NULL;
    }
  if(ptr_weibo_entity->mid != NULL)
    {
      free(ptr_weibo_entity->mid);
      ptr_weibo_entity->mid = NULL;
    }
  if(ptr_weibo_entity->text != NULL)
    {
      free(ptr_weibo_entity->text);
      ptr_weibo_entity->text = NULL;
    }
  if(ptr_weibo_entity->source != NULL)
    {
      free(ptr_weibo_entity->source);
      ptr_weibo_entity->source = NULL;
    }
  if(ptr_weibo_entity->user != NULL)
    {
      destroy_user_entity(ptr_weibo_entity->user);
      ptr_weibo_entity->user = NULL;
    }
  if(ptr_weibo_entity->retweeted_status != NULL)
    {
      destroy_weibo_entity(ptr_weibo_entity->retweeted_status);
      ptr_weibo_entity->retweeted_status = NULL;
    }
  free(ptr_weibo_entity);	
  debug_log_exit(FINE, func_name);
}

void destroy_comment_entity(PTR_COMMENT_ENTITY ptr_comment_entity)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "p", ptr_comment_entity);

  if(ptr_comment_entity->created_at != NULL)
    {
      free(ptr_comment_entity->created_at);
      ptr_comment_entity->created_at = NULL;
    }
  if(ptr_comment_entity->text != NULL)
    {
      free(ptr_comment_entity->text);
      ptr_comment_entity->text = NULL;
    }
  if(ptr_comment_entity->source != NULL)
    {
      free(ptr_comment_entity->source);
      ptr_comment_entity->source = NULL;
    }
  if(ptr_comment_entity->mid != NULL)
    {
      free(ptr_comment_entity->mid);
      ptr_comment_entity->mid = NULL;
    }
  if(ptr_comment_entity->idstr != NULL)
    {
      free(ptr_comment_entity->idstr);
      ptr_comment_entity->idstr = NULL;
    }
  if(ptr_comment_entity->user != NULL)
    {
      destroy_user_entity(ptr_comment_entity->user);
      ptr_comment_entity->user = NULL;
    }
  if(ptr_comment_entity->status != NULL)
    {
      destroy_weibo_entity(ptr_comment_entity->status);
      ptr_comment_entity->status = NULL;
    }
  free(ptr_comment_entity);
  debug_log_exit(FINE, func_name);
}


void destroy_geo_entity(PTR_GEO_ENTITY ptr_geo_entity)
{
	debug_log_enter(FINE, "destroy_geo_entity", "p", ptr_geo_entity);
	if(ptr_geo_entity->longitude != NULL)
	{
		free(ptr_geo_entity->longitude);
		ptr_geo_entity->longitude = NULL;
	}
	if(ptr_geo_entity->latitude != NULL)
	{
		free(ptr_geo_entity->latitude);
		ptr_geo_entity->latitude = NULL;
	}
	if(ptr_geo_entity->city != NULL)
	{
		free(ptr_geo_entity->city);
		ptr_geo_entity->city = NULL;
	}
	if(ptr_geo_entity->province != NULL)
	{
		free(ptr_geo_entity->province);
		ptr_geo_entity->province = NULL;
	}
	if(ptr_geo_entity->city_name != NULL)
	{
		free(ptr_geo_entity->city_name);
		ptr_geo_entity->city_name = NULL;
	}
	if(ptr_geo_entity->province_name != NULL)
	{
		free(ptr_geo_entity->province_name);
		ptr_geo_entity->province_name = NULL;
	}
	if(ptr_geo_entity->address != NULL)
	{
		free(ptr_geo_entity->address);
		ptr_geo_entity->address = NULL;
	}
	if(ptr_geo_entity->more != NULL)
	{
		free(ptr_geo_entity->more);
		ptr_geo_entity->more = NULL;
	}
	if(ptr_geo_entity->pinyin != NULL)
	{
		free(ptr_geo_entity->pinyin);
		ptr_geo_entity->city_name = NULL;
	}
	free(ptr_geo_entity);
	debug_log_exit(FINE, "destroy_geo_entity");
}


void destroy_weibo_entity_list(PTR_WEIBO_ENTITY ptr_weibo_entity)
{
  PTR_WEIBO_ENTITY weibo = ptr_weibo_entity;
  while(weibo) {
    ptr_weibo_entity = ptr_weibo_entity->next;
    destroy_weibo_entity(weibo);
    weibo = ptr_weibo_entity;
  }
}

void destroy_comment_entity_list(PTR_COMMENT_ENTITY ptr_comment_entity)
{
  PTR_COMMENT_ENTITY comment = ptr_comment_entity;
  while(comment) {
    ptr_comment_entity = ptr_comment_entity->next;
    destroy_comment_entity(comment);
    comment = ptr_comment_entity;
  }
}

void destroy_user_entity_list(PTR_USER_ENTITY ptr_user_entity)
{
  PTR_USER_ENTITY user = ptr_user_entity;
  while(user) {
    ptr_user_entity = ptr_user_entity->next;
    destroy_user_entity(user);
    user = ptr_user_entity;
  }
}

PTR_USER_ENTITY create_user_entity_from_json(cJSON* json_object)
{
        const char* func_name = __func__;
	debug_log_enter(FINE, func_name, "p", json_object); 
	char *debug = cJSON_Print(json_object);
	debug_log(FINEST, func_name, debug);
	free(debug);

	cJSON* ptr_json = NULL;
	int json_length = 0;
	
	PTR_USER_ENTITY ptr_user_entity = init_user_entity();

	if(ptr_user_entity == NULL)
	{
		debug_log(SEVERE, __func__, "SEVERE ERROR : allocate user entity object \n");
		return NULL;
	}

	if(cJSON_GetObjectItem(json_object, "idstr") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "idstr");
		json_length = strlen(ptr_json->valuestring);
		ptr_user_entity->idstr = (char*) malloc((1+json_length)*sizeof(char));
		strcpy(ptr_user_entity->idstr, ptr_json->valuestring);
	}

	if(cJSON_GetObjectItem(json_object, "screen_name") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "screen_name");
		json_length = strlen(ptr_json->valuestring);
		ptr_user_entity->screen_name = (char*) malloc((1+json_length)*sizeof(char));
		strcpy(ptr_user_entity->screen_name, ptr_json->valuestring);
	}
	if(cJSON_GetObjectItem(json_object, "name") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "name");
		json_length = strlen(ptr_json->valuestring);
		ptr_user_entity->name = (char*) malloc((1+json_length)*sizeof(char));
		strcpy(ptr_user_entity->name, ptr_json->valuestring);
	}
	if(cJSON_GetObjectItem(json_object, "description") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "description");
		json_length = strlen(ptr_json->valuestring);
		ptr_user_entity->description = (char*) malloc((1+json_length)*sizeof(char));
		strcpy(ptr_user_entity->description, ptr_json->valuestring);
	}
	if(cJSON_GetObjectItem(json_object, "location") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "location");
		json_length = strlen(ptr_json->valuestring);
		ptr_user_entity->location= (char*) malloc((1+json_length)*sizeof(char));
		strcpy(ptr_user_entity->location, ptr_json->valuestring);
	}
	if(cJSON_GetObjectItem(json_object, "profile_url") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "profile_url");
		json_length = strlen(ptr_json->valuestring);
		ptr_user_entity->profile_url = (char*) malloc((1+json_length)*sizeof(char));
		strcpy(ptr_user_entity->profile_url, ptr_json->valuestring);
	}
	if(cJSON_GetObjectItem(json_object, "gender") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "gender");
		json_length = strlen(ptr_json->valuestring);
		ptr_user_entity->gender = (char*) malloc((1+json_length)*sizeof(char));
		strcpy(ptr_user_entity->gender, ptr_json->valuestring);
	}
	if(cJSON_GetObjectItem(json_object, "id") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "id");
		ptr_user_entity->id = (uint64_t) ptr_json->valuedouble;
	}
	if(cJSON_GetObjectItem(json_object, "friends_count") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "friends_count");
		ptr_user_entity->friends_count = ptr_json->valueint;
	}
	if(cJSON_GetObjectItem(json_object, "followers_count") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "followers_count");
		ptr_user_entity->followers_count = ptr_json->valueint;
	}
	if(cJSON_GetObjectItem(json_object, "statuses_count") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "statuses_count");
		ptr_user_entity->statuses_count = ptr_json->valueint;
	}
	if(cJSON_GetObjectItem(json_object, "favorites_count") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "favorites_count");
		ptr_user_entity->favorites_count = (int) ptr_json->valuedouble;
	}
	if(cJSON_GetObjectItem(json_object, "online_status") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "online_status");
		ptr_user_entity->online_status = (BOOL) ptr_json->valueint;
	}
        if(cJSON_GetObjectItem(json_object, "verified") != NULL)
	  {
	    ptr_json = cJSON_GetObjectItem(json_object, "verified");
	    ptr_user_entity->verified = (BOOL) ptr_json->valueint;
	  }

	debug_log_exit(FINE, func_name);
	return ptr_user_entity;
}

PTR_WEIBO_ENTITY create_weibo_entity_from_json(cJSON* json_object)
{
        const char* func_name = __func__;
	debug_log_enter(FINE, func_name, "p",json_object); 
	char *debug = cJSON_Print(json_object);
	debug_log(FINEST, func_name, debug);
	free(debug);

	cJSON* ptr_json = NULL;
	int json_length = 0;
	int i = 0;
	
	PTR_WEIBO_ENTITY ptr_weibo_entity = init_weibo_entity();
	if(ptr_weibo_entity == NULL)
	{
		debug_log(SEVERE, func_name, "SEVERE ERROR : Cannot create weibo data from HTTP response! \n");
		return NULL;
	}

	if(cJSON_GetObjectItem(json_object, "created_at") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "created_at");
		json_length = strlen(ptr_json->valuestring);
		ptr_weibo_entity->created_at = (char*) malloc((1+json_length)*sizeof(char));
		strcpy(ptr_weibo_entity->created_at, ptr_json->valuestring);
	}
	if(cJSON_GetObjectItem(json_object, "idstr") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "idstr");
		json_length = strlen(ptr_json->valuestring);
		ptr_weibo_entity->idstr = (char*) malloc((1+json_length)*sizeof(char));
		strcpy(ptr_weibo_entity->idstr, ptr_json->valuestring);
	}
	if(cJSON_GetObjectItem(json_object, "mid") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "mid");
		json_length = strlen(ptr_json->valuestring);
		ptr_weibo_entity->mid = (char*) malloc((1+json_length)*sizeof(char));
		strcpy(ptr_weibo_entity->mid, ptr_json->valuestring);
	}
	if(cJSON_GetObjectItem(json_object, "text") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "text");
		json_length = strlen(ptr_json->valuestring);
		ptr_weibo_entity->text = (char*) malloc((1+json_length)*sizeof(char));
		strcpy(ptr_weibo_entity->text, ptr_json->valuestring);
	}
	if(cJSON_GetObjectItem(json_object, "source") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "source");
		json_length = strlen(ptr_json->valuestring);
		ptr_weibo_entity->source = (char*) malloc((1+json_length)*sizeof(char));
		strcpy(ptr_weibo_entity->source, ptr_json->valuestring);
	}
	if(cJSON_GetObjectItem(json_object, "retweeted_status") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "retweeted_status");
		ptr_weibo_entity->retweeted_status = create_weibo_entity_from_json(ptr_json);
	}

	if(cJSON_GetObjectItem(json_object, "id") != NULL)
	{
		ptr_json = cJSON_GetObjectItem(json_object, "id");
		ptr_weibo_entity->id = (uint64_t) ptr_json->valuedouble;
	}
	if(cJSON_GetObjectItem(json_object, "user") != NULL && cJSON_GetObjectItem(json_object, "user")->type != cJSON_NULL)
	{
		ptr_json = cJSON_GetObjectItem(json_object, "user");
		ptr_weibo_entity->user = create_user_entity_from_json(ptr_json);
	}
        if(cJSON_GetObjectItem(json_object, "reposts_count") != NULL)
	  {
	    ptr_json = cJSON_GetObjectItem(json_object, "reposts_count");
	    ptr_weibo_entity->reposts_count =  ptr_json->valueint;
	  }
        if(cJSON_GetObjectItem(json_object, "comments_count") != NULL)
	{
	    ptr_json = cJSON_GetObjectItem(json_object, "comments_count");
	    ptr_weibo_entity->comments_count = ptr_json->valueint;
	}

	debug_log_exit(FINE, func_name);
	return ptr_weibo_entity;
}



PTR_COMMENT_ENTITY create_comment_entity_from_json(cJSON* json_object)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "p", json_object);
  char *debug = cJSON_Print(json_object);
  debug_log(FINEST, func_name, debug);
  free(debug);

  cJSON* ptr_json = NULL;
  int json_length = 0;
  PTR_COMMENT_ENTITY ptr_comment_entity = init_comment_entity();
  if(ptr_comment_entity == NULL) {
    debug_log(SEVERE, __func__, "SEVERE ERROR : allocate comment entity object \n");
    return NULL;
  }

  if(cJSON_GetObjectItem(json_object, "created_at") != NULL) {
    ptr_json = cJSON_GetObjectItem(json_object, "created_at");
    json_length = strlen(ptr_json->valuestring);
    ptr_comment_entity->created_at = (char*) malloc((1+json_length)*sizeof(char));
    strcpy(ptr_comment_entity->created_at, ptr_json->valuestring);
  }
  if(cJSON_GetObjectItem(json_object, "mid") != NULL) {
    ptr_json = cJSON_GetObjectItem(json_object, "mid");
    json_length = strlen(ptr_json->valuestring);
    ptr_comment_entity->mid = (char*) malloc((1+json_length)*sizeof(char));
    strcpy(ptr_comment_entity->mid, ptr_json->valuestring);
  }
  if(cJSON_GetObjectItem(json_object, "text") != NULL) {
    ptr_json = cJSON_GetObjectItem(json_object, "text");
    ptr_comment_entity->text = strdup(ptr_json->valuestring);
  }
  if(cJSON_GetObjectItem(json_object, "source") != NULL) {
    ptr_json = cJSON_GetObjectItem(json_object, "source");
    ptr_comment_entity->source = strdup(ptr_json->valuestring);
  }
  if(cJSON_GetObjectItem(json_object, "id") != NULL) {
    ptr_json = cJSON_GetObjectItem(json_object, "id");
    ptr_comment_entity->id =(uint64_t) ptr_json->valuedouble;
  }
  if(cJSON_GetObjectItem(json_object, "idstr") != NULL) {
    ptr_json = cJSON_GetObjectItem(json_object, "idstr");
    ptr_comment_entity->source = strdup(ptr_json->valuestring);
  }
  if(cJSON_GetObjectItem(json_object, "user") != NULL && cJSON_GetObjectItem(json_object, "user")->type != cJSON_NULL) {
      ptr_json = cJSON_GetObjectItem(json_object, "user");
      ptr_comment_entity->user = create_user_entity_from_json(ptr_json);
      }
  if(cJSON_GetObjectItem(json_object, "status") != NULL && cJSON_GetObjectItem(json_object, "status")->type != cJSON_NULL) {
      ptr_json = cJSON_GetObjectItem(json_object, "status");
      ptr_comment_entity->status = create_weibo_entity_from_json(ptr_json);
  }
  return ptr_comment_entity;
}


PTR_GEO_ENTITY create_geo_entity_from_json(cJSON* json_object)
{
	debug_log_enter(FINE, "create_geo_from_json", "p",json_object); 
	char *debug = cJSON_Print(json_object);
	debug_log(FINEST, "create_geo_from_json", debug);
	free(debug);

	cJSON* ptr_json = NULL;
	int json_length = 0;
	
	PTR_GEO_ENTITY ptr_geo_entity = init_geo_entity();

	if(ptr_geo_entity == NULL)
	{
		debug_log(SEVERE, "create_geo_from_json", "SEVERE ERROR : Cannot create location data from HTTP response! \n");
		return NULL;
	}

	if(cJSON_GetObjectItem(json_object, "longitude") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "longitude");
		json_length = strlen(ptr_json->valuestring);
		ptr_geo_entity->longitude = (char*) malloc((1+json_length)*sizeof(char));
		strcpy(ptr_geo_entity->longitude, ptr_json->valuestring);
	}
	if(cJSON_GetObjectItem(json_object, "latitude") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "latitude");
		json_length = strlen(ptr_json->valuestring);
		ptr_geo_entity->latitude = (char*) malloc((1+json_length)*sizeof(char));
		strcpy(ptr_geo_entity->latitude, ptr_json->valuestring);
	}
	if(cJSON_GetObjectItem(json_object, "city") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "city");
		json_length = strlen(ptr_json->valuestring);
		ptr_geo_entity->city = (char*) malloc((1+json_length)*sizeof(char));
		strcpy(ptr_geo_entity->city, ptr_json->valuestring);
	}
	if(cJSON_GetObjectItem(json_object, "province") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "province");
		json_length = strlen(ptr_json->valuestring);
		ptr_geo_entity->province = (char*) malloc((1+json_length)*sizeof(char));
		strcpy(ptr_geo_entity->province, ptr_json->valuestring);
	}
	if(cJSON_GetObjectItem(json_object, "city_name") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "city_name");
		json_length = strlen(ptr_json->valuestring);
		ptr_geo_entity->city_name = (char*) malloc((1+json_length)*sizeof(char));
		strcpy(ptr_geo_entity->city_name, ptr_json->valuestring);
	}
	if(cJSON_GetObjectItem(json_object, "province_name") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "province_name");
		json_length = strlen(ptr_json->valuestring);
		ptr_geo_entity->province_name = (char*) malloc((1+json_length)*sizeof(char));
		strcpy(ptr_geo_entity->province_name, ptr_json->valuestring);
	}
	if(cJSON_GetObjectItem(json_object, "address") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "address");
		json_length = strlen(ptr_json->valuestring);
		ptr_geo_entity->address = (char*) malloc((1+json_length)*sizeof(char));
		strcpy(ptr_geo_entity->address, ptr_json->valuestring);
	}
	if(cJSON_GetObjectItem(json_object, "pinyin") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "pinyin");
		json_length = strlen(ptr_json->valuestring);
		ptr_geo_entity->pinyin = (char*) malloc((1+json_length)*sizeof(char));
		strcpy(ptr_geo_entity->pinyin, ptr_json->valuestring);
	}
	if(cJSON_GetObjectItem(json_object, "more") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "more");
		json_length = strlen(ptr_json->valuestring);
		ptr_geo_entity->more = (char*) malloc((1+json_length)*sizeof(char));
		strcpy(ptr_geo_entity->more, ptr_json->valuestring);
	}

	debug_log_exit(FINE, "create_geo_from_json");
	return ptr_geo_entity;
}


/* 
   False -> No error
   True -> error happends, it's printed in log
*/
BOOL check_api_error(cJSON* json)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  int error_code;
  char* request;
  cJSON* error_msg = NULL;
  char msg[255];
  error_msg = cJSON_GetObjectItem(json, "error");
  if (error_msg != NULL && error_msg->type != cJSON_NULL) {
    error_code = cJSON_GetObjectItem(json, "error_code")->valueint;
    request = cJSON_GetObjectItem(json, "request")->valuestring;
    snprintf(msg, 255, "Error for API: %s, Code=%d, Error Details=%s", request, error_code, error_msg->valuestring);
    debug_log(WARN, func_name, msg);
    return True;
  }
  return False;
}
