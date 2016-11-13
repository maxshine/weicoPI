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

#include <limits.h>
#include "datatype.h"

#ifndef DEBUG_UTIL_H
#define DEBUG_UTIL_H


#define OFF UINT_MAX
#define FATAL 10000
#define SEVERE 1000
#define WARN 100
#define INFO 10
#define FINE 9
#define FINER 8
#define FINEST 7
#define TRACE 5
#define DEFAULT_LOG_LEVEL 10

typedef int LogLevel;

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
