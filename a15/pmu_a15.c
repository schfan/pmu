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
void manual_run(void);
void set_events(int id);
void print_counters(int id);
void pmu_cleanup(void);

void initTERmios(int echo);
void resetTermios(void);
char getch_(int echo);
char getch(void);
char getche(void);

static unsigned int event0, event1, event2, event3, event4, event5;
static unsigned long long int cycle_count, counter0, counter1, counter2, counter3, counter4, counter5;
static struct termios old, new;

void main() {
	event0 = INSTRUCTION;
	//	manual_run();
	auto_run();
	//manual_run();
}
void auto_run() {
	int i = 1;
	int j = 0; //control the time of monitoring
	for (i = 1; i < 14; i++) {
		set_events(i);	
		//signal(SIGCHLD, SIG_IGN);
		//if (fork() == 0 )
		//	execv("/data/coremark.exe",NULL);
		for (j = 0; j < 40; j++){
			pmu_start();
			usleep(500000);
			pmu_stop();
		}
		print_counters(i);
		pmu_cleanup();
	}
}

void manual_run (){
	int run_id = 0;
	int j = 0;
	printf("Select test run ID (1 - 13):  ");
	run_id = getche() - '0';
	//choosing events
	set_events(run_id);
	for (j = 0; j < 40; j++){
		pmu_start();
		usleep(500000);
		pmu_stop();
	}
	print_counters(run_id);
}

void set_events(int id) {
	switch (id) {
		case 1:
			event1 = L1ICACHE_MISS;
			event2 = L1ITLB_MISS;
			event3 = L1DCACHE_MISS;
			event4 = L1DCACHE_ACCESS;
			event5 = L1DTLB_MISS;
			break;
		case 2:
			event1 = INSTRUCTION;
			event2 = EXCEPTION;
			event3 = EXCEPTION_RETURN;
			event4 = CONTEXTID;
			event5 = MISPREDICTED_BRANCH;
			break; 
		case 3:
			event1 = BRANCH_SPEC_DMB;
			event2 = PREDICTABLE_BRANCH; 
			event3 = DATA_MEM_ACCESS;
			event4 = L1ICACHE_ACCESS;
			event5 = L1DCACHE_EVICTION;
			break;
		case 4:
			event1 = L2DCACHE_ACCESS;
			event2 = L2DCACHE_MISS;
			event3 = L2DCACHE_EVICTION;
			event4 = BUS_ACCESS;
			event5 = LOCAL_MEM_ERR;
			break;
		case 5:
			event1 = INST_SPECULATIVE;
			event2 = INST_TTB;
			event3 = BUS_CYCLE;
			event4 = L1DCACHE_ACCESS_LD;
			event5 = L1DCACHE_ACCESS_ST;
			break;
		case 6:
			event1 = L1DCACHE_MISS_LD;
			event2 = L1DCACHE_MISS_ST;
			event3 = L1DCACHE_EVICTION_LD;
			event4 = L1DCACHE_EVICTION_ST;
			event5 = L1DCACHE_INVALID;
			break;
		case 7:
			event1 = L1DTLB_MISS_LD;
			event2 = L1DTLB_MISS_ST;
			event3 = L2DCACHE_ACCESS_LD;
			event4 = L2DCACHE_ACCESS_ST;
			event5 = L2DCACHE_MISS_LD;
			break;
		case 8:
			event1 = L2DCACHE_MISS_ST;
			event2 = L2DCACHE_EVICTION_VICTIM;
			event3 = L2DCACHE_EVICTION_CLEAN;
			event4 = L2DCACHE_INVALID;
			event5 = BUS_ACCESS_LD;
			break;
		case 9:
			event1 = BUS_ACCESS_ST;
			event2 = BUS_ACCESS_NORM;
			event3 = BUS_ACCESS_ANORM;
			event4 = BUS_ACCESS_NORM2;
			event5 = BUS_ACCESS_PERI;
			break;
		case 10:
			event1 = DATA_MEM_ACCESS_LD;
			event2 = DATA_MEM_ACCESS_ST;
			event3 = UNALIGNED_ACCESS_LD;
			event4 = UNALIGNED_ACCESS_ST;
			event5 = UNALIGNED_ACCESS;
			break;
		case 11:
			event1 = LDREX;
			event2 = STREX_PASS;
			event3 = STREX_FAIL;
			event4 = INSN_SPEC_LD;
			event5 = INSN_SPEC_ST;
			break;
		case 12:
			event1 = INSN_SPEC_LD_ST;
			event2 = INSN_SPEC_INT;
			event3 = INSN_SPEC_SIMD;
			event4 = INSN_SPEC_VFP;
			event5 = INSN_SPEC_SOFTPC;
			break;
		case 13:
			event1 = BRANCH_SPEC_IMMEDIATE;
			event2 = BRANCH_SPEC_PROCEDURE;
			event3 = BRANCH_SPEC_INDIRECT;
			event4 = BRANCH_SPEC_ISB;
			event5 = BRANCH_SPEC_DSB;
			break;
		default:	
			printf("No valid test run ID. \n");
			break;
	}	
}

