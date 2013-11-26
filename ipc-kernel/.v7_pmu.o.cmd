cmd_/home/spica/work/vexpress/code/pmu/ipc-kernel/v7_pmu.o := arm-linux-gnueabi-gcc -Wp,-MD,/home/spica/work/vexpress/code/pmu/ipc-kernel/.v7_pmu.o.d  -nostdinc -isystem /usr/lib/gcc-cross/arm-linux-gnueabi/4.7/include -I/home/spica/work/linaro-kernel/arch/arm/include -Iarch/arm/include/generated  -Iinclude -I/home/spica/work/linaro-kernel/arch/arm/include/uapi -Iarch/arm/include/generated/uapi -I/home/spica/work/linaro-kernel/include/uapi -Iinclude/generated/uapi -include /home/spica/work/linaro-kernel/include/linux/kconfig.h -D__KERNEL__ -mlittle-endian  -D__ASSEMBLY__ -mabi=aapcs-linux -mno-thumb-interwork -funwind-tables -mthumb -Wa,-mimplicit-it=always -Wa,-mno-warn-deprecated -Wa,-mthumb -D__LINUX_ARM_ARCH__=7 -march=armv7-a  -include asm/unified.h -msoft-float        -DMODULE  -c -o /home/spica/work/vexpress/code/pmu/ipc-kernel/v7_pmu.o /home/spica/work/vexpress/code/pmu/ipc-kernel/v7_pmu.S

source_/home/spica/work/vexpress/code/pmu/ipc-kernel/v7_pmu.o := /home/spica/work/vexpress/code/pmu/ipc-kernel/v7_pmu.S

deps_/home/spica/work/vexpress/code/pmu/ipc-kernel/v7_pmu.o := \
  /home/spica/work/linaro-kernel/arch/arm/include/asm/unified.h \
    $(wildcard include/config/arm/asm/unified.h) \
    $(wildcard include/config/thumb2/kernel.h) \

/home/spica/work/vexpress/code/pmu/ipc-kernel/v7_pmu.o: $(deps_/home/spica/work/vexpress/code/pmu/ipc-kernel/v7_pmu.o)

$(deps_/home/spica/work/vexpress/code/pmu/ipc-kernel/v7_pmu.o):
