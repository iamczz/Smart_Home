SUBDIRS	:= cam gsm zgb lcd server main time

APPS	:= app
AOBJS	:= $(patsubst %.s, %.o, $(wildcard *.s))
COBJS	:= $(patsubst %.c, %.o, $(wildcard *.c))
CPPOBJS	:= $(patsubst %.cpp, %.o, $(wildcard *.cpp))
OBJS	:= $(AOBJS) $(COBJS) $(CPPOBJS)
CROSS	:= 
CC	:= $(CROSS)arm-none-linux-gnueabi-gcc#A9的交叉编译设置
#CC	:= $(CROSS)gcc
AR	:= $(CROSS)ar
INCLUDE	:= $(PWD)/include
CFLAGS = -std=gnu99 -Wall -g -I$(INCLUDE)
#CFLAGS	:= -Wall -O2 -g -I$(INCLUDE)
LDFLAGS	:= -lpthread -L$(PWD)/lib -lmain -lcam -lgsm  -lzgb -llcd -lserver -ltime -L/usr/local/lib/lib -ljpeg
TOPDIR	:= $(PWD)
BIN_DIR = bin

export CFLAGS TOPDIR CC

all: $(OBJS)

	mkdir -p $(BIN_DIR)
	@for dir in $(SUBDIRS) ; \
		do $(MAKE) -C $$dir all CC=$(CC) AR=$(AR); \
	done
	$(CC) -o $(BIN_DIR)/$(APPS)  $(OBJS) $(LDFLAGS)
	sudo cp $(BIN_DIR)/$(APPS) /tftpboot/rootfs/	
	
.PHONY: $(SUBDIRS)
clean:
	rm -f  ../$(BIN_DIR)/$(APPS) $(OBJS)
	@for dir in $(SUBDIRS) ; \
		do $(MAKE) -C $$dir clean ; \
	done