void print_counters(int id) {	
	char string1[30];
	char string2[30];
	char string3[30];
	char string4[30];
	char string5[30];
	switch (id) {
		case 1:
			strcpy(string1, "L1ICACHE_MISS");
			strcpy(string2, "L1ITLB_MISS");
			strcpy(string3, "L1DCACHE_MISS");
			strcpy(string4, "L1DCACHE_ACCESS");
			strcpy(string5, "L1DTLB_MISS");
			break;
		case 2:
			strcpy(string1, "INSTRUCTION");
			strcpy(string2, "EXCEPTION");
			strcpy(string3, "EXCEPTION_RETURN");
			strcpy(string4, "CONTEXTID");
			strcpy(string5, "MISPREDICTED_BRANCH");
			break; 
		case 3:
			strcpy(string1, "BRANCH_SPEC_DMB");
			strcpy(string2, "PREDICTABLE_BRANCH"); 
			strcpy(string3, "DATA_MEM_ACCESS");
			strcpy(string4, "L1ICACHE_ACCESS");
			strcpy(string5, "L1DCACHE_EVICTION");
			break;
		case 4:
			strcpy(string1, "L2DCACHE_ACCESS");
			strcpy(string2, "L2DCACHE_MISS");
			strcpy(string3, "L2DCACHE_EVICTION");
			strcpy(string4, "BUS_ACCESS");
			strcpy(string5, "LOCAL_MEM_ERR");
			break;
		case 5:
			strcpy(string1, "INST_SPECULATIVE");
			strcpy(string2, "INST_TTB");
			strcpy(string3, "BUS_CYCLE");
			strcpy(string4, "L1DCACHE_ACCESS_LD");
			strcpy(string5, "L1DCACHE_ACCESS_ST");
			break;
		case 6:
			strcpy(string1, "L1DCACHE_MISS_LD");
			strcpy(string2, "L1DCACHE_MISS_ST");
			strcpy(string3, "L1DCACHE_EVICTION_LD");
			strcpy(string4, "L1DCACHE_EVICTION_ST");
			strcpy(string5, "L1DCACHE_INVALID");
			break;
		case 7:
			strcpy(string1, "L1DTLB_MISS_LD");
			strcpy(string2, "L1DTLB_MISS_ST");
			strcpy(string3, "L2DCACHE_ACCESS_LD");
			strcpy(string4, "L2DCACHE_ACCESS_ST");
			strcpy(string5, "L2DCACHE_MISS_LD");
			break;
		case 8:
			strcpy(string1, "L2DCACHE_MISS_ST");
			strcpy(string2, "L2DCACHE_EVICTION_VICTIM");
			strcpy(string3, "L2DCACHE_EVICTION_CLEAN");
			strcpy(string4, "L2DCACHE_INVALID");
			strcpy(string5, "BUS_ACCESS_LD");
			break;
		case 9:
			strcpy(string1, "BUS_ACCESS_ST");
			strcpy(string2, "BUS_ACCESS_NORM");
			strcpy(string3, "BUS_ACCESS_ANORM");
			strcpy(string4, "BUS_ACCESS_NORM2");
			strcpy(string5, "BUS_ACCESS_PERI");
			break;
		case 10:
			strcpy(string1, "DATA_MEM_ACCESS_LD");
			strcpy(string2, "DATA_MEM_ACCESS_ST");
			strcpy(string3, "UNALIGNED_ACCESS_LD");
			strcpy(string4, "UNALIGNED_ACCESS_ST");
			strcpy(string5, "UNALIGNED_ACCESS");
			break;
		case 11:
			strcpy(string1, "LDREX");
			strcpy(string2, "STREX_PASS");
			strcpy(string3, "STREX_FAIL");
			strcpy(string4, "INSN_SPEC_LD");
			strcpy(string5, "INSN_SPEC_ST");
			break;
		case 12:
			strcpy(string1, "INSN_SPEC_LD_ST");
			strcpy(string2, "INSN_SPEC_INT");
			strcpy(string3, "INSN_SPEC_SIMD");
			strcpy(string4, "INSN_SPEC_VFP");
			strcpy(string5, "INSN_SPEC_SOFTPC");
			break;
		case 13:
			strcpy(string1, "BRANCH_SPEC_IMMEDIATE");
			strcpy(string2, "BRANCH_SPEC_PROCEDURE");
			strcpy(string3, "BRANCH_SPEC_INDIRECT");
			strcpy(string4, "BRANCH_SPEC_ISB");
			strcpy(string5, "BRANCH_SPEC_DSB");
			break;
		default:	
			printf("No valid test run ID. \n");
			break;
	}
	if (id == 1){
		printf("Cycles %lld\n", cycle_count);
		printf("Instructions %lld\n", counter0);
		printf("IPC %f\n", (float)counter0/cycle_count); 
	}
	printf("%s %f\n", string1, (float)counter1*1000/counter0);	
	printf("%s %f\n", string2, (float)counter2*1000/counter0);	
	printf("%s %f\n", string3, (float)counter3*1000/counter0);	
	printf("%s %f\n", string4, (float)counter4*1000/counter0);	
	printf("%s %f\n", string5, (float)counter5*1000/counter0);	
}

