#include <curses.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "constants.h"
#include "datatype.h"
#include "weibo_datatype.h"
#include "gui_datatype.h"
#include "weibo.h"
#include "comment.h"
#include "user.h"
#include "friend.h"
#include "account.h"
#include "debug_util.h"

extern char* ACCESS_TOKEN;
extern char* USERID;
extern char* ACCOUNTID;
extern char* FRIENDID;
extern uint32_t PAGE;  /* start with page #1 */
extern uint32_t WEIBO_TYPE; /* 0 --  public timeline; 1 -- user; 2 -- friend timeline*/

void wm_runloop(PTR_WND_MANAGER wm_mgr)
{
  char debug[40];
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  int key = 0;
  EVENT event;
  WINDOW* tmp = newwin(1,1,wm_mgr->height-1, wm_mgr->width-1);
  keypad(tmp, TRUE);
  while (TRUE) {
    key = wgetch(tmp);
    snprintf(debug, 40, "key: %s value: %d", keyname(key), key);
    debug_log(FINEST, func_name, debug); 
    event.key = key;
    event.type = ET_KEY_PRESSED;
    event.userdata = NULL;
    (wm_mgr->handler)(wm_mgr, NULL, wm_mgr->focus, &event);
    (wm_mgr->show)(wm_mgr, NULL);
  }
  debug_log_exit(FINE, func_name);
}

void wm_push(PTR_WND_MANAGER wm_mgr, void* data)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  PTR_WND wnd = (PTR_WND)data;
  PTR_WND prev = NULL;
  PTR_WND current = wm_mgr->root_wnd_list;

  while(current != NULL && current != wnd){
    prev = current;
    current = current->next;
  }

  if (current != NULL && current == wnd) {
    prev->next = current->next;
  }
  if (current != NULL && current->next != NULL) {
    current->next->prev = prev;
  }

  while(current != NULL) {
    prev = current;
    current = current->next;
  }
  if (prev == NULL) {
    wm_mgr->root_wnd_list = wnd;
  } else {
    prev->next = wnd;
  }
  wnd->next = NULL;
  wnd->prev = prev;
  wm_mgr->focus = wnd;
  debug_log_exit(FINE, func_name);
}

void wm_pop(PTR_WND_MANAGER wm_mgr, void* data)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  PTR_WND wnd = (PTR_WND)data;
  PTR_WND prev = NULL;
  PTR_WND current = wm_mgr->root_wnd_list;

  while(current !=NULL && current != wnd){
    prev = current;
    current = current->next;
  }
  if (current != NULL && current == wnd) {
    prev->next = current->next;
  }
  if (current != NULL && current->next != NULL) {
    current->next->prev= prev;
  }
  free(wnd->title);
  delwin(wnd->curses_wnd);
  free(wnd);
  wnd = wm_mgr->root_wnd_list;

  while(wnd->next) {
    wnd = wnd->next;
  }
  wm_mgr->focus = wnd;
  debug_log_exit(FINE, func_name);
}

void wm_refresh(PTR_WND_MANAGER wm_mgr, void* data)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  PTR_WND wnd = wm_mgr->root_wnd_list;
  while (wnd != NULL) {
    if (wnd->show) {
      wnd->show(wm_mgr, wnd, NULL);
    }
    wnd = wnd->next;
  }
  debug_log_exit(FINE, func_name);
}

void wm_handler(PTR_WND_MANAGER wm_mgr, PTR_WND src, PTR_WND dst, PTR_EVENT event)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  int key = event->key;
  EVENT_TYPE type = event->type;
  switch(type)
    {
    case ET_KEY_PRESSED:
      switch(key)
	{
	case KEY_F(8):
	  endwin();
	  exit(EXIT_SUCCESS);
	  break;
	default:
	  wm_mgr->focus->handler(wm_mgr, NULL, wm_mgr->focus, event);
	}
    }
  debug_log_exit(FINE, func_name);
}

PTR_WND_MANAGER wm_init(void)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  WINDOW* win1;
  WINDOW* win2;
  PTR_WND_MANAGER wm_mgr = (PTR_WND_MANAGER)malloc(sizeof(WND_MANAGER));
  wm_mgr->focus = NULL;
  wm_mgr->root_wnd_list = NULL;
  wm_mgr->pop = wm_pop;
  wm_mgr->push = wm_push;
  wm_mgr->show = wm_refresh;
  wm_mgr->handler = wm_handler;
  initscr();
  getmaxyx(stdscr, wm_mgr->height, wm_mgr->width);
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0); /* invisible cursor */
  debug_log_exit(FINE, func_name);
  return wm_mgr;
}


PTR_WND wnd_init(PTR_WND_MANAGER wm_mgr, PTR_WND parent, const char* title, uint32_t height, uint32_t width, uint32_t y, uint32_t x)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  PTR_WND wnd = (PTR_WND) malloc(sizeof(WND));
  PTR_WND current = NULL;

  wnd->title = strdup(title);
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

