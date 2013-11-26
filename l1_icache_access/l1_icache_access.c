#include "v7_pmu.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <string.h>

void write_to_file(void);
void pmu_start(void);
void pmu_stop(void);
void auto_run(void);

static unsigned long long int cycle_count, counter0;

static FILE *f;
static struct timeval tv; 
static unsigned long long millisSinceEpoch;

void main() {
	f = fopen("ipc.txt","w");
	if (f == NULL){	
		printf("Error opening file!\n");
		exit(1);
	}
	auto_run();
	fclose(f);
}
void auto_run() {
	int j = 0; //control the time of monitoring
	//signal(SIGCHLD, SIG_IGN);
	//if (fork() == 0 )
	//	execv("/data/coremark.exe",NULL);
	for (j = 0; j < 600; j++){
		pmu_start();
		usleep(100000);
		pmu_stop();
		gettimeofday(&tv, NULL); 
		millisSinceEpoch = (unsigned long long)(tv.tv_sec) * 1000 + (unsigned long long)(tv.tv_usec) / 1000;
		fprintf(f, "%lld %lld\n", millisSinceEpoch, counter0); 
	}
}

void pmu_start(){
	enable_pmu();              // Enable the PMU
	reset_ccnt();              // Reset the CCNT (cycle counter)
	reset_pmn();               // Reset the configurable counters
	pmn_config(0, 0x14);       // Configure counter 0 to count event code 0x03
	counter0 = 0;
	cycle_count = 0;
	enable_ccnt();             // Enable CCNT
	enable_pmn(0);             // Enable counter
}

void pmu_stop(void){
	disable_ccnt();            // Stop CCNT
	disable_pmn(0);            // Stop counter 0
	counter0    += read_pmn(0); // Read counter 0
	cycle_count += read_ccnt(); // Read CCNT
}
