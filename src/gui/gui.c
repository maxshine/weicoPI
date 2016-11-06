#define _XOPEN_SOURCE_EXTENDED /* To get ncurses wide char support */
#define __USE_XOPEN_EXTENDED

#include <curses.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "constants.h"
#include "datatype.h"
#include "weibo_datatype.h"
#include "gui_datatype.h"
#include "gui_generic.h"
#include "gui_textfield.h"
#include "gui_util.h"
#include "weibo.h"
#include "weibo_util.h"
#include "comment.h"
#include "user.h"
#include "friend.h"
#include "account.h"
#include "debug_util.h"

#define WEIBO_FIELDS_CNT 5

extern char* ACCESS_TOKEN;
extern char* USERID;
extern char* ACCOUNTID;
extern char* FRIENDID;
extern uint32_t PAGE;  /* start with page #1 */
extern uint32_t WEIBO_TYPE; /* 0 --  public timeline; 1 -- user timeline; 2 -- friend timeline*/

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
    event.user_data = NULL;
    (wm_mgr->handler)(wm_mgr, NULL, wm_mgr->focus, &event);
    (wm_mgr->wmrefresh)(wm_mgr, NULL);
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
  if (wnd->destroyer) {
    wnd->destroyer(wnd);
  } else { 
    free(wnd->title);
    delwin(wnd->curses_wnd);
    free(wnd);
  }
  wnd = wm_mgr->root_wnd_list;

  while(wnd->next) {
    wnd = wnd->next;
  }
  wm_mgr->focus = wnd;
  debug_log_exit(FINE, func_name);
}

void wm_show(PTR_WND_MANAGER wm_mgr, void* data)
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

void wm_refresh(PTR_WND_MANAGER wm_mgr, void* data)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  PTR_WND wnd = wm_mgr->root_wnd_list;
  while (wnd != NULL) {
    if (wnd->wndrefresh) {
      wnd->wndrefresh(wm_mgr, wnd, NULL);
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
	  deinit_debug_log();
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
  wm_mgr->show = wm_show;
  wm_mgr->wmrefresh = wm_refresh;
  wm_mgr->handler = wm_handler;
  initscr();
  getmaxyx(stdscr, wm_mgr->height, wm_mgr->width);
  cbreak(); /* initially, the screen should disable line buffer */
  noecho(); /* the screen should disable input character echo by default */
  keypad(stdscr, TRUE);
  mvaddstr(wm_mgr->height-1, 0, "F1 -- Post  F2 -- Report  F3 -- Comment  F8 -- Exit");
  curs_set(0); /* invisible cursor */
  refresh();
  debug_log_exit(FINE, func_name);
  return wm_mgr;
}

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
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  uint32_t l = (uint32_t) strlen(text);
  l += 5;
  PTR_WND wnd = wnd_init(wm_mgr, NULL, "alert", 4, l>50?50:l, (wm_mgr->height-4)/2, (wm_mgr->width-50)/2);
  wnd->handler = wnd_alert_handler;
  wnd->show = wnd_alert_refresh;
  wborder(wnd->curses_wnd, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  wnd_generic_addstr_w(wnd, 1, 2, text, l);
  wrefresh(wnd->curses_wnd);
  debug_log_exit(FINE, func_name);
}

void wnd_popinput_handler(PTR_WND_MANAGER wm_mgr, PTR_WND src, PTR_WND dst, PTR_EVENT event)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);

  debug_log_exit(FINE, func_name);
}

void wnd_popinput_refresh(PTR_WND_MANAGER wm_mgr, PTR_WND self, void* data)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  touchwin(self->curses_wnd);
  wrefresh(self->curses_wnd);
  debug_log_exit(FINE, func_name);
}


char* wnd_popinput(PTR_WND_MANAGER wm_mgr, PTR_WND parent, uint32_t height, uint32_t width)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  EVENT event;
  char* p = NULL;
  PTR_WND wnd = wnd_init(wm_mgr, NULL, "input", height+2, width+4, (wm_mgr->height-height)/2, (wm_mgr->width-width)/2);
  PTR_WND subwnd = wnd_init(wm_mgr, wnd, "inputfield", height, width, 1, 2);
  wnd->handler = wnd_popinput_handler;
  wnd->show = wnd_popinput_refresh;
  wnd->parent = (void*)parent;
  wnd->user_data = (void*)malloc(sizeof(char)*subwnd->height*subwnd->width);
  wborder(wnd->curses_wnd, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  curs_set(True);
  echo();
  wmove(subwnd->curses_wnd, 0, 0);
  wrefresh(wnd->curses_wnd);
  wrefresh(subwnd->curses_wnd);
  wgetnstr(subwnd->curses_wnd, (char*)wnd->user_data, subwnd->height*subwnd->width-1);
  debug_log(FINE, func_name, (char*)wnd->user_data);
  noecho();
  curs_set(False);
  werase(subwnd->curses_wnd);
  werase(wnd->curses_wnd);
  wrefresh(subwnd->curses_wnd);
  wrefresh(wnd->curses_wnd);
  /*
  event.type = ET_INPUT_COMPLETE;
  event.usrdata = wnd->usrdata;
  wnd->parent->handler(wm_mgr, wnd, wnd->parent, &event);
  */
  p = (char*)strdup((wnd->user_data));
  delwin(subwnd->curses_wnd);
  free(subwnd->title);
  free(subwnd);
  free(wnd->user_data);
  wm_mgr->pop(wm_mgr, wnd);
  debug_log_exit(FINE, func_name);
  return p;
}


