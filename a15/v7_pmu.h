// Define events
#define L1ICACHE_MISS 0x01
#define L1ITLB_MISS 0x02
#define L1DCACHE_MISS 0x03
#define L1DCACHE_ACCESS 0X04
#define L1DTLB_MISS 0x05
#define RINSTRUCTION 0x06 //A7
#define WINSTRUCTION 0x07 //A7
#define INSTRUCTION 0x08
#define EXCEPTION 0x09
#define EXCEPTION_RETURN 0x0A 
#define CONTEXTID 0x0B
#define SOFT_PC 0x0C //A7
#define IMMEDIATE_BRANCH 0x0D //A7
#define PROCEDURE_RETURN 0x0E //A7
#define UNALIGNED_LS 0x0F //A7
#define MISPREDICTED_BRANCH 0x10 
#define CYCLE 0x11 
#define PREDICTABLE_BRANCH 0x12 
#define DATA_MEM_ACCESS 0x13
#define L1ICACHE_ACCESS 0x14
#define L1DCACHE_EVICTION 0x15
#define L2DCACHE_ACCESS 0x16
#define L2DCACHE_MISS 0x17
#define L2DCACHE_EVICTION 0x18
#define BUS_ACCESS 0x19
#define LOCAL_MEM_ERR 0x1A //A15
#define INST_SPECULATIVE 0x1B //A15
#define INST_TTB 0x1C //A15
#define BUS_CYCLE 0x1D //A15
#define L1DCACHE_ACCESS_LD 0x40 //A15
#define L1DCACHE_ACCESS_ST 0x41 //A15
#define L1DCACHE_MISS_LD 0x42 //A15
#define L1DCACHE_MISS_ST 0x43 //A15
#define L1DCACHE_EVICTION_LD 0x46 //A15
#define L1DCACHE_EVICTION_ST 0x47 //A15
#define L1DCACHE_INVALID 0x48 //A15
#define L1DTLB_MISS_LD 0x4C //A15
#define L1DTLB_MISS_ST 0x4D //A15
#define D2DCACHE_ACCESS_LD 0x50 //A15
#define D2DCACHE_ACCESS_ST 0x51 //A15
#define L2DCACHE_MISS_LD 0x52 //A15
#define L2DCACHE_MISS_ST 0x53 //A15
#define L2DCACHE_EVICTION_VICTIM 0x56 //A15
#define L2DCACHE_EVICTION_CLEAN 0x57 //A15
#define L2DCACHE_INVALID 0x58 //A15
#define BUS_ACCESS_LD 0x60 
#define BUS_ACCESS_ST 0x61
#define BUS_ACCESS_NORM 0x62 //A15
#define BUS_ACCESS_ANORM 0x63 //A15
#define BUS_ACCESS_NORM2 0x64 //A15
#define BUS_ACCESS_PERI 0x65 //A15
#define DATA_MEM_ACCESS_LD 0x66 //A15
#define DATA_MEM_ACCESS_ST 0x67 //A15
#define UNALIGNED_ACCESS_LD 0x68 //A15
#define UNALIGNED_ACCESS_ST 0x69 //A15
#define UNALIGNED_ACCESS 0x6A //A15
#define LDREX 0x6C //A15
#define STREX_PASS 0x6D //A15
#define STREX_FAIL 0x6E //A15
#define INSN_SPEC_LD 0x70 //A15
#define INSN_SPEC_ST 0x71 //A15
#define INSN_SPEC_LD_ST 0x72 //A15
#define INSN_SPEC_INT 0x73 //A15
#define INSN_SPEC_SIMD 0x74 //A15
#define INSN_SPEC_VFP 0x75 //A15
#define INSN_SPEC_SOFTPC 0x76 //A15
#define BRANCH_SPEC_IMMEDIATE 0x78 //A15
#define BRANCH_SPEC_PROCEDURE 0x79 //A15
#define BRANCH_SPEC_INDIRECT 0x7A //A15
#define BRANCH_SPEC_ISB 0x7C //A15
#define BRANCH_SPEC_DSB 0x7D //A15
#define BRANCH_SPEC_DMB 0x7E //A15
#define IRQ_EXCEPTION 0x86 //A7
#define FIQ_EXCEPTION 0x87 //A7
#define EXTERN_MEM_REQ 0xC0 //A7
#define NONCACHABLE_EXTERN_MEM_REQ 0xC1 //A7
#define LINEFILL_PREFETCH 0xC2 //A7
#define LINEFILL_PREFETCH_DROPPED 0xC3 //A7
#define READ_ALLOCATE_MD 0xC4 //A7
#define ETM_EXT_OUT0 0xC7 //A7
#define ETM_EXT_OUT1 0xC8 //A7
#define WRITE_STL_BUF 0xC9 //A7
#define DATA_SNOOPED 0xCA //A7

