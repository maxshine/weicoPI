#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug_util.h"


size_t write_body(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	char** data = userdata;
	*data = (char*)realloc(*data, size*nmemb*sizeof(char)+1*sizeof(char));
	memset(*data, size*nmemb*sizeof(char)+1*sizeof(char), 0);
	memcpy(*data, ptr, size*nmemb*sizeof(char));
}
