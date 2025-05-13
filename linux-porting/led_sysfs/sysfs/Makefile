EXTRA_CFLAGS = -Wall
obj-m = app.o

BBB_KERNEL := /home/bbb/BBB/kernelbuildscripts/KERNEL
TOOLCHAIN := /home/bbb/BBB/gcc-linaro-6.5.0-2018.12-x86_64_arm-linux-gnueabihf/bin
PREFIX := arm-linux-gnueabihf-

CC := $(TOOLCHAIN)/$(PREFIX)gcc

all: gpio.o app.o
	$(CC) -o app app.o gpio.o
gpio.o: gpio.c gpio.h
	$(CC) -o gpio.o -c gpio.c
app.o: app.c gpio.h
	$(CC) -o app.o -c app.c
clean:
	rm -rf *.o app