// ------------------------------------------------------------
// PMU for Cortex-A/R (v7-A/R)
// ------------------------------------------------------------

#ifndef _V7_PMU_H
#define _V7_PMU_H

// Returns the number of progammable counters
unsigned int getPMN(void);

// Sets the event for a programmable counter to record
// counter = r0 = Which counter to program  (e.g. 0 for PMN0, 1 for PMN1)
// event   = r1 = The event code (from appropiate TRM or ARM Architecture Reference Manual)
void pmn_config(unsigned int counter, unsigned int event);

// Enables/disables the divider (1/64) on CCNT
// divider = r0 = If 0 disable divider, else enable dvider
void ccnt_divider(int divider);

//
// Enables and disables
//

// Global PMU enable
// On ARM11 this enables the PMU, and the counters start immediately
// On Cortex this enables the PMU, there are individual enables for the counters
void enable_pmu(void);

// Global PMU disable
// On Cortex, this overrides the enable state of the individual counters
void disable_pmu(void);

// Enable the CCNT
void enable_ccnt(void);

// Disable the CCNT
void disable_ccnt(void);

// Enable PMN{n}
// counter = The counter to enable (e.g. 0 for PMN0, 1 for PMN1)
void enable_pmn(unsigned int counter);

// Enable PMN{n}
// counter = The counter to enable (e.g. 0 for PMN0, 1 for PMN1)
void disable_pmn(unsigned int counter);

//
// Read counter values
//

// Returns the value of CCNT
unsigned int read_ccnt(void);

// Returns the value of PMN{n}
// counter = The counter to read (e.g. 0 for PMN0, 1 for PMN1)
unsigned int read_pmn(unsigned int counter);

//
// Overflow and interrupts
//

// Returns the value of the overflow flags
unsigned int read_flags(void);

// Writes the overflow flags
void write_flags(unsigned int flags);

// Enables interrupt generation on overflow of the CCNT
void enable_ccnt_irq(void);

// Disables interrupt generation on overflow of the CCNT
void disable_ccnt_irq(void);

// Enables interrupt generation on overflow of PMN{x}
// counter = The counter to enable the interrupt for (e.g. 0 for PMN0, 1 for PMN1)
void enable_pmn_irq(unsigned int counter);

// Disables interrupt generation on overflow of PMN{x}
// counter = r0 =  The counter to disable the interrupt for (e.g. 0 for PMN0, 1 for PMN1)
void disable_pmn_irq(unsigned int counter);

//
// Counter reset functions
//

// Resets the programmable counters
void reset_pmn(void);

// Resets the CCNT
void reset_ccnt(void);

//
// Software Increment

// Writes to software increment register
// counter = The counter to increment (e.g. 0 for PMN0, 1 for PMN1)
void pmu_software_increment(unsigned int counter);

//
// User mode access
//

// Enables User mode access to the PMU (must be called in a priviledged mode)
void enable_pmu_user_access(void);

// Disables User mode access to the PMU (must be called in a priviledged mode)
void disable_pmu_user_access(void);

#endif
// ------------------------------------------------------------
// End of v7_pmu.h
// ------------------------------------------------------------
