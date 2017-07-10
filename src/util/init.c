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
#include "weibo_oath2.h"
#include "datatype.h"
#include "debug_util.h"

char* get_access_token(const char* config_filename)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "s", config_filename);
  FILE* config_file_handle = fopen(config_filename, "r");
  char* code = (char*) malloc(50*sizeof(char));
  memset(code, '\0', 50*sizeof(char));
  fgets(code, 50,  config_file_handle);
  *(code+strlen(code)-1) = '\0';
  fclose(config_file_handle);
  debug_log_exit(FINE, func_name);
  return code;
}

char* put_access_token(const char* config_filename, const char* code)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "ss", config_filename, code);
  FILE* config_file_handle = fopen(config_filename, "w");
  char token[100];
  memset(token, '\0', sizeof(char)*100);
  fetch_access_token(code, token);
  debug_log(FINEST, func_name, token);
  fputs(token, config_file_handle);
  fflush(config_file_handle);
  fclose(config_file_handle);
  debug_log_exit(FINE, func_name);
  return NULL;
}

int deinit(void)
{
	return 1;
}
