#ifndef _BASE_HPP__
#define _BASE_HPP__
#include <cstdint>

#define ERROR "There is No Operation !!!"
#define MISS -1

/* Global Variables */
extern int R[32];
extern uint32_t pc;
extern uint32_t predict_pc;
extern uint32_t memory[0x4000000];
extern uint32_t instruction;
extern uint32_t BTB[0x4000000]; 
extern int GHR;
extern uint32_t PHT[0x4000000]; // index for TakenHistory
extern uint32_t TakenHistory[0x4000000];
extern int imm;
extern int ALU_Result;
extern int bcond ;
extern int opcode;
extern int rs, rt, rd;
extern int shamt, funct;
extern int idx;
extern uint32_t mem_addr;
extern uint32_t tag;
extern uint32_t cache_idx;
extern uint32_t offset;

/* Result count */
extern int jump_count;
extern int branch_count;
extern int not_branch_count;
extern int total_inst_count;
extern int R_inst , I_inst , J_inst;
extern int memory_access_count;
extern int register_count;
extern int predict_count;
extern int mispredict_count;
extern int cpu_clock;
extern int total_cycle;
extern int cache_hit;
extern int cache_miss;

#endif 