# character device driver

EXTRA_CFLAGS = -Wall
obj-m += blink.o

BBB_KERNEL := /home/bbb/BBB/kernelbuildscripts/KERNEL
TOOLCHAIN := /home/bbb/BBB/kernelbuildscripts/dl/gcc-8.5.0-nolibc/arm-linux-gnueabi/bin/arm-linux-gnueabi-

all:
	make ARCH=arm CROSS_COMPILE=$(TOOLCHAIN) -C $(BBB_KERNEL) M=$(shell pwd) modules
	
clean:
	make -C $(BBB_KERNEL) M=$(shell pwd) clean
