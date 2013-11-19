CC = arm-linux-gnueabi-gcc
AS = arm-linux-gnueabi-as

v7_pmu.o: v7_pmu.S
	$(AS) -o -static $@ $<

ipc: ipc.c v7_pmu.h v7_pmu.o
	$(CC) -static ipc.c -o ipc.o v7_pmu.o

