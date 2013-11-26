CC = arm-linux-gnueabi-gcc
AS = arm-linux-gnueabi-as
pmu_a7: pmu_a7.c v7_pmu.h v7_pmu.o
	$(CC) -static -o pmu_a7.o pmu_a7.c v7_pmu.h v7_pmu.o

v7_pmu.o: v7_pmu.S
	$(AS) -o -static $@ $<


