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
#define _USE_XOPEN_EXTENDED
#define _XOPEN_SOURCE

#include <curses.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "constants.h"
#include "datatype.h"
#include "gui_datatype.h"
#include "gui_util.h"
#include "gui_generic.h"
#include "debug_util.h"

void wnd_popinput_handler(PTR_WND_MANAGER wm_mgr, PTR_WND src, PTR_WND dst, PTR_EVENT event)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);

  debug_log_exit(FINE, func_name);
}

/* data is a pointer to a attribute to be used */
void wnd_popinput_show(PTR_WND_MANAGER wm_mgr, PTR_WND self, void* data)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  werase(self->curses_wnd);
  //  wborder(self->curses_wnd,ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  werase(self->children->curses_wnd);
  mvwaddstr(self->curses_wnd, 0, 0, "Please input:");
  curs_set(True);
  echo();
  wmove(self->children->curses_wnd, 0, 0);
  touchwin(self->children->curses_wnd);
  touchwin(self->curses_wnd);
  wrefresh(self->curses_wnd);
  wrefresh(self->children->curses_wnd);
  debug_log_exit(FINE, func_name);
}

void wnd_popinput_refresh(PTR_WND_MANAGER wm_mgr, PTR_WND self, void* data)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);

  if (data != NULL) {
    wattrset(self->children->curses_wnd, *(int*)data);
  }

  touchwin(self->children->curses_wnd);
  touchwin(self->curses_wnd);
  wrefresh(self->curses_wnd);
  wrefresh(self->children->curses_wnd);
  debug_log_exit(FINE, func_name);
}

void wnd_popinput_destroyer(PTR_WND self)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  PTR_WND child = self->children;
  werase(child->curses_wnd);
  wrefresh(child->curses_wnd);
  delwin(child->curses_wnd);
  free(child);
  free(self->title);
  werase(self->curses_wnd);
  wrefresh(self->curses_wnd);
  delwin(self->curses_wnd);
  free(self->user_data);
  free(self);
  debug_log_exit(FINE, func_name);
}

void wnd_popinput_initializer(PTR_WND self)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  self->type = WT_INPUT_FIELD;
  self->children = NULL;
  self->user_data = NULL; /* pointer to input text, deep*/

  self->initializer = wnd_popinput_initializer;
  self->destroyer = wnd_popinput_destroyer;
  self->handler = wnd_popinput_handler;
  self->show = wnd_popinput_show;
  self->wndrefresh = wnd_popinput_refresh;
  debug_log_exit(FINE, func_name);
}

PTR_WND wnd_popinput_create(PTR_WND_MANAGER wm_mgr, PTR_WND parent, uint32_t height, uint32_t width, uint32_t y, uint32_t x)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  EVENT event;
  char* p = NULL;
  PTR_WND wnd = wnd_init(wm_mgr, parent, "input", height+2, width+4, y, x);
  wnd_popinput_initializer(wnd);
  PTR_WND subwnd = wnd_init(wm_mgr, wnd, "inputfield", height, width, 1, 2);
  wnd_generic_initializer(subwnd);
  wnd->user_data = (void*)malloc(sizeof(char)*height*width);
  debug_log_exit(FINE, func_name);
  return wnd;
}

char* wnd_popinput(PTR_WND_MANAGER wm_mgr, uint32_t height, uint32_t width)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  EVENT event;
  char* p = NULL;
  PTR_WND wnd = wnd_popinput_create(wm_mgr, NULL, height, width, (wm_mgr->height-height-2-1), 0/*(wm_mgr->width-width)/2*/);
  wnd->show(wm_mgr, wnd, NULL);
  wgetnstr(wnd->children->curses_wnd, (char*)wnd->user_data, height*width/2);
  debug_log(FINE, func_name, (char*)wnd->user_data);
  noecho();
  curs_set(False);

  /*
  event.type = ET_INPUT_COMPLETE;
  event.usrdata = wnd->usrdata;
  wnd->parent->handler(wm_mgr, wnd, wnd->parent, &event);
  */
  p = (char*)strdup((wnd->user_data));
  wm_mgr->pop(wm_mgr, wnd);
  debug_log_exit(FINE, func_name);
  return p;
}
