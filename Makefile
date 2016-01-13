MAINPATH := $(shell pwd)
OUTPATH := $(MAINPATH)/build

CFLAGS := -I $(MAINPATH)/include -c
LDFLAGS := -lcurl -lm

ifdef DEBUG
CFLAGS += -g -O0
endif


vpath %.c $(MAINPATH)/src/api/:$(MAINPATH)/src/debug/:$(MAINPATH)/src/util/:$(MAINPATH)/src/cJSON/:$(MAINPATH)/src/
vpath %.h $(MAINPATH)/include/
vpath %.o $(MAINPATH)/build/

objects := main.o weibo_oath2.o init.o weibo.o comment.o user.o account.o friend.o cJSON.o weibo_util.o http_action.o http_util.o debug_util.o  

jsons := cJSON.h cJSON.c

debug := debug_util.h debug_util.c

http := http_action.h http_action.c http_util.h http_util.c

common := constants.h $(debug) datatype.h weibo_datatype.h

api := weibo_oath2.c weibo_oath2.h weibo.h weibo.c comment.h comment.c friend.h friend.c user.h user.c account.c account.h

util := weibo_util.h weibo_util.c

weicoPi : $(objects)
	cd $(OUTPATH);gcc $(LDFLAGS) $^ -o $(MAINPATH)/$@

main.o : init.c init.h main.c $(api) $(common)
	gcc $(CFLAGS) $(MAINPATH)/src/main.c -o $(OUTPATH)/main.o

weibo_oath2.o : weibo_oath2.c weibo_oath2.h $(common) $(http)
	gcc $(CFLAGS) $(MAINPATH)/src/api/weibo_oath2.c -o $(OUTPATH)/weibo_oath2.o 

weibo_write.o : weibo_write.c weibo_write.h $(common) $(http)
	gcc $(CFLAGS) $(MAINPATH)/src/api/weibo_write.c -o $(OUTPATH)/weibo_write.o 

weibo_read.o : weibo_read.c weibo_write.h $(common) $(json) $(http) $(util)
	gcc $(CFLAGS) $(MAINPATH)/src/api/weibo_read.c -o $(OUTPATH)/weibo_read.o 

weibo.o : weibo.c weibo.h $(common) $(json) $(http) $(util)
	gcc $(CFLAGS) $(MAINPATH)/src/api/weibo.c -o $(OUTPATH)/weibo.o 

comment.o : comment.c comment.h $(common) $(json) $(http) $(util)
	gcc $(CFLAGS) $(MAINPATH)/src/api/comment.c -o $(OUTPATH)/comment.o 

friend.o : friend.c friend.h $(common) $(json) $(http) $(util)
	gcc $(CFLAGS) $(MAINPATH)/src/api/friend.c -o $(OUTPATH)/friend.o 

user.o : user.c user.h $(common) $(json) $(http) $(util)
	gcc $(CFLAGS) $(MAINPATH)/src/api/user.c -o $(OUTPATH)/user.o 

account.o : account_read.c account_read.h $(common) $(json) $(http) $(util)
	gcc $(CFLAGS) $(MAINPATH)/src/api/account_read.c -o $(OUTPATH)/account_read.o 

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
