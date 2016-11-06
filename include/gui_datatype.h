#define _XOPEN_SOURCE_EXTENDED /* To get ncurses wide char support */
#define __USE_XOPEN_EXTENDED

#include <curses.h>
#include <stdint.h>

#ifndef GUI_DATATYPE_H

#define GUI_DATATYPE_H

typedef enum event_type {
  ET_KEY_PRESSED,
  ET_DATA_REFRESH,
  ET_WINDOW_REFRESH,
  ET_INPUT_COMPLETE
} EVENT_TYPE;

typedef struct event {
  void* user_data;
  int key;
  EVENT_TYPE type;
} EVENT, *PTR_EVENT;

typedef enum mywindow_type {
  WT_TEXT_FIELD,
  WT_INPUT_FIELD,
  WT_PANEL,
  WT_ALERT,
  WT_GENERIC
} WND_TYPE;

struct mywindow_manager;


typedef struct mywindow {
  char* title;
  uint32_t x;
  uint32_t y;
  uint32_t abs_x;
  uint32_t abs_y;
  uint32_t width;
  uint32_t height;

  WND_TYPE type;
  struct mywindow_manager* wm_mgr;
  WINDOW* curses_wnd;
  void (*initializer)(struct mywindow*);
  void (*destroyer)(struct mywindow*);
  void (*handler)(struct mywindow_manager*, struct mywindow*, struct mywindow*, struct event*);
  void (*show)(struct mywindow_manager*, struct mywindow*, void*);
  void (*wndrefresh)(struct mywindow_manager*, struct mywindow*, void*);

  void* user_data;

  struct mywindow* parent;
  struct mywindow* children;
  struct mywindow* prev;
  struct mywindow* next;
  
} WND, *PTR_WND;

typedef struct mywindow_manager {
  uint32_t width;
  uint32_t height;
  struct mywindow* root_wnd_list;
  struct mywindow* focus;
  void (*handler)(struct mywindow_manager*, struct mywindow*, struct mywindow*, struct event*);
  void (*pop)(struct mywindow_manager*, void*);
  void (*push)(struct mywindow_manager*, void*);
  void (*show)(struct mywindow_manager*, void*);
  void (*wmrefresh)(struct mywindow_manager*, void*);;
} WND_MANAGER, *PTR_WND_MANAGER;

#endif
