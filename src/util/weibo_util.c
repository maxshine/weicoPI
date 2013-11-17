#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "datatype.h"
#include "constants.h"
#include "cJSON.h"
#include "debug_util.h"

PTR_WEIBO_ENTITY init_weibo_entity()
{
	debug_log_enter(FINE, "init_weibo_entity", NULL);
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
		ptr_weibo_entity->attitudes_count = 0;
		ptr_weibo_entity->geo = NULL;
		ptr_weibo_entity->user = NULL;	
		ptr_weibo_entity->prev = NULL;
		ptr_weibo_entity->next = NULL;
		ptr_weibo_entity->pic_urls_qty = 0;
		i = 0;
		while(i++ < MAX_PIC_URL_QTY)
		{
			(ptr_weibo_entity->pic_urls)[i] = NULL;
		}
	}
	debug_log_exit(FINE, "init_weibo_entity");
	return ptr_weibo_entity;
}

PTR_USER_ENTITY init_user_entity() 
{
	debug_log_enter(FINE, "init_user_entity", NULL);
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
		ptr_user_entity->prev = NULL;
		ptr_user_entity->next = NULL;
	}
	debug_log_exit(FINE, "init_user_entity");
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
	debug_log_enter(FINE, "destroy_user_entity", "p", ptr_user_entity);
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
	debug_log_exit(FINE, "destroy_user_entity");
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

void destroy_weibo_entity(PTR_WEIBO_ENTITY ptr_weibo_entity)
{
	debug_log_enter(FINE, "destroy_weibo_entity", "p", ptr_weibo_entity);

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
	if(ptr_weibo_entity->geo != NULL)
	{
		destroy_geo_entity(ptr_weibo_entity->geo);
		ptr_weibo_entity->geo = NULL;
	}
	if(ptr_weibo_entity->user != NULL)
	{
		destroy_user_entity(ptr_weibo_entity->user);
		ptr_weibo_entity->user = NULL;
	}
	if(ptr_weibo_entity->pic_urls_qty != 0)
	{
		i = 0;
		while(i++ < ptr_weibo_entity->pic_urls_qty)
		{
			free((ptr_weibo_entity->pic_urls)[i]);
			(ptr_weibo_entity->pic_urls)[i] = NULL;
		}
	}
	free(ptr_weibo_entity);	
	debug_log_exit(FINE, "destroy_weibo_entity");
}

PTR_USER_ENTITY create_user_from_json(cJSON* json_object)
{
	debug_log_enter(FINE, "create_user_from_json", "p",json_object); 
	char *debug = cJSON_Print(json_object);
	debug_log(FINEST, "create_user_from_json", debug);
	free(debug);

	cJSON* ptr_json = NULL;
	int json_length = 0;
	
	PTR_USER_ENTITY ptr_user_entity = init_user_entity();

	if(ptr_user_entity == NULL)
	{
		debug_log(SEVERE, "create_user_from_json", "SEVERE ERROR : Cannot create user data from HTTP response! \n");
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
		ptr_user_entity->id = (long) ptr_json->valuedouble;
	}
	if(cJSON_GetObjectItem(json_object, "friends_count") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "friends_count");
		ptr_user_entity->friends_count = (int) ptr_json->valuedouble;
	}
	if(cJSON_GetObjectItem(json_object, "followers_count") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "followers_count");
		ptr_user_entity->followers_count = (int) ptr_json->valuedouble;
	}
	if(cJSON_GetObjectItem(json_object, "statuses_count") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "statuses_count");
		ptr_user_entity->statuses_count = (long) ptr_json->valuedouble;
	}
	if(cJSON_GetObjectItem(json_object, "favorites_count") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "favorites_count");
		ptr_user_entity->favorites_count = (long) ptr_json->valuedouble;
	}
	if(cJSON_GetObjectItem(json_object, "online_status") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "online_status");
		ptr_user_entity->online_status = (long) ptr_json->valuedouble;
	}

	debug_log_exit(FINE, "create_user_from_json");
	return ptr_user_entity;
}

PTR_GEO_ENTITY create_geo_from_json(cJSON* json_object)
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

PTR_WEIBO_ENTITY create_weibo_from_json(cJSON* json_object)
{
	debug_log_enter(FINE, "create_weibo_from_json", "p",json_object); 
	char *debug = cJSON_Print(json_object);
	debug_log(FINEST, "create_weibo_from_json", debug);
	free(debug);

	cJSON* ptr_json = NULL;
	int json_length = 0;
	int i = 0;
	
	PTR_WEIBO_ENTITY ptr_weibo_entity = init_weibo_entity();
	if(ptr_weibo_entity == NULL)
	{
		debug_log(SEVERE, "create_weibo_from_json", "SEVERE ERROR : Cannot create weibo data from HTTP response! \n");
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
		ptr_weibo_entity->created_at = (char*) malloc((1+json_length)*sizeof(char));
		strcpy(ptr_weibo_entity->created_at, ptr_json->valuestring);
	}
	if(cJSON_GetObjectItem(json_object, "mid") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "mid");
		json_length = strlen(ptr_json->valuestring);
		ptr_weibo_entity->created_at = (char*) malloc((1+json_length)*sizeof(char));
		strcpy(ptr_weibo_entity->created_at, ptr_json->valuestring);
	}
	if(cJSON_GetObjectItem(json_object, "text") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "text");
		json_length = strlen(ptr_json->valuestring);
		ptr_weibo_entity->created_at = (char*) malloc((1+json_length)*sizeof(char));
		strcpy(ptr_weibo_entity->created_at, ptr_json->valuestring);
	}
	if(cJSON_GetObjectItem(json_object, "source") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "source");
		json_length = strlen(ptr_json->valuestring);
		ptr_weibo_entity->created_at = (char*) malloc((1+json_length)*sizeof(char));
		strcpy(ptr_weibo_entity->created_at, ptr_json->valuestring);
	}
	if(cJSON_GetObjectItem(json_object, "pic_urls") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "pic_urls");
		json_length = cJSON_GetArraySize(ptr_json);
		ptr_weibo_entity->pic_urls_qty = json_length;
		while(i++ < ptr_weibo_entity->pic_urls_qty)
		{
			json_length = strlen(cJSON_GetArrayItem(ptr_json, i)->valuestring);
			(ptr_weibo_entity->pic_urls)[i] = (char*) malloc((1+json_length)*sizeof(char));
			strcpy((ptr_weibo_entity->pic_urls)[i], cJSON_GetArrayItem(ptr_json, i)->valuestring);
		}
	
	}
	if(cJSON_GetObjectItem(json_object, "id") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "id");
		ptr_weibo_entity->id = (long)ptr_json->valuedouble;
	}
	if(cJSON_GetObjectItem(json_object, "user") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "user");
		ptr_weibo_entity->user = create_user_from_json(ptr_json);
	}
	if(cJSON_GetObjectItem(json_object, "geo") != NULL)		
	{
		ptr_json = cJSON_GetObjectItem(json_object, "geo");
		ptr_weibo_entity->geo = create_geo_from_json(ptr_json);
	}

	debug_log_exit(FINE, "create_weibo_from_json");
	return ptr_weibo_entity;
}
