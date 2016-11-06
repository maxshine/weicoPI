#define _XOPEN_SOURCE_EXTENDED /* To get ncurses wide char support */
#define __USE_XOPEN_EXTENDED

#include <curses.h>
#include <stdint.h>
#include "gui_datatype.h"

#ifndef GUI_GENERIC_H

#define GUI_GENERIC_H

void wnd_generic_addstr(PTR_WND self, uint32_t y, uint32_t x, char* str, uint32_t limit);
void wnd_generic_addstr_w(PTR_WND self, uint32_t y, uint32_t x, char* str, uint32_t limit);
void wnd_generic_fillup(PTR_WND self, uint32_t y, uint32_t x, char c);
void wnd_generic_destroyer(PTR_WND self);
void wnd_generic_handler(PTR_WND_MANAGER wm_mgr, PTR_WND src, PTR_WND dst, PTR_EVENT event);
void wnd_generic_refresh(PTR_WND_MANAGER wm_mgr, PTR_WND self, void* data);
void wnd_generic_show(PTR_WND_MANAGER wm_mgr, PTR_WND self, void* data);
void wnd_generic_initializer(PTR_WND self);


#endif
