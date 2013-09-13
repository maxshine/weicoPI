#include <limits.h>

#ifndef DEBUG_UTIL_H
#define DEBUG_UTIL_H 1


#define OFF UNIT_MAX
#define FATAL 10000
#define SEVERE 1000
#define WARNING 100
#define INFO 10
#define FINE 9
#define FINER 8
#define FINEST 7
#define TRACE 5
#define DEFAULT_LOG_LEVEL 10

typedef int LogLevel;

typedef enum BOOL {FALSE=0, TRUE=1} BOOL;


typedef struct log_descriptor {
	FILE * log_file_handler;
	LogLevel effective_level;
} LogDescriptor;

void debug_show_file_content(FILE* file);
void debug_log_text_file(LogLevel level, const char* function_name, FILE* msg);
void debug_log(LogLevel level, const char* function_name, const char* msg);
void do_debug_log(LogLevel level, const char* msg);
BOOL init_debug_log(const char* file_name, const LogLevel effective_level);
BOOL deinit_debug_log(void);
void debug_log_enter(LogLevel level, const char* function_name, const char* format, ...);
void debug_log_exit(LogLevel level, const char* function_name);

#endif
