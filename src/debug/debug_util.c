/****************************************************************************
 * Copyright (c) 2013-now                                                   *
 * This software is subjected to Apache License Version 2.0, January 2004   *
 * http://www.apache.org/licenses/                                          *
 * Permission is hereby granted, free of charge, to any person obtaining a  *
 * copy of this software and associated documentation files (the            *
 * "Software"), to deal in the Software without restriction, including      *
 * without limitation the rights to use, copy, modify, merge, publish,      *
 * distribute, distribute with modifications, sublicense, and/or sell       *
 * copies of the Software, and to permit persons to whom the Software is    *
 * furnished to do so, subject to the following conditions:                 *
 *                                                                          *
 * The above copyright notice and this permission notice shall be included  *
 * in all copies or substantial portions of the Software.                   *
 *                                                                          *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS  *
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF               *
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   *
 * IN NO EVENT SHALL THE ABOVE COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,   *
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR    *
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR    *
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.                               *
 *                                                                          *
 * Except as contained in this notice, the name(s) of the above copyright   *
 * holders shall not be used in advertising or otherwise to promote the     *
 * sale, use or other dealings in this Software without prior written       *
 * authorization.                                                           *
 ****************************************************************************/

/****************************************************************************
 *  Author: Yang, Gao  <maxshine@gmail.com> 2013-on                         *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <pwd.h>
#include <dirent.h>
#include <errno.h>
#include <stdarg.h>
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

void do_debug_log(LogLevel level, const char* msg)
{
	if (level >= global_log_descriptor.effective_level) {
		fputs(msg, global_log_descriptor.log_file_handler);
		fflush(global_log_descriptor.log_file_handler);
	}	
}

void debug_log(LogLevel level, const char* function_name, const char* msg)
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
	sprintf(message, "%s %#x %#x %s %d %s\n", timestamp, pid, tid, function_name, level, msg);
	do_debug_log(level, message);
	free(t);
	free(message);
	free(timestamp);	
}


void debug_log_text_file(LogLevel level, const char* function_name, FILE* msg)
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
	sprintf(message, "%s %#x %#x %s\n", timestamp, pid, tid, function_name);
	do_debug_log(level, message);
	while (fgets(message, 100, msg)) {
		do_debug_log(level, message);
	}	
	do_debug_log(level, "\n");
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
	DIR *dir_handler = NULL;
	if (passwd != NULL) {
		if (passwd->pw_dir == NULL) {
			puts("Please ensure the user has home directory configured\n");
			return False;
		}
		dir_handler = opendir(passwd->pw_dir);
		if (errno == ENOENT || errno == ENOTDIR) {
			perror("Please ensure the user home directory created");
		}
		closedir(dir_handler);
		dir_handler = NULL;
		absolute_file_name = (char*)malloc((strlen(passwd->pw_dir)+strlen(file_name)+20)*sizeof(char));
		memset(absolute_file_name, 0, (strlen(passwd->pw_dir)+strlen(file_name)+20)*sizeof(char));
		sprintf(absolute_file_name, "%s%s", passwd->pw_dir, "/.weicoPi");
		dir_handler = opendir(absolute_file_name);
		if (errno == ENOENT) {
			mkdir(absolute_file_name, S_IRUSR | S_IWUSR | S_IXUSR);
		}
		closedir(dir_handler);
		dir_handler = NULL;
		sprintf(absolute_file_name, "%s%s%s", passwd->pw_dir, "/.weicoPi/", file_name);
		global_log_descriptor.log_file_handler = fopen(absolute_file_name, "a+");
		free(absolute_file_name);
		if (global_log_descriptor.log_file_handler == NULL) {
			return False;	
		}
	} else {
		return False;		
	}
	return True;
}

BOOL deinit_debug_log(void)
{
	fflush(global_log_descriptor.log_file_handler);
	fclose(global_log_descriptor.log_file_handler);
	return True;
}

void debug_log_enter(LogLevel level, const char* function_name, const char* format, ...)
{
        pid_t pid = getpid();
        pthread_t tid = pthread_self();
        time_t* t = (time_t*)malloc(sizeof(time_t));
        char* timestamp = (char*)malloc(30*sizeof(char));
        char* message_prefix = (char*)malloc(100*sizeof(char));
	char* message_1 = NULL;
	char* message = NULL;
	int format_length = 0;
	char c = '\0';
	int i = 0;
	va_list vg;
	
        memset(timestamp, 0, 30*sizeof(char));
        memset(message_prefix, 0, 100*sizeof(char));
        time(t);
        strftime(timestamp, 30, "[%F %T Z%z]", localtime(t));
        sprintf(message_prefix, "%s %#x %#x %s > %s ", timestamp, pid, tid, function_name, "ENTRY");
	message_1 = message_prefix;
	message = (char*)malloc((1000+strlen(message_1))*sizeof(char));
	memset(message, 0, (1000+strlen(message_1))*sizeof(char));

	if(format != NULL)
	{
		format_length = strlen(format);
		va_start(vg, format);
		while(c=*(format+i)) {
		  switch(c){
			case 'd':
			  sprintf(message, "%s : %d", message_1, va_arg(vg, int));
			  free(message_1);
			  message_1 = message;
			  message = (char*)malloc((1000+strlen(message_1))*sizeof(char));
			  memset(message, 0, (1000+strlen(message_1))*sizeof(char));
			  i++;
			  break;
			case 'f':
			  sprintf(message, "%s : %lf", message_1, va_arg(vg, double));
			  free(message_1);
			  message_1 = message;
			  message = (char*)malloc((1000+strlen(message_1))*sizeof(char));
			  memset(message, 0, (1000+strlen(message_1))*sizeof(char));
			  i++;
			  break;
			case 's':
			  sprintf(message, "%s : %s", message_1, va_arg(vg, char*));
			  free(message_1);
			  message_1 = message;
			  message = (char*)malloc((1000+strlen(message_1))*sizeof(char));
			  memset(message, 0, (1000+strlen(message_1))*sizeof(char));
			  i++;
			  break;
			case 'p':
			  sprintf(message, "%s : %p", message_1, va_arg(vg, void*));
			  free(message_1);
			  message_1 = message;
			  message = (char*)malloc((1000+strlen(message_1))*sizeof(char));
			  memset(message, 0, (1000+strlen(message_1))*sizeof(char));
			  i++;
			  break;
			default:
			  i++;
			  break;
		  }
		}
		va_end(vg);
	}
	sprintf(message, "%s\n", message_1);
        do_debug_log(level, message);
	free(message_1);
	free(message);
}

void debug_log_exit(LogLevel level, const char* function_name)
{
        pid_t pid = getpid();
        pthread_t tid = pthread_self();
        time_t* t = (time_t*)malloc(sizeof(time_t));
        char* timestamp = (char*)malloc(30*sizeof(char));
        char* message = (char*)malloc(100*sizeof(char));

        memset(timestamp, 0, 30*sizeof(char));
        memset(message, 0, 100*sizeof(char));
        time(t);
        strftime(timestamp, 30, "[%F %T Z%z]", localtime(t));
        sprintf(message, "%s %#x %#x %s < %s\n", timestamp, pid, tid, function_name, "EXIT");
        do_debug_log(level, message);
	free(message);
}
