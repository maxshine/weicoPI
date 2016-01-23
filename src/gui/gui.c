#include <curses.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "gui_datatype.h"


PTR_WND_MANAGER wm_init(void)
{
  WND_MANAGER wm_mgr = malloc(sizeof(WND_MANAGER));
  wm_mgr->focus = NULL;
  wm_mgr->wnd_list = NULL;
  wm_mgr->pop = wm_pop;
  wm_mgr->push = wm_push;
  wm_mgr->refresh = wm_refresh;
  initscr();
  getmaxyx(wm_mgr->height, wm_mgr->width);
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  return wm_mgr;
}

void wm_runloop(PTR_WND_MANAGER wm_mgr)
{
  int key = 0;
  EVENT event;
  while (TRUE) {
    key = getch();
    event.key = key;
    event.type = ET_KEY_PRESSED;
    event.userdata = NULL;
    wm_mgr->handler(wm_mgr, NULL, wm_mgr->focus, &event);
  }
}

void wm_push(PTR_WND_MANAGER wm_mgr, void* data)
{
  PTR_WND wnd = (PTR_WND)data;
  PTR_WND prev = NULL;
  PTR_WND current = wm_mgr->wnd_list;
  PTR_WND next = wm_mgr->wnd_list->next;

  while(current != NULL && current != wnd){
    prev = current;
    current = next;
    next = current->next;
  }

  if (current == wnd) {
    prev->next = current->next;
  }

  while(current != NULL) {
    prev = current;
    current = next;
    next = current->next;
  }
  
  prev->next = wnd;
  wnd->next = NULL;
}

void wm_pop(PTR_WND_MANAGER wm_mgr, void* data)
{
  PTR_WND wnd = (PTR_WND)data;
  PTR_WND prev = NULL;
  PTR_WND current = wm_mgr->wnd_list;
  PTR_WND next = wm_mgr->wnd_list->next;

  while(current !=NULL &&current != wnd){
    prev = current;
    current = next;
    next = current->next;
  }
  if (current == wnd) {
    prev->next = current->next;
  }
  free(wnd->title);
  free(wnd);
}

void wm_refresh(PTR_WND_MANAGER wm_mgr, void* data)
{
  clear();
  PTR_WND wnd = wm_mgr->wnd_list;
  while (wnd != NULL) {
    wborder(wnd->curses_wnd, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
    wnd->refresh(wm_mgr, wnd, NULL);
    wrefresh(wnd->curses_wnd);
    wnd = wnd->next;
  }
  refresh();
}

void wm_handler(PTR_WND_MANAGER wm_mgr, PTR_WND src, PTR_WND dst, PTR_EVENT event)
{

}

PTR_WND wnd_init(PTR_WND_MANAGER wm_mgr, PTR_WND parent, const char* title, uint32_t y, uint32_t x, uint32_t height, uint32_t width)
{
  PTR_WND wnd = (PTR_WND) malloc(sizeof(WND));
  PTR_WND prev = NULL;
  PTR_WND next = NULL;
  wnd->title = strdup(title);
  wnd->x = x;
  wnd->y = y;
  wnd->abs_y = (parent->abs_y + y) % wm_mgr->height;
  wnd->abs_x = (parent->abs_x + x) % wm_mgr->width;
  wnd->height = height;
  wnd->width = width;
  wnd->parent = parent;
  if (parent != NULL) {
    prev = parent->children;
    next = parent->children->next;
    while (next) {
      prev = next;
      next = parent->children->next;
    }
    prev->next = wnd;
  }
  wnd->curses_wnd = newwin((int)height, (int)width, wnd->abs_y, wnd->abs_x);
  (wm_mgr->push)(wm_mgr, wnd, NULL);
  return PTR_WND;
}

void wnd_weibo_initializer(PTR_WND self)
{
  uint32_t i = 0;
  uint32_t weibo_field_cnt = 5;
  uint32_t weibo_field_width = self->width-2;
  uint32_t weibo_field_height = ((WEIBO_CONTENT_LIMIT) / weibo_field_width) + 2;
  self->type = WT_PANEL;
  self->usrdata = (void*) malloc(weibo_field_cnt*sizeof(WINDOW*));
  for (i=0; i<weibo_field_cnt; i++) {
    userdata[i] = derwin(self->curses_wnd, weibo_field_height, weibo_field_width, i*weibo_field_height+1, 1);
  }
}

void wnd_weibo_destroyer(PTR_WND self)
{
  uint32_t i = 0;
  uint32_t weibo_field_cnt = 5;
  for (i=0; i<weibo_field_cnt; i++) {
    delwin((WINDOW*)(userdata[i]));
  }
  free(self->title);
  free(self->userdata);
}

void wnd_weibo_handler(PTR_WND_MANAGER wm_mgr, PTR_WND src, PTR_WND dst, PTR_EVENT event)
{

}

void wnd_weibo_refresh(PTR_WND_MANAGER wm_mgr, PTR_WND self, void* data)
{
  uint32_t i = 0;
  uint32_t weibo_field_cnt = 5;
  for (i=0; i<weibo_field_cnt; i++) {
    wborder((WINDOW*)(userdata[i]), ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
    wrefresh((WINDOW*)(userdata[i]));
  }
}

void wnd_popinput(PTR_WND_MANAGER wm_mgr, PTR_WND parent)
{

}

void wnd_popinput_handler(PTR_WND_MANAGER wm_mgr, PTR_WND src, PTR_WND dst, PTR_EVENT event)
{

}

void wnd_alert(PTR_WND_MANAGER wm_mgr, PTR_WND parent, const char* text, uint32_t sec)
{

}
