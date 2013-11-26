obj-m += ipckernel.o
ipckernel-objs := ipc.o v7_pmu.o
KERNEL_DIR = /home/spica/work/linaro-kernel
PWD = $(shell pwd)

AS = arm-linux-gnueabi-as
CC = arm-linux-gnueabi-gcc

all: 
	$(MAKE) ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- -C $(KERNEL_DIR) SUBDIRS=$(PWD) modules

v7_pmu.o: v7_pmu.S
	$(AS) -o -static $@ $<


