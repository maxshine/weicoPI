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

objects := main.o weibo_oath2.o debug_util.o header_util.o http_util.o init.o weibo_write.o weibo_read.o user_read.o cJSON.o

jsons := cJSON.h cJSON.c

debug := debug_util.h debug_util.c

http := header_util.h header_util.c http_util.h http_util.c

common := constants.h $(debug)

api := weibo_oath2.c weibo_oath2.h weibo_write.h weibo_write.c weibo_read.h weibo_read.c user_read.h user_read.c

weicoPi : $(objects)
	cd $(OUTPATH);gcc $(LDFLAGS) $^ -o $(MAINPATH)/$@

main.o : init.c init.h main.c $(api) $(common)
	gcc $(CFLAGS) $(MAINPATH)/src/main.c -o $(OUTPATH)/main.o

weibo_oath2.o : weibo_oath2.c weibo_oath2.h $(common) $(http)
	gcc $(CFLAGS) $(MAINPATH)/src/api/weibo_oath2.c -o $(OUTPATH)/weibo_oath2.o 

weibo_write.o : weibo_write.c weibo_write.h $(common) $(http)
	gcc $(CFLAGS) $(MAINPATH)/src/api/weibo_write.c -o $(OUTPATH)/weibo_write.o 

weibo_read.o : weibo_read.c weibo_write.h $(common) $(json) $(http)
	gcc $(CFLAGS) $(MAINPATH)/src/api/weibo_read.c -o $(OUTPATH)/weibo_read.o 

user_read.o : user_read.c user_read.h $(common) $(json) $(http)
	gcc $(CFLAGS) $(MAINPATH)/src/api/user_read.c -o $(OUTPATH)/user_read.o 

debug_util.o : debug_util.h debug_util.c constants.h
	gcc $(CFLAGS) $(MAINPATH)/src/debug/debug_util.c -o $(OUTPATH)/debug_util.o

header_util.o : header_util.c header_util.h constants.h
	gcc $(CFLAGS) $(MAINPATH)/src/util/header_util.c -o $(OUTPATH)/header_util.o

http_util.o : http_util.c http_util.h constants.h
	gcc $(CFLAGS) $(MAINPATH)/src/util/http_util.c -o $(OUTPATH)/http_util.o

cJSON.o : cJSON.h cJSON.c
	gcc $(CFLAGS) $(MAINPATH)/src/cJSON/cJSON.c -o $(OUTPATH)/cJSON.o

init.o : init.c init.h constants.h
	gcc $(CFLAGS) $(MAINPATH)/src/util/init.c -o $(OUTPATH)/init.o

.PHONY : all

all : weicoPi

clean :
	rm -f $(OUTPATH)/*.o