void wnd_weibo_field_addstr(PTR_WND self, uint32_t y, uint32_t x, char* str, uint32_t limit)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  char* p = (char*)malloc(sizeof(char)*(limit+1));
  char* p1 = p;
  uint32_t orig_x = x;
  uint32_t orig_y = y;
  strncpy(p, str, limit);
  *(p+limit+1)='\0';
  while(*p && y < self->height-2) {
    wmove(self->curses_wnd, (int)y, (int)x);
    waddch(self->curses_wnd, *p); 
    getyx(self->curses_wnd, y, x);
    if (x >= self->width-2) {
      y += 1;
      x = orig_x;
    }
    p++;
  }
  free(p1);
  debug_log_exit(FINE, func_name);
}

void wnd_weibo_field_fillup(PTR_WND self, uint32_t y, uint32_t x, char c)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  uint32_t orig_x = x;
  uint32_t orig_y = y;
  while(y < self->height-1) {
    wmove(self->curses_wnd, (int)y, (int)x);
    waddch(self->curses_wnd, c);
    getyx(self->curses_wnd, y, x);
    if (x >= self->width-2) {
      y += 1;
      x = orig_x;
    }
  }
  debug_log_exit(FINE, func_name);
}

void wnd_alert_handler(PTR_WND_MANAGER wm_mgr, PTR_WND src, PTR_WND dst, PTR_EVENT event)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  PTR_WND wnd = wm_mgr->root_wnd_list;

  switch(event->type)
    {
    case ET_KEY_PRESSED:
      switch(event->key)
        {
        case KEY_ENTER:
	case 0012:
	  werase(dst->curses_wnd);
	  wrefresh(dst->curses_wnd);
	  wm_mgr->pop(wm_mgr, dst);
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

void wnd_alert_refresh(PTR_WND_MANAGER wm_mgr, PTR_WND self, void* data)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  touchwin(self->curses_wnd);
  wrefresh(self->curses_wnd);
  debug_log_exit(FINE, func_name);
}

void wnd_alert(PTR_WND_MANAGER wm_mgr, char* text)
{
  uint32_t l = (uint32_t) strlen(text);
  l += 5;
  PTR_WND wnd = wnd_init(wm_mgr, NULL, "alert", 4, l>50?50:l, (wm_mgr->height-4)/2, (wm_mgr->width-50)/2);
  wnd->handler = wnd_alert_handler;
  wnd->show = wnd_alert_refresh;
  wborder(wnd->curses_wnd, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  wnd_weibo_field_addstr(wnd, 1, 2, text, l);
  wrefresh(wnd->curses_wnd);
}


void wnd_weibo_initializer(PTR_WND self)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  char s[10];
  PTR_WND wnd;
  PTR_WEIBO_ENTITY weibo;
  uint32_t i = 0;
  uint32_t weibo_field_cnt = 5;
  uint32_t weibo_field_width = self->width-4;
  uint32_t weibo_field_height = ((WEIBO_CONTENT_LIMIT) / weibo_field_width) + 4;
  self->type = WT_PANEL;
  wborder(self->curses_wnd, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  wrefresh(self->curses_wnd);
  for (i=0; i<weibo_field_cnt; i++) {
    snprintf(s, 10, "%s-%ud", "weibo", i);
    wnd = wnd_init(self->wm_mgr, self, s, weibo_field_height, weibo_field_width, i*weibo_field_height+1, 1);
    wborder(wnd->curses_wnd, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
    wrefresh(wnd->curses_wnd);
  }
  wattron(self->children->curses_wnd, A_REVERSE);
  self->usrdata = (void*)get_public_timeline(ACCESS_TOKEN, 1);
  wnd = self->children;
  weibo = (PTR_WEIBO_ENTITY)(self->usrdata);
  while(wnd && weibo) {
    wnd_weibo_field_addstr(wnd, 1, 2, weibo->text, WEIBO_CONTENT_LIMIT);
    wrefresh(wnd->curses_wnd);
    wnd = wnd->next;
    weibo = weibo->next;
  }
  debug_log_exit(FINE, func_name);
}


void wnd_weibo_destroyer(PTR_WND self)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  PTR_WND wnd;
  wnd = self->children;
  while (wnd) {
    delwin(wnd->curses_wnd);
    free(wnd->title);
    wnd = wnd->next;
  }
  free(self->title);
  debug_log_exit(FINE, func_name);
}

void wnd_weibo_handler(PTR_WND_MANAGER wm_mgr, PTR_WND src, PTR_WND dst, PTR_EVENT event)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  static PTR_WND cursor = NULL;
  static PTR_WEIBO_ENTITY weibo = NULL;
  PTR_WEIBO_ENTITY weibo1 = NULL;
  PTR_WND wnd = dst->children;
  if (weibo == NULL) {
    weibo = (PTR_WEIBO_ENTITY)dst->usrdata;
  }
  if (cursor == NULL) {
    cursor = dst->children;
  }

  switch(event->type)
    {
    case ET_KEY_PRESSED:
      switch(event->key)
        {
        case KEY_F(1):
          break;
	case KEY_F(2):
	  break;
	case KEY_F(3):
	  break; 
	case KEY_DOWN:
	  wattroff(cursor->curses_wnd, A_REVERSE);
	  wnd_weibo_field_fillup(cursor, 1, 1, ' ');
	  if (weibo != NULL) {
	    wnd_weibo_field_addstr(cursor, 1, 2, weibo->text, WEIBO_CONTENT_LIMIT);
	  }
	  weibo = weibo==NULL?NULL:weibo->next;
	  if (weibo == NULL) {
	    destroy_weibo_entity_list((PTR_WEIBO_ENTITY)dst->usrdata);
	    switch(WEIBO_TYPE)
	      {
	      case 0:
		dst->usrdata = (void*)get_public_timeline(ACCESS_TOKEN, ++PAGE);
		break;
	      case 1:
		dst->usrdata = (void*)get_user_timeline_byid(ACCESS_TOKEN, USERID, ++PAGE);
		break;
	      case 2:
		dst->usrdata = (void*)get_friend_timeline(ACCESS_TOKEN, ++PAGE);
		break;
	      }
	    weibo = (PTR_WEIBO_ENTITY)(dst->usrdata);
	  }
	  cursor = cursor->next;
	  
	  if (cursor == NULL) {
	    cursor = dst->children;
	  }
	  wnd = cursor;
	  while (wnd) {
	    werase(wnd->curses_wnd);
	    wborder(wnd->curses_wnd, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
	    wnd = wnd->next;
	  }
	  wattron(cursor->curses_wnd, A_REVERSE);
	  wnd = cursor;
	  weibo1 = weibo;
	  while (wnd && weibo1) {
	    wnd_weibo_field_addstr(wnd, 1, 2, weibo1->text, WEIBO_CONTENT_LIMIT);
	    wrefresh(wnd->curses_wnd);
	    wnd = wnd->next;
	    weibo1 = weibo1->next;
	  }
	  break;
	case KEY_UP:
          wattroff(cursor->curses_wnd, A_REVERSE);
          wnd_weibo_field_fillup(cursor, 1, 1, ' ');
	  weibo = weibo==NULL?NULL:weibo->prev;
          if (weibo == NULL) {
	    destroy_weibo_entity_list((PTR_WEIBO_ENTITY)dst->usrdata);
            switch(WEIBO_TYPE)
              {
              case 0:
                dst->usrdata = (void*)get_public_timeline(ACCESS_TOKEN, PAGE==1?1:(--PAGE));
                break;
              case 1:
                dst->usrdata = (void*)get_user_timeline_byid(ACCESS_TOKEN, USERID, PAGE==1?1:(--PAGE));
                break;
              case 2:
                dst->usrdata = (void*)get_friend_timeline(ACCESS_TOKEN, PAGE==1?1:(--PAGE));
                break;
              }
            weibo = (PTR_WEIBO_ENTITY)(dst->usrdata);
          }
          cursor = cursor->prev;
          if (cursor == NULL) {
            cursor = dst->children;
	  }
	  wnd = cursor;
	  while (wnd) {
	    werase(wnd->curses_wnd);
	    wborder(wnd->curses_wnd, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
	    wnd = wnd->next;
	  }
	  wattron(cursor->curses_wnd, A_REVERSE);
	  wnd = cursor;
	  weibo1 = weibo;
	  while (wnd && weibo1) {
	    wnd_weibo_field_addstr(wnd, 1, 2, weibo1->text, WEIBO_CONTENT_LIMIT);
	    wrefresh(wnd->curses_wnd);
	    wnd = wnd->next;
	    weibo1 = weibo1->next;
	  }
          break;
        default:
	  break;
        }
    }
  debug_log_exit(FINE, func_name);
}

void wnd_weibo_refresh(PTR_WND_MANAGER wm_mgr, PTR_WND self, void* data)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  PTR_WND wnd = self->children;
  touchwin(self->curses_wnd);
  wrefresh(self->curses_wnd);
  while(wnd) {
    touchwin(wnd->curses_wnd);
    wrefresh(wnd->curses_wnd);
    wnd = wnd->next;
  }
  debug_log_exit(FINE, func_name);
}

void wnd_popinput(PTR_WND_MANAGER wm_mgr, PTR_WND parent)
{

}

void wnd_popinput_handler(PTR_WND_MANAGER wm_mgr, PTR_WND src, PTR_WND dst, PTR_EVENT event)
{

}

