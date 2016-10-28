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

void wnd_generic_refresh(PTR_WND_MANAGER wm_mgr, PTR_WND self, void* data);

PTR_WND wnd_init(PTR_WND_MANAGER wm_mgr, PTR_WND parent, const char* title, uint32_t height, uint32_t width, uint32_t y, uint32_t x);
void wnd_weibo_initializer(PTR_WND self);
void wnd_weibo_destroyer(PTR_WND self);
void wnd_weibo_handler(PTR_WND_MANAGER wm_mgr, PTR_WND src, PTR_WND dst, PTR_EVENT event);
void wnd_weibo_refresh(PTR_WND_MANAGER wm_mgr, PTR_WND self, void* data);

void wnd_alert(PTR_WND_MANAGER wm_mgr, char* text);
void wnd_alert_handler(PTR_WND_MANAGER wm_mgr, PTR_WND src, PTR_WND dst, PTR_EVENT event);
void wnd_alert_refresh(PTR_WND_MANAGER wm_mgr, PTR_WND self, void* data);

char* wnd_popinput(PTR_WND_MANAGER wm_mgr, PTR_WND parent, uint32_t height, uint32_t width);

#endif
