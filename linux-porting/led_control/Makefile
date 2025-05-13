
EXTRA_FLAGS = -Wall
obj-m = led.o

BBB_KERNEL := /home/bbb/BBB/kernelbuildscripts/KERNEL
TOOLCHAIN := /home/bbb/BBB/gcc-linaro-6.5.0-2018.12-x86_64_arm-linux-gnueabihf/bin
PREFIX := arm-linux-gnueabihf-

CC := $(TOOLCHAIN)/$(PREFIX)gcc
all:
	make ARCH=arm CROSS_COMPILE=$(TOOLCHAIN)/$(PREFIX) -C $(BBB_KERNEL) M=$(shell pwd) modules
	$(CC) -o app user_app.c
clean:
	make -C $(BBB_KERNEL) M=$(shell pwd) clean
	rm -rf app
