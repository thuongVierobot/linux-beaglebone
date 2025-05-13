EXTRA_FLAGS = -Wall
obj-m = sysfs.o
KERNEL := /home/bbb/BBB/kernelbuildscripts/KERNEL
TOOLCHAIN:= /home/bbb/BBB/gcc-linaro-6.5.0-2018.12-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-

all:
	make ARCH=arm CROSS_COMPILE=$(TOOLCHAIN) -C $(KERNEL) M=`pwd` modules
clean:
	make -C $(KERNEL) M=`pwd` clean
