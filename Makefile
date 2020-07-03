CFLAGS=-fPIC -g
EXCFLAGS:=
EXTERLIBS:=
CROSS_COMPILE = 

CFLAGS +=   -DPROJ_GATEWAY   -DGLOG_NO_ABBREVIATED_SEVERITIES

CC=$(CROSS_COMPILE)gcc
CPP=$(CROSS_COMPILE)g++
STRIP=$(CROSS_COMPILE)strip
AR=$(CROSS_COMPILE)ar

all:driverControler

export SRCDIR = $(shell pwd)
export OBJDIR = $(SRCDIR)/obj
#$(warning $(SRCDIR))
CC_SRCS = $(shell find $(SRCDIR) -type f -name '*.cc')
CPP_SRCS = $(shell find $(SRCDIR) -type f -name '*.cpp')
CC_OBJS = $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(CC_SRCS:%cc=%o))
CPP_OBJS = $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(CPP_SRCS:%cpp=%o))

CFLAGS += $(EXCFLAGS) -DLINUX   
LDFLAGS += $(EXLDFLAGS)  -lrt -lpthread   

LIBS = $(EXTERLIBS)  -lpthread  
LIB_OBJS1 = can_dev.o v4l2.o  sensor.o global.o threadt.o

CPPFLAGS += -Wall  -std=c++11

CCFLAGS=   -std=c++11 -I$(curdir)/mqttYun/glog -I./glog/ -I$(STAGING_DIR)/usr/include  -I$(STAGING_DIR)/include  -I$(curdir)/src/    -I./glog/ -lpthread -I./ -I$(STAGING_DIR)/usr/lib 

driverControler: main.cpp $(LIB_OBJS1)
	$(CPP)  $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -o $@ main.cpp $(LIB_OBJS1) ${LIBS} -lpthread
	$(STRIP) $@
#	rm -rf $(LIB_OBJS1)
	
%.o: %.c
	$(CC) $(CFLAGS) -c $< -lpthread

%.o: %.cpp
	$(CPP) $(CPPFLAGS) $(CFLAGS) -c $< 

%.o: %.cc
	$(CPP) $(CCFLAGS) -I./glog/ -c $< -o $@
	
clean:
	rm -rf driverControler *.o
	
	