void wnd_weibo_handler(PTR_WND_MANAGER wm_mgr, PTR_WND src, PTR_WND dst, PTR_EVENT event)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  static PTR_WND cursor = NULL;
  static PTR_WEIBO_ENTITY weibo = NULL;
  static uint32_t action = 0; /* 0 -- F1 post new weibo; 1 -- F2 comment weibo; 2 -- F3 repost weibo */
  PTR_WEIBO_ENTITY weibo1 = NULL;
  PTR_WND wnd = dst->children;
  char* p = NULL;
  if (weibo == NULL) {
    weibo = (PTR_WEIBO_ENTITY)dst->user_data;
  }
  if (cursor == NULL) {
    cursor = dst->children;
  }

  switch(event->type)
    {
    case ET_INPUT_COMPLETE:
      if (action == 0) {
	create_weibo_text((const char*)ACCESS_TOKEN, (const char*) event->user_data);
      } else if (action == 2) {
	repost_weibo((const char*)ACCESS_TOKEN, (const char*) event->user_data, (const char*) weibo->idstr);
      } else if (action == 1) {
	create_comment((const char*)ACCESS_TOKEN, (const char *)weibo->idstr, (const char *)event->user_data);
      }
      break;
    case ET_KEY_PRESSED:
      switch(event->key)
        {
        case KEY_F(1):
	  action = 0;
	  p = wnd_popinput(wm_mgr, dst, 3, 50);
	  if(create_weibo_text((const char*)ACCESS_TOKEN, (const char*) p)) {
	    wnd_alert(wm_mgr, "Weibo posted");
	  } else {
	    wnd_alert(wm_mgr, "Failure");
	  }
	  free(p);
	  p = NULL;
          break;
	case KEY_F(2):
	  action = 1;
	  p = wnd_popinput(wm_mgr, dst, 3, 50);
	  if(repost_weibo((const char*)ACCESS_TOKEN, (const char*) p, (const char*) weibo->idstr)) {
            wnd_alert(wm_mgr, "Weibo reposted");
          } else {
            wnd_alert(wm_mgr, "Failure");
          }
	  free(p);
	  p = NULL;
	  break;
	case KEY_F(3):
	  action = 2;
	  p = wnd_popinput(wm_mgr, dst, 3, 50);
	  if(create_comment((const char*)ACCESS_TOKEN, (const char *)weibo->idstr, (const char *)p)) {
            wnd_alert(wm_mgr, "Comment posted");
          } else {
            wnd_alert(wm_mgr, "Failure");
          }
	  free(p);
	  p = NULL;
	  break; 
	case KEY_DOWN:
	  weibo = weibo==NULL?NULL:weibo->next;
	  if (weibo == NULL) {
	    destroy_weibo_entity_list((PTR_WEIBO_ENTITY)dst->user_data);
	    switch(WEIBO_TYPE)
	      {
	      case 0:
		dst->user_data = (void*)get_public_timeline(ACCESS_TOKEN, ++PAGE);
		break;
	      case 1:
		dst->user_data = (void*)get_user_timeline_byid(ACCESS_TOKEN, USERID, ++PAGE);
		break;
	      case 2:
		dst->user_data = (void*)get_friend_timeline(ACCESS_TOKEN, ++PAGE);
		break;
	      }
	    weibo = (PTR_WEIBO_ENTITY)(dst->user_data);
	  }
	  cursor = cursor->next;
	  
	  /* next paging */
	  if (cursor == NULL) {
	    cursor = dst->children;
	    weibo1 = weibo;
	    wnd = cursor;
	    while (wnd) {
	      if (weibo1 == NULL) {
		wnd->user_data = NULL;
	      } else {
		wnd->user_data = weibo1->text;
	      }
	      wnd = wnd->next;
	      weibo1 = weibo1==NULL?NULL:weibo1->next;
	    }
	  }
	  dst->show(wm_mgr, dst, cursor);
	  break;
	case KEY_UP:
	  weibo = weibo==NULL?NULL:weibo->prev;
          if (weibo == NULL) {
	    destroy_weibo_entity_list((PTR_WEIBO_ENTITY)dst->user_data);
            switch(WEIBO_TYPE)
              {
              case 0:
                dst->user_data = (void*)get_public_timeline(ACCESS_TOKEN, PAGE==1?1:(--PAGE));
                break;
              case 1:
                dst->user_data = (void*)get_user_timeline_byid(ACCESS_TOKEN, USERID, PAGE==1?1:(--PAGE));
                break;
              case 2:
                dst->user_data = (void*)get_friend_timeline(ACCESS_TOKEN, PAGE==1?1:(--PAGE));
                break;
              }
            weibo = (PTR_WEIBO_ENTITY)(dst->user_data);
          }
          cursor = cursor->prev;
	  /* previous paging */
          if (cursor == NULL) {
            cursor = dst->children;
	    weibo1 = weibo;
	    wnd = cursor;
	    while (wnd) {
              if (weibo1 == NULL) {
                wnd->user_data = NULL;
              } else {
                wnd->user_data = weibo1->text;
              }
              wnd = wnd->next;
              weibo1 = weibo1==NULL?NULL:weibo1->next;	      
	    }
	  }
	  dst->show(wm_mgr, dst, cursor);
          break;
        default:
	  break;
        }
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
    wnd->destroyer(wnd);
    wnd = wnd->next;
  }
  free(self->title);
  delwin(self->curses_wnd);
  free(self);
  debug_log_exit(FINE, func_name);
}

