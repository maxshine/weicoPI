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
#define __USE_XOPEN_EXTENDED

#include <curses.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "constants.h"
#include "datatype.h"
#include "gui_datatype.h"
#include "gui_util.h"
#include "debug_util.h"

void wnd_generic_addstr(PTR_WND self, uint32_t y, uint32_t x, char* str, uint32_t limit)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  cchar_t c;
  char* p = (char*)malloc(sizeof(char)*(limit+1));
  char* p1 = p;
  uint32_t orig_x = x;
  uint32_t orig_y = y;
  strncpy(p, str, limit);
  *(p+limit+1)='\0';
  while(*p && y < self->height-2) {
    wmove(self->curses_wnd, (int)y, (int)x);
    waddch(self->curses_wnd, *p); 
    getyx(self->curses_wnd, y, x);
    if (x >= self->width-2) {
      y += 1;
      x = orig_x;
    }
    p++;
  }
  free(p1);
  debug_log_exit(FINE, func_name);
}

void wnd_generic_addstr_w(PTR_WND self, uint32_t y, uint32_t x, char* str, uint32_t limit)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  cchar_t c;
  uint8_t* p1 = (uint8_t*)str;
  uint8_t offset = 0;
  uint32_t orig_x = x;
  uint32_t orig_y = y;
  uint32_t i = 0;
  memset(&c, 0, sizeof(c));
  while(*p1 && y<self->height-2 && i<limit) {
    offset = fillup_cchar_utf8(p1, &c);
    wmove(self->curses_wnd, (int)y, (int)x);
    wadd_wch(self->curses_wnd, &c);
    getyx(self->curses_wnd, y, x);
    if (x >= self->width-4) {
      y += 1;
      x = orig_x;
    }
    p1 = p1 + offset;
    i++;
  }
  debug_log_exit(FINE, func_name);
}

void wnd_generic_fillup(PTR_WND self, uint32_t y, uint32_t x, char c)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  uint32_t orig_x = x;
  uint32_t orig_y = y;
  while(y < self->height-1) {
    wmove(self->curses_wnd, (int)y, (int)x);
    waddch(self->curses_wnd, c);
    getyx(self->curses_wnd, y, x);
    if (x >= self->width-2) {
      y += 1;
      x = orig_x;
    }
  }
  debug_log_exit(FINE, func_name);
}

void wnd_generic_destroyer(PTR_WND self)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  PTR_WND child = self->children;
  while (child) {
    if (child->destroyer) {
      child->destroyer(child);
    }
    child = child->next;
  }
  if (self->user_data != NULL) {
    free(self->user_data);
  }
  werase(self->curses_wnd);
  wrefresh(self->curses_wnd);
  delwin(self->curses_wnd);
  free(self->title);
  debug_log_exit(FINE, func_name);
}

void wnd_generic_handler(PTR_WND_MANAGER wm_mgr, PTR_WND src, PTR_WND dst, PTR_EVENT event)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);

  switch(event->type)
    {
    default:
      if (dst->children) {
	dst->children->handler(wm_mgr, dst, dst->children, event);
      } else if (dst->parent) {
	dst->parent->handler(wm_mgr, dst, dst->parent, event);
      }
      break;
    }
  debug_log_exit(FINE, func_name);
}

void wnd_generic_refresh(PTR_WND_MANAGER wm_mgr, PTR_WND self, void* data)
{
  const char* func_name = __func__;
  PTR_WND child = self->children;
  debug_log_enter(FINE, func_name, NULL);
  //redrawwin(self->curses_wnd);                                                                                                
  touchwin(self->curses_wnd);
  wrefresh(self->curses_wnd);
  while(child) {
    if(child->wndrefresh) {
      child->wndrefresh(wm_mgr, child, data);
    }
    child = child->next;
  }
  debug_log_exit(FINE, func_name);
}

void wnd_generic_show(PTR_WND_MANAGER wm_mgr, PTR_WND self, void* data)
{
  const char* func_name = __func__;
  PTR_WND child = self->children;
  debug_log_enter(FINE, func_name, NULL);
  //redrawwin(self->curses_wnd);
  touchwin(self->curses_wnd);
  wrefresh(self->curses_wnd);
  while(child) {
    if(child->show) {
      child->show(wm_mgr, child, data);
    }
    child = child->next;
  }
  debug_log_exit(FINE, func_name);
}

void wnd_generic_initializer(PTR_WND self)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "p", self);
  self->type = WT_GENERIC;
  self->user_data = NULL;
  self->children = NULL;
  self->initializer = wnd_generic_initializer;
  self->destroyer = wnd_generic_destroyer;
  self->handler = wnd_generic_handler;
  self->show = wnd_generic_show;
  self->wndrefresh = wnd_generic_refresh;
  debug_log_exit(FINE, func_name);
}

