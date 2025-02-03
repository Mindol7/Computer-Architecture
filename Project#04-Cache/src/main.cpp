#include <iostream>
#include <fstream>
using namespace std;

#include "base.hpp"
#include "latch.hpp"
#include "stage.hpp"
#include "alu.hpp"
#include "addr.hpp"
#include "fr.hpp"
#include "cache.hpp"
#include "alg.hpp"

/* Global Variables */
int R[32] = {0, };
uint32_t pc= 0x00000000;
uint32_t predict_pc= 0;
uint32_t memory[0x4000000]= {0, };
uint32_t instruction= 0;
uint32_t BTB[0x4000000]= {0, };
int GHR= 0;
uint32_t PHT[0x4000000]= {0, }; // index for TakenHistory
uint32_t TakenHistory[0x4000000] = {0, };
int ALU_Result= 0;
int bcond = 0;
int opcode= 0;
int rs, rt, rd= 0;
int imm= 0;
int shamt, funct= 0;
int idx= 0;
uint32_t mem_addr = 0;
uint32_t tag;
uint32_t cache_idx;
uint32_t offset;

/* Result count */
int jump_count= 0;
int branch_count = 0;
int not_branch_count = 0;
int total_inst_count = 0;
int R_inst , I_inst , J_inst = 0;
int memory_access_count = 0;
int register_count = 0;
int predict_count = 0;
int mispredict_count = 0;
int cpu_clock = 0;
int total_cycle = 0;
int cache_hit = 0;
int cache_miss = 0;

int main(int argc, char * argv[]){
    int var, res, ret = 0x00000000;
    fstream input_file("../test_prog/gcd.bin", ios::in);

    if(input_file.fail() == true){
        cerr<<"Fail to open file"<<endl;
        exit(1);
    }

    /* Convert Little Endian to Big Endian */

    while(input_file.eof() != true){
        input_file.read(reinterpret_cast<char*>(&var), sizeof(var));
        ret = (int)var;
        uint32_t b1, b2, b3, b4;

        b1 = ret & 0x000000ff; b2 = ret & 0x0000ff00;
        b3 = ret & 0x00ff0000; b4 = ret & 0xff000000;

        res = (b1 << 24) | (b2 << 8) | (b3 >> 8) | (b4 >> 24);

        memory[idx] = res;
        idx++;
    }

    R[29] = 0x1000000; // Stack Pointer Register
    R[31] = 0xffffffff; // Return Register
    pc = 0; // Program Counter
    while(true){
        if(pc == 0xffffffff){
            cout<<"Program Halt"<<endl;
            break;
        }
        /* Start 1 Cycle */
        
        Fetch();
        Decode();
        Execution();
        MEM();
        WB(); 

        copy_latch(); // work after 1 cycle
        cout<<dec<<++total_cycle<<" Cycle is Finish"<<endl;
    }
    cout<<"\n=================================="<<endl;
    cout<<"===========PROGRAM RESULT==========="<<endl;
    cout<<"Total Cycle Count of Program : "<<dec<<total_cycle<<endl; 
    cout<<"The Result is R[2] : "<<dec<<R[2]<<endl;
    cout<<"Total Instruction Count : "<<total_inst_count-1<<endl; // pc가 0xffff:ffff로 EXE되기전 fetch로 invalid값 가지므로 count를 -1 해줌
    cout<<"R-Type Instruction Count : "<<R_inst<<endl;
    cout<<"I-Type Instruction Count : "<<I_inst<<endl;
    cout<<"J-Type Instruction Count : "<<J_inst<<endl;
    cout<<"Jump Count : "<<jump_count<<endl;
    cout<<"Branch Count : "<<branch_count<<endl;
    cout<<"Memory Access Count : "<<memory_access_count<<endl;
    cout<<"Prediction Count : "<<predict_count+mispredict_count<<endl;
    cout<<"Predict Count : "<<predict_count<<endl;
    cout<<"Mispredict Count : "<<mispredict_count<<endl;
    cout<<"Total Cache Access Count : "<<(cache_hit + cache_miss)<<endl;
    cout<<"Cache Hit Count : "<<cache_hit<<endl;
    cout<<"Cache Miss Count : "<<cache_miss<<endl;
    cout<<"Cache Hit Rate : "<<(((float)cache_hit * 100) / (cache_hit + cache_miss))<<endl;
    cout<<"AMAT : "<<(float)(cache_hit * 10 + cache_miss * 1000)<<" ns"<<endl;
    cout<<"==================================\n"<<endl;

    return 0;
}