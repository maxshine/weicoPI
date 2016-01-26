#include <curses.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
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
extern uint32_t PAGE;
extern uint32_t WEIBO_TYPE; /* 0 --  public timeline; 1 -- user; 2 -- friend timeline*/

void wm_runloop(PTR_WND_MANAGER wm_mgr)
{
  int key = 0;
  EVENT event;
  while (TRUE) {
    key = getch();
    event.key = key;
    event.type = ET_KEY_PRESSED;
    event.userdata = NULL;
    (wm_mgr->handler)(wm_mgr, NULL, wm_mgr->focus, &event);
    (wm_mgr->show)(wm_mgr, NULL);
  }
}

void wm_push(PTR_WND_MANAGER wm_mgr, void* data)
{
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
}

void wm_pop(PTR_WND_MANAGER wm_mgr, void* data)
{
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
}

void wm_refresh(PTR_WND_MANAGER wm_mgr, void* data)
{
  clear();
  PTR_WND wnd = wm_mgr->root_wnd_list;
  while (wnd != NULL) {
    wborder(wnd->curses_wnd, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
    wnd->show(wm_mgr, wnd, NULL);
    wrefresh(wnd->curses_wnd);
    wnd = wnd->next;
  }
  refresh();
}

void wm_handler(PTR_WND_MANAGER wm_mgr, PTR_WND src, PTR_WND dst, PTR_EVENT event)
{
  int key = event->key;
  EVENT_TYPE type = event->type;
  switch(type)
    {
    case ET_KEY_PRESSED:
      switch(key)
	{
	case KEY_F(8):
	  exit(EXIT_SUCCESS);
	  break;
	default:
	  wm_mgr->focus->handler(wm_mgr, NULL, wm_mgr->focus, event);
	}
    }
}

PTR_WND_MANAGER wm_init(void)
{
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
  win1 = derwin(stdscr, 20, 50, 0, 0);
  win2 = derwin(stdscr, 20, 50, 21,0);
  wborder(win1, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  wborder(win2, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  waddstr(win1, "This is window 1");
  waddstr(win2, "This is window 2");
  wrefresh(win1);
  wrefresh(win2);
  return wm_mgr;
}


PTR_WND wnd_init(PTR_WND_MANAGER wm_mgr, PTR_WND parent, const char* title, uint32_t y, uint32_t x, uint32_t height, uint32_t width)
{
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
    wnd->curses_wnd = derwin(parent->curses_wnd, (int)(wnd->height), (int)(wnd->width), wnd->height+1, 1);
  } else {
    wnd->curses_wnd = newwin((int)height, (int)width, wnd->abs_y, wnd->abs_x);
    wm_mgr->push(wm_mgr, wnd);
  }
  return wnd;
}

void wnd_weibo_initializer(PTR_WND self)
{
  char s[10];
  PTR_WND wnd;
  PTR_WEIBO_ENTITY weibo;
  uint32_t i = 0;
  uint32_t weibo_field_cnt = 5;
  uint32_t weibo_field_width = self->width-2;
  uint32_t weibo_field_height = ((WEIBO_CONTENT_LIMIT) / weibo_field_width) + 2;
  self->type = WT_PANEL;
  for (i=0; i<weibo_field_cnt; i++) {
    snprintf(s, 10, "%s-%ud", "weibo", i);
    wnd = wnd_init(self->wm_mgr, self, s, 1, 1, weibo_field_height, weibo_field_width);
  }
  self->usrdata = (void*)get_public_timeline(ACCESS_TOKEN, 1);
  wnd = self->children;
  weibo = (PTR_WEIBO_ENTITY)(self->usrdata);
  while(wnd) {
    wborder(wnd->curses_wnd, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
    waddstr(wnd->curses_wnd, weibo->text);
    wrefresh(wnd->curses_wnd);
    wnd = wnd->next;
    weibo = weibo->next;
  }
}

void wnd_weibo_destroyer(PTR_WND self)
{
  PTR_WND wnd;
  wnd = self->children;
  while (wnd) {
    delwin(wnd->curses_wnd);
    free(wnd->title);
    wnd = wnd->next;
  }
  free(self->title);
}

void wnd_weibo_handler(PTR_WND_MANAGER wm_mgr, PTR_WND src, PTR_WND dst, PTR_EVENT event)
{
  static PTR_WND cursor = NULL;
  static PTR_WEIBO_ENTITY weibo = NULL;
  PTR_WND wnd = dst->children;

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
	  weibo = weibo->next;
	  if (weibo == NULL) {
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
	  wattrset(cursor->curses_wnd, A_NORMAL);
	  cursor = cursor->next;
	  if (cursor == NULL) {
	    cursor = dst->children;
	    wnd = dst->children;
            while (wnd) {
              werase(wnd->curses_wnd);
              wnd = wnd->next;
            }
            wnd = dst->children;
            while (wnd && weibo) {
              waddstr(wnd->curses_wnd, weibo->text);
              wnd = wnd->next;
              weibo = weibo->next;
            }
	  } else {
	    wattrset(cursor->curses_wnd, A_REVERSE);
	  }
	  break;
	case KEY_UP:
          weibo = weibo->prev;
          if (weibo == NULL) {
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
          wattrset(cursor->curses_wnd, A_NORMAL);
          cursor = cursor->prev;
          if (cursor == NULL) {
            cursor = dst->children;
            wnd = dst->children;
            while (wnd) {
              werase(wnd->curses_wnd);
              wnd = wnd->next;
            }
            wnd = dst->children;
            while (wnd && weibo) {
              waddstr(wnd->curses_wnd, weibo->text);
              wnd = wnd->next;
              weibo = weibo->next;
            }
          } else {
            wattrset(cursor->curses_wnd, A_REVERSE);
          }
          break;
        default:
	  break;
        }
    }
}

void wnd_weibo_refresh(PTR_WND_MANAGER wm_mgr, PTR_WND self, void* data)
{

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
