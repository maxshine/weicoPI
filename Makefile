MAINPATH := $(shell pwd)
OUTPATH := $(MAINPATH)/build

CFLAGS := -I $(MAINPATH)/include -c
LDFLAGS := -lcurl

ifdef DEBUG
CFLAGS += -g -O1
endif


vpath %.c $(MAINPATH)/src/api/:$(MAINPATH)/src/debug/:$(MAINPATH)/src/util/:$(MAINPATH)/src/
vpath %.h $(MAINPATH)/include/
vpath %.o $(MAINPATH)/build/

objects := main.o weibo_oath2.o debug_util.o header_util.o init.o weibo_write.o

weicoPi : $(objects)
	cd $(OUTPATH);gcc $(LDFLAGS) $^ -o $(MAINPATH)/$@

main.o : init.c init.h main.c weibo_oath2.c weibo_oath2.h debug_util.h debug_util.c header_util.c header_util.h constants.h
	gcc $(CFLAGS) $(MAINPATH)/src/main.c -o $(OUTPATH)/main.o

weibo_oath2.o : weibo_oath2.c weibo_oath2.h debug_util.h debug_util.c header_util.c header_util.h constants.h
	gcc $(CFLAGS) $(MAINPATH)/src/api/weibo_oath2.c -o $(OUTPATH)/weibo_oath2.o 

weibo_write.o : weibo_write.c weibo_write.h debug_util.h debug_util.c constants.h
	gcc $(CFLAGS) $(MAINPATH)/src/api/weibo_write.c -o $(OUTPATH)/weibo_write.o 

debug_util.o : debug_util.h debug_util.c constants.h
	gcc $(CFLAGS) $(MAINPATH)/src/debug/debug_util.c -o $(OUTPATH)/debug_util.o

header_util.o : header_util.c header_util.h constants.h
	gcc $(CFLAGS) $(MAINPATH)/src/util/header_util.c -o $(OUTPATH)/header_util.o

init.o : init.c init.h constants.h
	gcc $(CFLAGS) $(MAINPATH)/src/util/init.c -o $(OUTPATH)/init.o

.PHONY : all

all : weicoPi

clean :
	rm -f $(OUTPATH)/*.o
