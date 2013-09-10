#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include "debug_util.h"

LogDescriptor global_log_descriptor;

void debug_show_file_content(FILE* file)
{
	char *c = (char*)malloc(100*sizeof(char));
	int old_pos = fseek(file, 0L, SEEK_SET);
	while(fgets(c, 100, file)) {
		puts(c);
	}
	fseek(file, old_pos, SEEK_CUR);
	free(c);
}

void do_debug_log(LogLevel level, char* msg)
{
	if (level >= global_log_descriptor.effective_level) {
		fputs(msg, global_log_descriptor.log_file_handler);
		fflush(global_log_descriptor.log_file_handler);
	}	
}

void debug_log(LogLevel level, char* function_name, char* msg)
{
	pid_t pid = getpid();
	pthread_t tid = pthread_self();
	time_t* t = (time_t*)malloc(sizeof(time_t));
	char* timestamp = (char*)malloc(30*sizeof(char));
	char* message = (char*)malloc((30+strlen(msg)+100)*sizeof(char));
	memset(timestamp, 0, 30*sizeof(char));
	memset(message, 0, (30+strlen(msg)+100)*sizeof(char));
	time(t);
	strftime(timestamp, 30, "[%F %T Z%z]", localtime(t));
	sprintf(message, "%s %#x %#x %s %s\n", timestamp, pid, tid, function_name, msg);
	do_debug_log(level, message);
	free(t);
	free(message);
	free(timestamp);	
}


void debug_log_text_file(LogLevel level, char* function_name, FILE* msg)
{
	pid_t pid = getpid();
	pthread_t tid = pthread_self();
	time_t* t = (time_t*)malloc(sizeof(time_t));
	char* timestamp = (char*)malloc(30*sizeof(char));
	char* message = (char*)malloc(100*sizeof(char));
	int old_pos = fseek(msg, 0L, SEEK_SET);
	memset(timestamp, 0, 30*sizeof(char));
	memset(message, 0, 100*sizeof(char));
	time(t);
	strftime(timestamp, 30, "[%F %T Z%z]", localtime(t));
	sprintf(message, "%s %#x %#x %s \n", timestamp, pid, tid, function_name);
	do_debug_log(level, message);
	while (fgets(message, 100, msg)) {
		do_debug_log(level, message);
	}	
	free(t);
	free(message);
	free(timestamp);
	fseek(msg, old_pos, SEEK_SET);
}

BOOL init_debug_log(const char* file_name, const LogLevel effective_level)
{
	global_log_descriptor.effective_level = effective_level;
	global_log_descriptor.log_file_handler = NULL;
	struct passwd* passwd = getpwuid(getuid());
	char* absolute_file_name = NULL;
	if (passwd != NULL) {
		absolute_file_name = (char*)malloc((strlen(passwd->pw_dir)+strelen(file_name)+20)*sizeof(char));
		memset(absolute_file_name, 0, (strlen(passwd->pw_dir)+strlen(file_name)+20)*sizeof(char));
		sprintf(absolute_file_name, "%s%s%s", passwd->pw_dir, "/.weicoPi/", file_name);
		global_log_descriptor.log_file_handler = fopen(absolute_file_name, "a+");
		free(absolute_file_name);
		if (global_log_descriptor.log_file_handler == NULL) {
			return FALSE;	
		}
	} else {
		return FALSE;		
	}
	return TRUE;
}

BOOL deinit_debug_log(void)
{
	fflush(global_log_descriptor.log_file_handler);
	fclose(global_log_descriptor.log_file_handler);
}
