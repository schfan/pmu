#include "v7_pmu.h"
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/syscalls.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/fcntl.h>
#include <asm/uaccess.h>
#include <linux/delay.h>

static unsigned long long int cycle0,insn0,cycle,insn;
static long long int insns[500];
static long long int cycles[500];
//static struct timeval tv; 
static int ready, sleep_time, loop_time, loop_num;
static int stop;

void pmu_restart(){
	disable_ccnt();            // Stop CCNT
	disable_pmn(0);            // Stop counter 0
	enable_pmu();              // Enable the PMU
	reset_ccnt();              // Reset the CCNT (cycle counter)
	reset_pmn();               // Reset the configurable counters
	pmn_config(0, 0x08);       // Configure counter 0 to count event code 0x03
	insn = 0;
	cycle = 0;
	enable_ccnt();             // Enable CCNT
	enable_pmn(0);             // Enable counter
}

void auto_run(void) {
	int j = 0; //control the time of monitoring
	//signal(SIGCHLD, SIG_IGN);
	//if (fork() == 0 )
	//	execv("/data/coremark.exe",NULL);
	int i = 0; //control the granularity
	for (j = 0; j < loop_num; j++){//0.05s
		pmu_restart();
		insn0 = 0;
		cycle0 = 0;
		i = 0;
		for (i = 0; i < loop_time; i++){
			insn = read_pmn(0); // Read counter 0
			cycle = read_ccnt(); // Read CCNT
			cycles[i] = cycle - cycle0;
			insns[i] = insn - insn0;
			cycle0 = cycle;
			insn0 = insn;
			udelay(sleep_time);
		}
		ready = 1;
	}
	stop = 1;
	printk("Reading Finished.\n");
}

void auto_write(void){	
	struct file *file;
	int i;
	int length;
	char buf[30];
	loff_t pos = 0;
	int fd;
	char *filename = "/data/ipc.txt";
	mm_segment_t old_fs = get_fs();
	length = loop_time;
	
	set_fs(KERNEL_DS);

	fd = sys_open(filename, O_WRONLY|O_CREAT, 0644);
	if (fd >= 0) {
		file = fget(fd);
		if (file) {
			for (i = 0; i < length; i++){
				sprintf(buf, "%lld %lld\n", cycles[i], insns[i]);
				vfs_write(file, buf, strlen(buf), &pos);
				pos = pos + strlen(buf);
			}
			fput(file);
		}
		sys_close(fd);
	}
	set_fs(old_fs);
}	

int __init init_module(void) {
	sleep_time = 1000;//100us
	loop_time = 500;
	loop_num = 3;
	stop = 0;
	printk("I am loaded.\n");
	auto_run();
//	auto_write();
	return 0;
}

void cleanup_module(void){
	printk("Successfully unloaded.\n");
}


MODULE_LICENSE("GPL");
