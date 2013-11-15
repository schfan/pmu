#include <linux/module.h>
#include <linux/kernel.h>

#define PERF_DEF_OPTS (1 | 16)
static int OldState;

void enable_cpu_counters(void) {
	/* enable user-mode access */
	asm volatile("MCR p15, 0, %0, C9, C14, 0\n\t" :: "r"(1));
	/* Program PMU and enable all counters */
	asm volatile("mcr p15, 0, %0, c9, c12, 0" :: "r"(PERF_DEF_OPTS));
	asm volatile("mcr p15, 0, %0, c9, c12, 1" :: "r"(0x8000000f));
	/* disable counter overflow interrupts (just in case)*/
	asm volatile("MCR p15, 0, %0, C9, C14, 2\n\t" :: "r"(0x8000000f));
	printk(KERN_INFO "user-mode access to performance registers enabled\n");
}

int __init init_module(void) {
	on_each_cpu(enable_cpu_counters, NULL, 1);
	return 0;
}

void cleanup_module(void){
	printk(KERN_INFO "user-mode access to performance registers disabled\n");
}

MODULE_LICENSE("GPL");
