#include "v7_pmu.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <stdint.h>

void pmu_restart(void);
void *auto_run(void *args);
void *auto_write(void *args);

static unsigned long long int cycle0,insn0,cycle,insn;
static float ipc[500];
//static struct timeval tv; 
static uint64_t current_time[500];
static int ready, sleep_time, loop_time, loop_num;
static int stop;

void main() {
	pthread_t thread1, thread2;
	int iret1, iret2; 

	sleep_time = 1000;//100us
	loop_time = 500;
	loop_num = 3;
	stop = 0;

	iret1 = pthread_create(&thread1, NULL, auto_run, NULL);
	pthread_join(thread1, NULL);	
	iret2 = pthread_create(&thread2, NULL, auto_write, NULL);
	pthread_join(thread2, NULL);
	exit(0);
}

void *auto_run(void *args) {
	int j = 0; //control the time of monitoring
	//signal(SIGCHLD, SIG_IGN);
	//if (fork() == 0 )
	//	execv("/data/coremark.exe",NULL);
	int i = 0; //control the granularity
	struct timespec ts;
	for (j = 0; j < loop_num; j++){//0.05s
		pmu_restart();
		insn0 = 0;
		cycle0 = 0;
		i = 0;
		for (i = 0; i < loop_time; i++){
			insn = read_pmn(0); // Read counter 0
			cycle = read_ccnt(); // Read CCNT
			ipc[i] = (float)(insn - insn0)/(cycle - cycle0);
			clock_gettime(CLOCK_REALTIME, &ts);
			current_time[i] = (uint64_t)ts.tv_sec * 1000000LL + (uint64_t)ts.tv_nsec / 1000LL;
			cycle0 = cycle;
			insn0 = insn;
			usleep(sleep_time);
		}
		ready = 1;
	}
	stop = 1;
}

void *auto_write(void *args){
	FILE *f;
	struct timespec ts = {0};
	int i;
	int length;
	int read_sleep_time = 100000;//100ms
	//ts.tv_sec = 0;
	//ts.tv_nsec = 10000; //1us
	length = loop_time;
	f = fopen("ipc.txt","w");
	if (f == NULL){	
		printf("Error opening file!\n");
		exit(1);
	}
	//while(!stop) {	
	//	nanosleep(&ts, (struct timespec *)NULL);
//		usleep(read_sleep_time);
		if (ready == 1){
			for (i = 0; i < length; i++){
				fprintf(f, "%lld %f\n", (unsigned long long)current_time[i], ipc[i]);
			}
			ready = 0;
		}
//	}
	fclose(f);
}	

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

