#define _XOPEN_SOURCE_EXTENDED /* To get ncurses wide char support */
#define __USE_XOPEN_EXTENDED

#include <curses.h>
#include <stdint.h>
#include "gui_datatype.h"

#ifndef GUI_ALERT_H

#define GUI_ALERT_H

void wnd_alert_handler(PTR_WND_MANAGER wm_mgr, PTR_WND src, PTR_WND dst, PTR_EVENT event);
void wnd_alert_addstr_w(PTR_WND self, uint8_t* str, uint32_t limit);
void wnd_alert_fillup(PTR_WND self, char c, int attrs, short color_pair_number, void* opts);
void wnd_alert_show(PTR_WND_MANAGER wm_mgr, PTR_WND self, void* data);
void wnd_alert_refresh(PTR_WND_MANAGER wm_mgr, PTR_WND self, void* data);
void wnd_alert_destroyer(PTR_WND self);
void wnd_alert_initializer(PTR_WND self);
PTR_WND wnd_alert_create(PTR_WND_MANAGER wm_mgr, PTR_WND parent, uint32_t height, uint32_t width, uint32_t y, uint32_t x, char* title);
void wnd_alert(PTR_WND_MANAGER wm_mgr, char* text);


#endif
