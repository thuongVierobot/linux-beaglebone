# Toolchain
TOOLCHAIN := /home/thuong/BBB/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf/bin
PREFIX := arm-linux-gnueabihf-

# Arch
#ifeq (${ARCH}, arm)
	CC := $(TOOLCHAIN)/$(PREFIX)gcc
#endif

all:
	$(CC) -o hello_arm main.c
	gcc -o hello_x86 main.c
clean:
	rm -rf hello_x86 hello_arm
