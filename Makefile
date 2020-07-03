CC = gcc
CPP=g++
CFLAGS = -Wall -g -O0 -std=C++11
CFLAG = -Werror


all:driverControler

objs = can_dev.o v4l2.o main.o sensor.o global.o threadt.o
 
dep_files :=$(wildcard $(dep_files))
#生成目标文件out也是默认生成的文件
driverControler :$(objs)
	$(CPP) -o driverControler $^ -pthread
#如果dep_files不为空，说明有依赖文件，包含进来

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $< -MD -MF .$@.d 
clean:
	rm *.o driverControler 
#删除依赖
disclean:
	rm $(dep_files)
.PHONY:clean


