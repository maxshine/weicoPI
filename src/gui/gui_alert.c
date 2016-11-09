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
#include "gui_generic.h"
#include "debug_util.h"

void wnd_alert_handler(PTR_WND_MANAGER wm_mgr, PTR_WND src, PTR_WND dst, PTR_EVENT event)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);

  switch(event->type)
    {
    case ET_KEY_PRESSED:
      switch(event->key)
        {
        case KEY_ENTER:
	case 0012:
	  werase(dst->curses_wnd);
	  if (dst->parent == NULL) {
	    wm_mgr->pop(wm_mgr, dst);
	  }
          break;
	default:
	  break;
	}
      break;
    default:
      break;
    }
  debug_log_exit(FINE, func_name);
}

void wnd_alert_addstr_w(PTR_WND self, uint8_t* str, uint32_t limit)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  cchar_t c;
  uint8_t* p1 = str;
  uint8_t offset = 0;
  uint32_t i = 0;
  memset(&c, 0, sizeof(c));

  while(*p1 && i<limit) {
    offset = fillup_cchar_utf8(p1, &c);
    wadd_wch(self->children->curses_wnd, &c);
    p1 = p1 + offset;
    i = i + 2; /* hard code width of wide character */
  }
  debug_log_exit(FINE, func_name);
}

void wnd_alert_fillup(PTR_WND self, char c, int attrs, short color_pair_number, void* opts)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  uint8_t y = 0;
  uint8_t x = 0;
  if (attrs != -1) {
    wattrset(self->children->curses_wnd, attrs);
  }
  if (color_pair_number != 0) {
    wcolor_set(self->children->curses_wnd, color_pair_number, opts);
  }
  while(y < self->children->height) {
    waddch(self->children->curses_wnd, c);
    x = getcurx(self->children->curses_wnd);
    if (x == self->children->width-1) {
      y++;
    }
  }
  debug_log_exit(FINE, func_name);
}

/* data is a pointer to a attribute to be used */
void wnd_alert_show(PTR_WND_MANAGER wm_mgr, PTR_WND self, void* data)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  werase(self->curses_wnd);
  //wborder(self->curses_wnd,ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  werase(self->children->curses_wnd);

  if (data != NULL) {
    wattrset(self->children->curses_wnd, *(int*)data);
  }
  if (self->user_data != NULL) {
    wnd_alert_addstr_w(self, (uint8_t*)(self->user_data), self->children->height*self->children->width);
  } else {
    wnd_alert_fillup(self, ' ', -1, 0, NULL); /* no text, clear up field */
  }
  touchwin(self->children->curses_wnd);
  wrefresh(self->children->curses_wnd);
  touchwin(self->curses_wnd);
  wrefresh(self->curses_wnd);
  debug_log_exit(FINE, func_name);
}

void wnd_alert_refresh(PTR_WND_MANAGER wm_mgr, PTR_WND self, void* data)
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

void wnd_alert_destroyer(PTR_WND self)
{
  PTR_WND child = self->children;
  werase(child->curses_wnd);
  delwin(child->curses_wnd);
  free(child);
  werase(self->curses_wnd);
  free(self->title);
  delwin(self->curses_wnd);
  free(self);
}

void wnd_alert_initializer(PTR_WND self)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  self->type = WT_ALERT;
  self->children = NULL;
  self->user_data = NULL; /* pointer to displayed txt, shallow */
  self->initializer = wnd_alert_initializer;
  self->destroyer = wnd_alert_destroyer;
  self->handler = wnd_alert_handler;
  self->show = wnd_alert_show;
  self->wndrefresh = wnd_alert_refresh;
  debug_log_exit(FINE, func_name);
}

PTR_WND wnd_alert_create(PTR_WND_MANAGER wm_mgr, PTR_WND parent, uint32_t height, uint32_t width, uint32_t y, uint32_t x, uint8_t* text)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  EVENT event;
  char* p = NULL;
  PTR_WND wnd = wnd_init(wm_mgr, parent, "alert", height, width, y, x);
  wnd_alert_initializer(wnd);
  wnd->user_data = text;
  PTR_WND subwnd = wnd_init(wm_mgr, wnd, "alert text", height-2, width-4, 1, 2);
  wnd_generic_initializer(subwnd);
  debug_log_exit(FINE, func_name);
  return wnd;
}

void wnd_alert(PTR_WND_MANAGER wm_mgr, char* text)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  uint32_t l = (uint32_t) strlen(text);
  l += 4;
  PTR_WND wnd = wnd_alert_create(wm_mgr, NULL, 4, l>50?50:l, (wm_mgr->height-4)/2, (wm_mgr->width-50)/2, (uint8_t*)text);
  wnd->show(wm_mgr, wnd, NULL);
  debug_log_exit(FINE, func_name);
}

void wnd_alert_w(PTR_WND_MANAGER wm_mgr, uint8_t* text)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  uint32_t l = (uint32_t) strlen(text);
  l += 4;
  PTR_WND wnd = wnd_alert_create(wm_mgr, NULL, 4, l>50?50:l, (wm_mgr->height-4)/2, (wm_mgr->width-50)/2, text);
  wnd->show(wm_mgr, wnd, NULL);
  debug_log_exit(FINE, func_name);
}
