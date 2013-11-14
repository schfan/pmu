obj-m += perfcnt_enable.o
KERNEL_DIR = /home/spica/work/linaro-kernel
PWD = $(shell pwd)
ccflags-y := -O2 -static 

default:
	$(MAKE) ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- -C $(KERNEL_DIR) SUBDIRS=$(PWD) modules 

clean:
	$(MAKE) -C $(KERNEL_DIR) SURDIRS=$(PWD) clean

 
