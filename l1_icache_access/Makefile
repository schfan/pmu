CC = arm-linux-gnueabi-gcc
AS = arm-linux-gnueabi-as

v7_pmu.o: v7_pmu.S
	$(AS) -o -static $@ $<

l1_icache_access: l1_icache_access.c v7_pmu.h v7_pmu.o
	$(CC) -static l1_icache_access.c -o l1_icache_access.o v7_pmu.o

