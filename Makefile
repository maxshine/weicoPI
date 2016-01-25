MAINPATH := $(shell pwd)
OUTPATH := $(MAINPATH)/build

CFLAGS := -I $(MAINPATH)/include -c
LDFLAGS := -lcurl -lm -lcurses

ifdef DEBUG
CFLAGS += -g -O0
endif


vpath %.c $(MAINPATH)/src/gui/:$(MAINPATH)/src/api/:$(MAINPATH)/src/debug/:$(MAINPATH)/src/util/:$(MAINPATH)/src/cJSON/:$(MAINPATH)/src/
vpath %.h $(MAINPATH)/include/
vpath %.o $(MAINPATH)/build/

objects := main.o gui.o init.o weibo.o comment.o user.o account.o friend.o cJSON.o weibo_util.o http_action.o http_util.o debug_util.o  

jsons := cJSON.h cJSON.c

debug := debug_util.h debug_util.c

http := http_action.h http_action.c http_util.h http_util.c

common := constants.h $(debug) datatype.h weibo_datatype.h gui_datatype.h

api := weibo.h weibo.c comment.h comment.c friend.h friend.c user.h user.c account.c account.h

util := weibo_util.h weibo_util.c

gui := gui.c gui.h

weicoPi : $(objects)
	cd $(OUTPATH);gcc $(LDFLAGS) $^ -o $(MAINPATH)/$@

main.o : init.c init.h main.c $(api) $(common)
	gcc $(CFLAGS) $(MAINPATH)/src/main.c -o $(OUTPATH)/main.o

gui.o : $(gui) $(api) $(common)
	gcc $(CFLAGS) $(MAINPATH)/src/gui/gui.c -o $(OUTPATH)/gui.o

weibo.o : weibo.c weibo.h $(common) $(json) $(http) $(util)
	gcc $(CFLAGS) $(MAINPATH)/src/api/weibo.c -o $(OUTPATH)/weibo.o 

comment.o : comment.c comment.h $(common) $(json) $(http) $(util)
	gcc $(CFLAGS) $(MAINPATH)/src/api/comment.c -o $(OUTPATH)/comment.o 

friend.o : friend.c friend.h $(common) $(json) $(http) $(util)
	gcc $(CFLAGS) $(MAINPATH)/src/api/friend.c -o $(OUTPATH)/friend.o 

user.o : user.c user.h $(common) $(json) $(http) $(util)
	gcc $(CFLAGS) $(MAINPATH)/src/api/user.c -o $(OUTPATH)/user.o 

account.o : account.c account.h $(common) $(json) $(http) $(util)
	gcc $(CFLAGS) $(MAINPATH)/src/api/account.c -o $(OUTPATH)/account.o 

debug_util.o : debug_util.h debug_util.c constants.h
	gcc $(CFLAGS) $(MAINPATH)/src/debug/debug_util.c -o $(OUTPATH)/debug_util.o

http_action.o : http_action.c http_action.h constants.h
	gcc $(CFLAGS) $(MAINPATH)/src/util/http_action.c -o $(OUTPATH)/http_action.o

http_util.o : http_util.c http_util.h constants.h
	gcc $(CFLAGS) $(MAINPATH)/src/util/http_util.c -o $(OUTPATH)/http_util.o

cJSON.o : cJSON.h cJSON.c
	gcc $(CFLAGS) $(MAINPATH)/src/cJSON/cJSON.c -o $(OUTPATH)/cJSON.o

init.o : init.c init.h constants.h
	gcc $(CFLAGS) $(MAINPATH)/src/util/init.c -o $(OUTPATH)/init.o

weibo_util.o : weibo_util.h weibo_util.c $(cJSON) $(common)
	gcc $(CFLAGS) $(MAINPATH)/src/util/weibo_util.c -o $(OUTPATH)/weibo_util.o

.PHONY : all

all : weicoPi

clean :
	rm -f $(OUTPATH)/*.o
	rm -f $(MAINPATH)/weicoPi
