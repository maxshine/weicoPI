#include <curses.h>
#include <stdint.h>

#ifndef GUI_DATATYPE_H

#define GUI_DATATYPE_H

typedef enum event_type {
  ET_KEY_PRESSED,
  ET_DATA_REFRESH,
  ET_WINDOW_REFRESH,
  ET_INPUT_COMPLETE
} EVENT_TYPE

typedef struct event {
  void* userdata;
  int key;
  EVENT_TYPE type;
} EVENT, *PTR_EVENT;

typedef void (*EVENT_HANDLER) (PTR_WND_MANAGER, PTR_WND, PTR_WND, PTR_EVENT);

typedef void (*WND_INITIALIZER) (PTR_WND);

typedef void (*WND_OPS) (PTR_WND_MANAGER, PTR_WND, void*);

typedef void (*WND_MANAGER_OPS) (PTR_WND_MANAGER, void*);

typedef enum mywindow_type {
  WT_TEXT_FIELD,
  WT_INPUT_FIELD,
  WT_PANEL
} WND_TYPE;

typedef struct mywindow {
  char* title;
  uint32_t x;
  uint32_t y;
  uint32_t abs_x;
  uint32_t abs_y;
  uint32_t width;
  uint32_t height;

  WND_TYPE type;
  PTR_WND_MANAGER wm_mgr;
  WINDOW* curses_wnd;
  WND_INITIALIZER initializer;
  WND_INITIALIZER destroyer;
  EVENT_HANDLER handler;
  WND_OPS refresh;

  void* usrdata;

  struct mywindow* parent;
  struct mywindow* children;
  struct mywindow* prev;
  struct mywindow* next
  
} WND, *PTR_WND;

typedef struct mywindow_manager {
  uint32_t screen_width;
  uint32_t screen_height;
  WND* root_wnd_list;
  WND* focus;
  EVENT_HANDLER handler;
  WND_MANAGER_OPS pop;
  WND_MANAGER_OPS push;
  WND_MANAGER_OPS refresh;
} WND_MANAGER, *PTR_WND_MANAGER;

#endif
