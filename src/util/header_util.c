#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug_util.h"

char* get_header_field_from_file(FILE* input_file, const char* prefix, char* buffer)
{
        int flag_scanf = 0;
        char* str = NULL;
        char* prefix_str = NULL;

        str = (char*)malloc(100*sizeof(char));
        prefix_str = (char*)malloc((strlen(prefix)+3)*sizeof(char));
        memset(str, '\0', 100*sizeof(char));
        memset(prefix_str, '\0', (strlen(prefix)+3)*sizeof(char));
	sprintf(prefix_str, "%s%%s", prefix);

        rewind(input_file);

        while(fgets(str, 100, input_file)) {
                flag_scanf = sscanf(str, prefix_str, buffer);
                if(flag_scanf != 0) {
                        break;
                }
        }

        rewind(input_file);
}
