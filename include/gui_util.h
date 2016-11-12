#define _XOPEN_SOURCE_EXTENDED /* To get ncurses wide char support */

#include <curses.h>
#include <stdint.h>
#include "gui_datatype.h"

#ifndef GUI_UTIL_H

#define GUI_UTIL_H

uint8_t fillup_cchar_utf8(uint8_t* src, cchar_t* dst);

PTR_WND wnd_init(PTR_WND_MANAGER wm_mgr, PTR_WND parent, const char* title, uint32_t height, uint32_t width, uint32_t y, uint32_t x);

#endif
