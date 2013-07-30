#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* get_auth_code(const char* config_filename)
{
	FILE* config_file_handle = fopen(config_filename, "r");
	char* code = (char*) malloc(50*sizeof(char));
	memset(code, '\0', 50*sizeof(char));
	fgets(code, 50,  config_file_handle);
	*(code+strlen(code)-1) = '\0';
	fclose(config_file_handle);
	return code;
}

int deinit(void)
{
	return 1;
}
