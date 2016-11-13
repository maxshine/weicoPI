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
#include "gui_datatype.h"

#ifndef GUI_H

#define GUI_H

PTR_WND_MANAGER wm_init(void);
void wm_runloop(PTR_WND_MANAGER wm_mgr);
void wm_push(PTR_WND_MANAGER wm_mgr, void* data);
void wm_pop(PTR_WND_MANAGER wm_mgr, void* data);
void wm_refresh(PTR_WND_MANAGER wm_mgr, void* data);
void wm_handler(PTR_WND_MANAGER wm_mgr, PTR_WND src, PTR_WND dst, PTR_EVENT event);
void wm_show(PTR_WND_MANAGER wm_mgr, void* data);

void wnd_weibo_initializer(PTR_WND self);
void wnd_weibo_destroyer(PTR_WND self);
void wnd_weibo_handler(PTR_WND_MANAGER wm_mgr, PTR_WND src, PTR_WND dst, PTR_EVENT event);
void wnd_weibo_refresh(PTR_WND_MANAGER wm_mgr, PTR_WND self, void* data);
void wnd_weibo_show(PTR_WND_MANAGER wm_mgr, PTR_WND self, void* data);
void wnd_weibo_initializer(PTR_WND self);
PTR_WND wnd_weibo_create(PTR_WND_MANAGER wm_mgr, PTR_WND parent, uint32_t height, uint32_t width, uint32_t y, uint32_t x);

void wnd_user_handler(PTR_WND_MANAGER wm_mgr, PTR_WND src, PTR_WND dst, PTR_EVENT event);
void wnd_user_destroyer(PTR_WND self);
void wnd_user_show(PTR_WND_MANAGER wm_mgr, PTR_WND self, void* data);
void wnd_user_refresh(PTR_WND_MANAGER wm_mgr, PTR_WND self, void* data);
void wnd_user_initializer(PTR_WND self);
PTR_WND wnd_user_create(PTR_WND_MANAGER wm_mgr, PTR_WND parent);

void wnd_comment_handler(PTR_WND_MANAGER wm_mgr, PTR_WND src, PTR_WND dst, PTR_EVENT event);
void wnd_comment_destroyer(PTR_WND self);
void wnd_comment_show(PTR_WND_MANAGER wm_mgr, PTR_WND self, void* data);
void wnd_comment_refresh(PTR_WND_MANAGER wm_mgr, PTR_WND self, void* data);
void wnd_comment_initializer(PTR_WND self);
PTR_WND wnd_comment_create(PTR_WND_MANAGER wm_mgr, PTR_WND parent, char* weibo_id);

#endif
