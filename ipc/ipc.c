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
clock_t t;

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
	for (j = 0; j < 40; j++){
		pmu_start();
		usleep(500000);
		pmu_stop();
		t = clock();
		fprintf(f, "%f %f\n", (float)t, (float)counter0/cycle_count); 
	}
}

void pmu_start(){
	enable_pmu();              // Enable the PMU
	reset_ccnt();              // Reset the CCNT (cycle counter)
	reset_pmn();               // Reset the configurable counters
	pmn_config(0, 0x08);       // Configure counter 0 to count event code 0x03
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
