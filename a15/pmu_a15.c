#include "v7_pmu.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <termios.h>

void write_to_file(void);
void pmu_start(void);
void pmu_stop(void);
void initTERmios(int echo);
void resetTermios(void);
char getch_(int echo);
char getch(void);
char getche(void);

static unsigned int event0, event1, event2, event3, event4, event5;
static struct termios old, new;

int main (){
	int run_id = 0;
	printf("Select test run ID:  ");
	run_id = getche() - '0';
	//choosing events
	event0 = INSTRUCTION;
	switch (run_id) {
		case 1:
			event1 = L1ICACHE_MISS;
			event2 = L1ITLB_MISS;
			event3 = L1DCACHE_MISS;
			event4 = L1DCACHE_ACCESS;
			event5 = L1DTLB_MISS;
			char* string1 = "L1ICACHE_MISS";
			char* string2 = "L1ITLB_MISS";
			char* string3 = "L1DCACHE_MISS";
			char* string4 = "L1DCACHE_ACCESS";
			char* string5 = "L1DTLB_MISS";
			break;
		case 2:
			event1 = INSTRUCTION;
			event2 = EXCEPTION;
			event3 = EXCEPTION_RETURN;
			event4 = CONTEXTID;
			event5 = MISPREDICTED_BRANCH;
			break; 
		case 3:
			event1 = CYCLE;
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

		default:	

			printf("Default\n");
			break;
	}	
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

	printf("CountEvent0=0x%x,CountEvent1=0x%x,CountEvent2=0x%x,CountEvent3=0x%x,CountEvent4=0x%x,CountEvent5=0x%x\n", event0,event1,event2,event3,event4,event5);
}

void pmu_stop(void){
	unsigned int cycle_count, overflow, counter0, counter1, counter2, counter3, counter4, counter5;

	disable_ccnt();            // Stop CCNT
	disable_pmn(0);            // Stop counter 0
	disable_pmn(1);            // Stop counter 1
	disable_pmn(2);            // Stop counter 2
	disable_pmn(3);            // Stop counter 3
	disable_pmn(4);            // Stop counter 4
	disable_pmn(5);            // Stop counter 5

	counter0    = read_pmn(0); // Read counter 0
	counter1    = read_pmn(1); // Read counter 1
	counter2    = read_pmn(2); // Read counter 2
	counter3    = read_pmn(3); // Read counter 3
	counter4    = read_pmn(4); // Read counter 4
	counter5    = read_pmn(5); // Read counter 5

	cycle_count = read_ccnt(); // Read CCNT
	overflow=read_flags();        //Check for overflow flag

	printf("Counter0=%d,Counter1=%d,Counter2=%d,Counter3=%d,Counter4=%d,Counter5=%d\n", counter0, counter1,counter2,counter3,counter4,counter5);
	printf("Overflow flag: = %d, Cycle Count: = %d \n\n", overflow,cycle_count);
}
