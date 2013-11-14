#include "v7_pmu.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void write_to_file(void);
void pmu_start(void);
void pmu_stop(void);

static unsigned int event0, event1, event2, event3, event4, event5;

int main ( int argc, char *argv[] ){
	int run_id = 0;
	int 
	if ( argc != 2 ) {
		fputs ( "usage: $prog <id of test run>\n", stderr );
		exit ( EXIT_FAILURE );
	} 
	
	run_id = (int)strtol(argv[1], NULL, 10);
	
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