/* data is a WND  pointer, indicate which field to be highlited */
void wnd_weibo_show(PTR_WND_MANAGER wm_mgr, PTR_WND self, void* data)
{
  const char* func_name = __func__;
  PTR_WND wnd = self->children;
  uint8_t i = 0;
  int attrs_reverse = WA_REVERSE;
  int attrs_normal = WA_NORMAL;
  debug_log_enter(FINE, func_name, NULL);
  werase(self->curses_wnd);
  wborder(self->curses_wnd,ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  touchwin(self->curses_wnd);
  wrefresh(self->curses_wnd);
  while(wnd) {
    if ((PTR_WND)data == wnd) {
      wnd->show(wm_mgr, wnd, &attrs_reverse);
    } else {
      wnd->show(wm_mgr, wnd, &attrs_normal);
    }
    i++;
    wnd = wnd->next;
  }
  debug_log_exit(FINE, func_name);
}

void wnd_weibo_refresh(PTR_WND_MANAGER wm_mgr, PTR_WND self, void* data)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, NULL);
  PTR_WND wnd = self->children;
  int attrs_reverse = WA_REVERSE;
  int attrs_normal = WA_NORMAL;
  touchwin(self->curses_wnd);
  wrefresh(self->curses_wnd);
  
  while(wnd) {
    if ((PTR_WND)data == wnd) {
      wnd->wndrefresh(wm_mgr, wnd, &attrs_reverse);
    } else {
      wnd->wndrefresh(wm_mgr, wnd, &attrs_normal);
    }
    wnd = wnd->next;
  }
  debug_log_exit(FINE, func_name);
}

/* Draw the weibo window initially */
void wnd_weibo_initializer(PTR_WND self)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "p", self);
  char s[10];
  PTR_WND wnd = NULL;
  PTR_WEIBO_ENTITY weibo = NULL;
  uint32_t i = 0;
  uint32_t weibo_field_cnt = WEIBO_FIELDS_CNT;
  uint32_t weibo_field_width = 0;
  uint32_t weibo_field_height = 0;

  self->type = WT_PANEL;
  self->initializer = wnd_weibo_initializer;
  self->destroyer = wnd_weibo_destroyer;
  self->show = wnd_weibo_show;
  self->handler = wnd_weibo_handler;
  self->wndrefresh = wnd_weibo_refresh;
  self->user_data = NULL;
  self->children = NULL;

  weibo_field_width = self->width - 4;
  weibo_field_height = ((WEIBO_CONTENT_LIMIT) / weibo_field_width) + 2;
  for (i=0; i<weibo_field_cnt; i++) {
    snprintf(s, 10, "%s-%u", "weibo", i);
    wnd = wnd_textfield_create(self->wm_mgr, self, weibo_field_height, weibo_field_width, i*weibo_field_height+1, 2, s);
  }

  self->user_data = (void*)get_public_timeline(ACCESS_TOKEN, PAGE);
  wnd = self->children;
  weibo = (PTR_WEIBO_ENTITY)(self->user_data);
  while(wnd && weibo) {
    wnd->user_data = weibo->text;
    wnd = wnd->next;
    weibo = weibo->next;
  }
  debug_log_exit(FINE, func_name);
}

PTR_WND wnd_weibo_create(PTR_WND_MANAGER wm_mgr, PTR_WND parent, uint32_t height, uint32_t width, uint32_t y, uint32_t x)
{
  const char* func_name = __func__;
  debug_log_enter(FINE, func_name, "ppdddd", wm_mgr, parent, height, width, y, x);

  PTR_WND wnd = wnd_init(wm_mgr, NULL, "weibo", wm_mgr->height-2, wm_mgr->width/2, 0, 0);
  wnd_weibo_initializer(wnd);
  debug_log_exit(FINE, func_name);
  return wnd;
}
