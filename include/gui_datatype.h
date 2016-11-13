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


#define _XOPEN_SOURCE_EXTENDED /* To get ncurses wide char support */

#include <curses.h>
#include <stdint.h>

#ifndef GUI_DATATYPE_H

#define GUI_DATATYPE_H

typedef enum event_type {
  ET_KEY_PRESSED,
  ET_DATA_REFRESH,
  ET_WINDOW_REFRESH,
  ET_INPUT_COMPLETE
} EVENT_TYPE;

typedef struct event {
  void* user_data;
  int key;
  EVENT_TYPE type;
} EVENT, *PTR_EVENT;

typedef enum mywindow_type {
  WT_TEXT_FIELD,
  WT_INPUT_FIELD,
  WT_PANEL,
  WT_ALERT,
  WT_GENERIC
} WND_TYPE;

struct mywindow_manager;


typedef struct mywindow {
  char* title;
  uint32_t x;
  uint32_t y;
  uint32_t abs_x;
  uint32_t abs_y;
  uint32_t width;
  uint32_t height;

  WND_TYPE type;
  struct mywindow_manager* wm_mgr;
  WINDOW* curses_wnd;
  void (*initializer)(struct mywindow*);
  void (*destroyer)(struct mywindow*);
  void (*handler)(struct mywindow_manager*, struct mywindow*, struct mywindow*, struct event*);
  void (*show)(struct mywindow_manager*, struct mywindow*, void*);
  void (*wndrefresh)(struct mywindow_manager*, struct mywindow*, void*);

  void* user_data;

  struct mywindow* parent;
  struct mywindow* children;
  struct mywindow* prev;
  struct mywindow* next;
  
} WND, *PTR_WND;

typedef struct mywindow_manager {
  uint32_t width;
  uint32_t height;
  struct mywindow* root_wnd_list;
  struct mywindow* focus;
  void (*handler)(struct mywindow_manager*, struct mywindow*, struct mywindow*, struct event*);
  void (*pop)(struct mywindow_manager*, void*);
  void (*push)(struct mywindow_manager*, void*);
  void (*show)(struct mywindow_manager*, void*);
  void (*wmrefresh)(struct mywindow_manager*, void*);;
} WND_MANAGER, *PTR_WND_MANAGER;

#endif
