#define _XOPEN_SOURCE_EXTENDED /* To get ncurses wide char support */
#define __USE_XOPEN_EXTENDED

#include <curses.h>
#include <stdint.h>
#include "gui_datatype.h"

#ifndef GUI_POPINPUT_H

#define GUI_POPINPUT_H

void wnd_popinput_handler(PTR_WND_MANAGER wm_mgr, PTR_WND src, PTR_WND dst, PTR_EVENT event);
void wnd_popinput_show(PTR_WND_MANAGER wm_mgr, PTR_WND self, void* data);
void wnd_popinput_refresh(PTR_WND_MANAGER wm_mgr, PTR_WND self, void* data);
void wnd_popinput_destroyer(PTR_WND self);
void wnd_popinput_initializer(PTR_WND self);
PTR_WND wnd_popinput_create(PTR_WND_MANAGER wm_mgr, PTR_WND parent, uint32_t height, uint32_t width, uint32_t y, uint32_t x);
char* wnd_popinput(PTR_WND_MANAGER wm_mgr, uint32_t height, uint32_t width);

#endif