void pmu_start(){
	enable_pmu();              // Enable the PMU
	reset_ccnt();              // Reset the CCNT (cycle counter)
	reset_pmn();               // Reset the configurable counters
	pmn_config(0, event0);       // Configure counter 0 to count event code 0x03
	pmn_config(1, event1);       // Configure counter 1 to count event code 0x03
	pmn_config(2, event2);       // Configure counter 2 to count event code 0x03
	pmn_config(3, event3);       // Configure counter 3 to count event code 0x03
	pmn_config(4, event4);       // Configure counter 4 to count event code 0x03
	pmn_config(5, event5);       // Configure counter 5 to count event code 0x03

	enable_ccnt();             // Enable CCNT
	enable_pmn(0);             // Enable counter
	enable_pmn(1);             // Enable counter
	enable_pmn(2);             // Enable counter
	enable_pmn(3);             // Enable counter
	enable_pmn(4);             // Enable counter
	enable_pmn(5);             // Enable counter
}

void pmu_stop(void){
	disable_ccnt();            // Stop CCNT
	disable_pmn(0);            // Stop counter 0
	disable_pmn(1);            // Stop counter 1
	disable_pmn(2);            // Stop counter 2
	disable_pmn(3);            // Stop counter 3
	disable_pmn(4);            // Stop counter 4
	disable_pmn(5);            // Stop counter 5
	counter0    += read_pmn(0); // Read counter 0
	counter1    += read_pmn(1); // Read counter 1
	counter2    += read_pmn(2); // Read counter 2
	counter3    += read_pmn(3); // Read counter 3
	counter4    += read_pmn(4); // Read counter 4
	counter5    += read_pmn(5); // Read counter 5
	cycle_count += read_ccnt(); // Read CCNT
}

void pmu_cleanup(void){
	event1 = 0;
	event2 = 0;
	event3 = 0;
	event4 = 0;	
	event5 = 0;
	counter0 = 0;
	counter1 = 0;
	counter2 = 0;
	counter3 = 0;
	counter4 = 0;
	counter5 = 0;
	cycle_count = 0;
}

/* Initialize new terminal i/o settings */
void initTermios(int echo) {
	tcgetattr(0, &old); /* grab old terminal i/o settings */
	new = old; /* make new settings same as old settings */
	new.c_lflag &= ~ICANON; /* disable buffered i/o */
	new.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
	tcsetattr(0, TCSANOW, &new); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) {
	tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) {
	char ch;
	initTermios(echo);
	ch = getchar();
	resetTermios();
	return ch;
}

/* Read 1 character without echo */
char getch(void) {
	return getch_(0);
}

/* Read 1 character with echo */
char getche(void) {
	return getch_(1);
}



