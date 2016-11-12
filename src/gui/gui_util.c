#define _XOPEN_SOURCE_EXTENDED /* To get ncurses wide char support */
#define _USE_XOPEN_EXTENDED
#define _XOPEN_SOURCE

#include <string.h>
#include <curses.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include "constants.h"
#include "datatype.h"
#include "gui_datatype.h"
#include "debug_util.h"


uint8_t fillup_cchar_utf8(uint8_t* src, cchar_t* dst)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  uint8_t c = 1; /* length of utf8 coding of a char */
  uint8_t i = *src & 0x80; /* indicator a char is ansi or multi bytes */
  uint8_t j = 0;
  wchar_t t1 = 0;
  wchar_t t2[CCHARW_MAX] = {0};
  attr_t attrs;
  short int pairs;
  
  for (j=0; j<CCHARW_MAX; j++) {
    t2[j] = '\0';
  }
  if (i == 0) {  /* it's ansi */
    t2[0] = (wchar_t)(*src);
    c = 1;
  } else {
    c = 0;
    while (i != 0 && c <= 6) {
      c++;
      i = (*src<<c) & 0x80;
    }
    j = 0;
    while (j < c-1) {
      t1 = *(src+c-1-j) & 0x3F;
      t2[0] = t2[0] | (t1<<(6*j));
      j++;
    }
    t1 = *src & (0xFF>>(c+1));
    t2[0] = t2[0] | (t1<<(6*(c-1)));
  }
  attr_get(&attrs, &pairs, NULL);
  setcchar(dst, t2, attrs, pairs, NULL);
  debug_log_exit(FINE, func_name);
  return c;
}

PTR_WND wnd_init(PTR_WND_MANAGER wm_mgr, PTR_WND parent, const char* title, uint32_t height, uint32_t width, uint32_t y, uint32_t x)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "ppsdddd", wm_mgr, parent, title, height, width, y, x);
  PTR_WND wnd = (PTR_WND) malloc(sizeof(WND));
  PTR_WND current = NULL;

  wnd->title = (char*)strdup((char*)title);
  wnd->x = x;
  wnd->y = y;
  if (parent != NULL) {
    wnd->abs_y = (parent->abs_y + y) % wm_mgr->height;
    wnd->abs_x = (parent->abs_x + x) % wm_mgr->width;
  } else {
    wnd->abs_y = y;
    wnd->abs_x = x;
  }
  wnd->height = height;
  wnd->width = width;
  wnd->parent = parent;
  wnd->wm_mgr = wm_mgr;
  wnd->show = NULL;
  wnd->initializer = NULL;
  wnd->destroyer = NULL;
  wnd->handler = NULL;
  wnd->wndrefresh = NULL;
  if (parent != NULL) {
    current = parent->children;
    while (current && current->next) {
      current = current->next;
    }
    if (current == NULL) {
      parent->children = wnd;
    } else {
      current->next = wnd;
    }
    wnd->prev = current;
    wnd->next = NULL;
    wnd->curses_wnd = derwin(parent->curses_wnd, (int)(wnd->height), (int)(wnd->width), y, x);
  } else {
    wnd->curses_wnd = newwin((int)height, (int)width, wnd->abs_y, wnd->abs_x);
    wm_mgr->push(wm_mgr, wnd);
  }
  debug_log_exit(FINE, func_name);
  return wnd;
}
