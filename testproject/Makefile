#交叉编译工具链名称及路径
#CROSS_COMPILE = arm-linux-
CROSS_COMPILE = /usr/local/arm/arm-2009q3/bin/arm-none-linux-gnueabi-

AS			= $(CROSS_COMPILE)as
LD			= $(CROSS_COMPILE)ld
CC			= $(CROSS_COMPILE)gcc
CPP			= $(CC) -E
AR			= $(CROSS_COMPILE)ar
NM			= $(CROSS_COMPILE)nm
STRIP		= $(CROSS_COMPILE)strip
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump

#导出 给子目录下Makefile使用
export AS LD CC CPP AR NM
export STRIP OBJCOPY OBJDUMP

#编译器编译时编译设置
CFLAGS := -Wall -O2 -g -DDEBUG
#添加头文件路径
CFLAGS += -I $(shell pwd)/include -I/opt/libdecode/include

#连接器的连接参数设置
LDFLAGS :=-ljpeg -lz -lpng -L/opt/libdecode/lib

export CFLAGS LDFLAGS

TOPDIR := $(shell pwd)
export TOPDIR

#定义将来编译生产的可执行程序文件名
TARGET :=imageplayer

#添加项目中所有用到的源文件，有顶层的.c和子文件夹
obj-y +=main.o

#添加顶层目录下的子文件夹（目录名后加/ )
obj-y +=display/
obj-y +=image_manage/
all:
	make -C ./ -f $(TOPDIR)/Makefile.build
	$(CC) $(LDFLAGS) -o $(TARGET) built-in.o
	
cp:
	cp ../testproject/ /root/porting_x210/rootfs/rootfs/ -rf


clean:
	rm -f $(shell find -name "*.o")
	rm -f $(TARGET)

distclean:
	rm -f $(shell find -name "*.o")
	rm -f $(shell find -name "*.d")
	rm -f $(TARGET)