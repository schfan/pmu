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

static unsigned int event0, event1, event2, event3;
static unsigned long long int cycle_count, counter0, counter1, counter2, counter3;
static struct termios old, new;

void main() {
	event0 = INSTRUCTION;
	//	manual_run();
	auto_run();
}
void auto_run() {
	int i = 1;
	int j = 0; //control the time of monitorin
	for (i = 1; i < 14; i++) {
		set_events(i);	
	//	signal(SIGCHLD, SIG_IGN);
	//	if (fork() == 0 )
	//		execv("/data/coremark.exe",NULL);
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
			break;
		case 2:
			event1 = L1DCACHE_ACCESS;
			event2 = L1DTLB_MISS;
			event3 = RINSTRUCTION;
			break;
		case 3:
			event1 = WINSTRUCTION;
			event2 = INSTRUCTION;
			event3 = EXCEPTION;
			break; 
		case 4:
			event1 = EXCEPTION_RETURN;
			event2 = CONTEXTID;
			event3 = SOFT_PC;
			break;
		case 5:
			event1 = IMMEDIATE_BRANCH;
			event2 = PROCEDURE_RETURN;
			event3 = UNALIGNED_LS;
			break;
		case 6:
			event1 = MISPREDICTED_BRANCH;
			event2 = PREDICTABLE_BRANCH; 
			event3 = DATA_MEM_ACCESS;
			break;
		case 7:
			event1 = L1ICACHE_ACCESS;
			event2 = L1DCACHE_EVICTION;
			event3 = L2DCACHE_ACCESS;
			break;
		case 8:
			event1 = L2DCACHE_MISS;
			event2 = L2DCACHE_EVICTION;
			event3 = BUS_ACCESS;
			break;
		case 9:
			event1 = BUS_ACCESS_LD;
			event2 = BUS_ACCESS_ST;
			event3 = IRQ_EXCEPTION;
			break;
		case 10:
			event1 = FIQ_EXCEPTION;
			event2 = EXTERN_MEM_REQ;
			event3 = NONCACHABLE_EXTERN_MEM_REQ;
			break;
		case 11:
			event1 = LINEFILL_PREFETCH;
			event2 = LINEFILL_PREFETCH_DROPPED;
			event3 = READ_ALLOCATE_MD;
			break;
		case 12:
			event1 = ETM_EXT_OUT0;
			event2 = ETM_EXT_OUT1;
			event3 = WRITE_STL_BUF;
			break;
		case 13:
			event1 = DATA_SNOOPED;
			event2 = ZERO;
			event3 = ZERO;
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
	switch (id) {
		case 1:
			strcpy(string1, "L1ICACHE_MISS");
			strcpy(string2, "L1ITLB_MISS");
			strcpy(string3, "L1DCACHE_MISS");
			break;
		case 2:
			strcpy(string1, "L1DCACHE_ACCESS");
			strcpy(string2, "L1DTLB_MISS");
			strcpy(string3, "RINSTRUCTION");
			break;
		case 3:
			strcpy(string1, "WINSTRUCTION");
			strcpy(string2, "INSTRUCTION");
			strcpy(string3, "EXCEPTION");
			break; 
		case 4:
			strcpy(string1, "EXCEPTION_RETURN");
			strcpy(string2, "CONTEXTID");
			strcpy(string3, "SOFT_PC");
			break;
		case 5:
			strcpy(string1, "IMMEDIATE_BRANCH");
			strcpy(string2, "PROCEDURE_RETURN");
			strcpy(string3, "UNALIGNED_LS");
			break;
		case 6:
			strcpy(string1, "MISPREDICTED_BRANCH");
			strcpy(string2, "PREDICTABLE_BRANCH"); 
			strcpy(string3, "DATA_MEM_ACCESS");
			break;
		case 7:
			strcpy(string1, "L1ICACHE_ACCESS");
			strcpy(string2, "L1DCACHE_EVICTION");
			strcpy(string3, "L2DCACHE_ACCESS");
			break;
		case 8:
			strcpy(string1, "L2DCACHE_MISS");
			strcpy(string2, "L2DCACHE_EVICTION");
			strcpy(string3, "BUS_ACCESS");
			break;
		case 9:
			strcpy(string1, "BUS_ACCESS_LD");
			strcpy(string2, "BUS_ACCESS_ST");
			strcpy(string3, "IRQ_EXCEPTION");
			break;
		case 10:
			strcpy(string1, "FIQ_EXCEPTION");
			strcpy(string2, "EXTERN_MEM_REQ");
			strcpy(string3, "NONCACHABLE_EXTERN_MEM_REQ");
			break;
		case 11:
			strcpy(string1, "LINEFILL_PREFETCH");
			strcpy(string2, "LINEFILL_PREFETCH_DROPPED");
			strcpy(string3, "READ_ALLOCATE_MD");
			break;
		case 12:
			strcpy(string1, "ETM_EXT_OUT0");
			strcpy(string2, "ETM_EXT_OUT1");
			strcpy(string3, "WRITE_STL_BUF");
			break;
		case 13:
			strcpy(string1, "DATA_SNOOPED");
			strcpy(string2, "ZERO");
			strcpy(string3, "ZERO");
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
}

void pmu_start(){
	enable_pmu();              // Enable the PMU
	reset_ccnt();              // Reset the CCNT (cycle counter)
	reset_pmn();               // Reset the configurable counters
	pmn_config(0, event0);       // Configure counter 0 to count event code 0x03
	pmn_config(1, event1);       // Configure counter 1 to count event code 0x03
	pmn_config(2, event2);       // Configure counter 2 to count event code 0x03
	pmn_config(3, event3);       // Configure counter 3 to count event code 0x03

	enable_ccnt();             // Enable CCNT
	enable_pmn(0);             // Enable counter
	enable_pmn(1);             // Enable counter
	enable_pmn(2);             // Enable counter
	enable_pmn(3);             // Enable counter
}

void pmu_stop(void){
	disable_ccnt();            // Stop CCNT
	disable_pmn(0);            // Stop counter 0
	disable_pmn(1);            // Stop counter 1
	disable_pmn(2);            // Stop counter 2
	disable_pmn(3);            // Stop counter 3
	counter0    += read_pmn(0); // Read counter 0
	counter1    += read_pmn(1); // Read counter 1
	counter2    += read_pmn(2); // Read counter 2
	counter3    += read_pmn(3); // Read counter 3
	cycle_count += read_ccnt(); // Read CCNT
}

void pmu_cleanup(void){
	event1 = 0;
	event2 = 0;
	event3 = 0;
	counter0 = 0;
	counter1 = 0;
	counter2 = 0;
	counter3 = 0;
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



