CC = arm-linux-gnueabi-gcc
AS = arm-linux-gnueabi-as

v7_pmu.o: v7_pmu.S
	$(AS) -o -static $@ $<

pmu_a15: pmu_a15.c v7_pmu.h v7_pmu.o
	$(CC) -static pmu_a15.c -o pmu_a15.o v7_pmu.o

