#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void debug_show_file_content(FILE* file)
{
	char *c = (char*)malloc(100*sizeof(char));
	int old_pos = fseek(file, 0L, SEEK_SET);
	while(fgets(c, 100, file)) {
		puts(c);
	}
	fseek(file, old_pos, SEEK_CUR);
}
