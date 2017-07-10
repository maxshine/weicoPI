MAINPATH := $(shell pwd)
SOURCEPATH := $(MAINPATH)/src
OUTPATH := $(MAINPATH)/build

CFLAGS := -I $(MAINPATH)/include -c -std=c99 $(shell ncursesw5-config --cflags)
LDFLAGS := -lcurl -lm $(shell ncursesw5-config --libs)

ifdef DEBUG
CFLAGS += -g -O0
endif


vpath %.c $(SOURCEPATH)/http/:$(SOURCEPATH)/gui/:$(SOURCEPATH)/api/:$(SOURCEPATH)/debug/:$(SOURCEPATH)/util/:$(SOURCEPATH)/cJSON/:$(SOURCEPATH)/
vpath %.h $(MAINPATH)/include/
vpath %.o $(MAINPATH)/build/

objects := main.o gui.o gui_alert.o gui_popinput.o gui_textfield.o gui_generic.o gui_util.o weibo_oath2.o init.o weibo.o comment.o user.o account.o friend.o cJSON.o weibo_util.o http_action.o http_util.o debug_util.o  

jsons := cJSON.h cJSON.c

debug := debug_util.h debug_util.c

http := http_action.h http_action.c http_util.h http_util.c

common := constants.h $(debug) datatype.h weibo_datatype.h gui_datatype.h

api := weibo_oath2.h weibo_oath2.c weibo.h weibo.c comment.h comment.c friend.h friend.c user.h user.c account.c account.h weibo_util.h weibo_util.c

oath2 := weibo_oath2.h weibo_oath2.c

util := init.h init.c

gui := gui.c gui.h gui_util.c gui_util.h gui_generic.c gui_generic.h gui_textfield.c gui_textfield.h gui_alert.h gui_alert.c gui_popinput.h gui_popinput.c

weicoPi : $(objects)
	cd $(OUTPATH);gcc $(LDFLAGS) $^ -o $(MAINPATH)/$@

main.o : main.c $(util) $(oath2) $(api) $(common) $(gui)
	gcc $(CFLAGS) $(SOURCEPATH)/main.c -o $(OUTPATH)/main.o

gui.o : $(gui) $(api) $(common)
	gcc $(CFLAGS) $(SOURCEPATH)/gui/gui.c -o $(OUTPATH)/gui.o

gui_alert.o : $(gui) $(common)
	gcc $(CFLAGS) $(SOURCEPATH)/gui/gui_alert.c -o $(OUTPATH)/gui_alert.o

gui_popinput.o : $(gui) $(common)
	gcc $(CFLAGS) $(SOURCEPATH)/gui/gui_popinput.c -o $(OUTPATH)/gui_popinput.o

gui_textfield.o : $(gui) $(common)
	gcc $(CFLAGS) $(SOURCEPATH)/gui/gui_textfield.c -o $(OUTPATH)/gui_textfield.o

gui_generic.o : $(gui) $(common)
	gcc $(CFLAGS) $(SOURCEPATH)/gui/gui_generic.c -o $(OUTPATH)/gui_generic.o

gui_util.o : $(gui) $(common)
	gcc $(CFLAGS) $(SOURCEPATH)/gui/gui_util.c -o $(OUTPATH)/gui_util.o

weibo_oath2.o : $(oath2) $(common) $(json) $(http) 
	gcc $(CFLAGS) $(SOURCEPATH)/api/weibo_oath2.c -o $(OUTPATH)/weibo_oath2.o 

weibo.o : weibo.c weibo.h $(common) $(json) $(http) $(util)
	gcc $(CFLAGS) $(SOURCEPATH)/api/weibo.c -o $(OUTPATH)/weibo.o 

comment.o : comment.c comment.h $(common) $(json) $(http) $(util)
	gcc $(CFLAGS) $(SOURCEPATH)/api/comment.c -o $(OUTPATH)/comment.o 

friend.o : friend.c friend.h $(common) $(json) $(http) $(util)
	gcc $(CFLAGS) $(SOURCEPATH)/api/friend.c -o $(OUTPATH)/friend.o 

user.o : user.c user.h $(common) $(json) $(http) $(util)
	gcc $(CFLAGS) $(SOURCEPATH)/api/user.c -o $(OUTPATH)/user.o 

account.o : account.c account.h $(common) $(json) $(http) $(util)
	gcc $(CFLAGS) $(SOURCEPATH)/api/account.c -o $(OUTPATH)/account.o 

debug_util.o : debug_util.h debug_util.c constants.h
	gcc $(CFLAGS) $(SOURCEPATH)/debug/debug_util.c -o $(OUTPATH)/debug_util.o

http_action.o : http_action.c http_action.h constants.h
	gcc $(CFLAGS) $(SOURCEPATH)/http/http_action.c -o $(OUTPATH)/http_action.o

http_util.o : http_util.c http_util.h constants.h
	gcc $(CFLAGS) $(SOURCEPATH)/http/http_util.c -o $(OUTPATH)/http_util.o

cJSON.o : cJSON.h cJSON.c
	gcc $(CFLAGS) $(SOURCEPATH)/cJSON/cJSON.c -o $(OUTPATH)/cJSON.o

init.o : $(util) $(common)
	gcc $(CFLAGS) $(SOURCEPATH)/util/init.c -o $(OUTPATH)/init.o

weibo_util.o : weibo_util.h weibo_util.c $(cJSON) $(common)
	gcc $(CFLAGS) $(SOURCEPATH)/api/weibo_util.c -o $(OUTPATH)/weibo_util.o

.PHONY : all clean

all : weicoPi

clean :
	rm -f $(OUTPATH)/*.o
	rm -f $(MAINPATH)/weicoPi
