#include <iostream>
using namespace std;

#include "stage.hpp"
#include "base.hpp"
#include "latch.hpp"
#include "alu.hpp"
#include "fr.hpp"
#include "addr.hpp"
#include "BranchPredictor.hpp"
#include "VerifyPredict.hpp"
#include "cache.hpp"
#include "alg.hpp"

class IF_ID;
class ID_EX;
class EX_MEM;
class MEM_WB;

class DMC_;

DMC_ DMC[128];

void flush_IF_ID();
void flush_ID_EX();
/* Latch Initial */
IF_ID IFID_Latch[2];
ID_EX IDEX_Latch[2];
EX_MEM EXMEM_Latch[2];
MEM_WB MEMWB_Latch[2];

/* Fetch Stage */

void Fetch(){
    cout<<"=======Fetch Stage======="<<endl;
    instruction = memory[pc / 4];
    IFID_Latch[0].instruction = instruction;
    IFID_Latch[0].next_pc = pc + 4;
    IFID_Latch[0].valid = 1;
    total_inst_count++;

    /* Branch Prediction */
    pc = AlwaysNotTaken(pc);
    
    // pc = AlwaysTaken(pc);
    
    // if(BTFN_Predictor(pc)){
    //     pc = BTB[pc];
    // }   
    // else{
    //     pc = pc + 4;
    // }

    // if(OneBitPredictor(pc)){
    //     pc = BTB[pc];
    // }
    // else{
    //     pc = pc + 4;
    // }

    // if(GlobalHistoryPredictor(pc, GHR)){
    //     pc = BTB[pc];
    // }
    // else{
    //     pc = pc + 4;
    // }

    // if(GsharePredictor(pc, GHR)){
    //     pc = BTB[pc];
    // }
    // else{
    //     pc = pc + 4;
    // }

    // if(LocalBranchPredictor(pc)){
    //     pc = BTB[pc];
    // }
    // else{
    //     pc = pc + 4;
    // }

    return;
}

/* Decode Stage */
void Decode(){
    if(IFID_Latch[1].valid == 0){
        return;
    }
    
    cout<<"=======Decode Stage======="<<endl;
    instruction = IFID_Latch[1].instruction;
    opcode = (instruction >> 26) & 0x3f; // Extract 6bits
    if(opcode == 0){ // R-type
        R_inst++;
        rs = (instruction >> 21) & 0x1f;
        rt = (instruction >> 16) & 0x1f;
        rd = (instruction >> 11) & 0x1f;
        shamt = (instruction >> 6) & 0x1f;
        funct = instruction & 0x3f;
            
        /* Control Signal Generate */
        IDEX_Latch[0].EX.set_EX(opcode, funct);
        IDEX_Latch[0].MEM.set_MEM(opcode, funct);
        IDEX_Latch[0].WB.set_WB(opcode, funct);

        /* Read Register File */    
        IDEX_Latch[0].rs = rs;
        IDEX_Latch[0].rt = rt;
        IDEX_Latch[0].rd = rd;
        IDEX_Latch[0].imm = 0;
        IDEX_Latch[0].shamt = shamt;
        IDEX_Latch[0].funct = funct;
        IDEX_Latch[0].next_pc = IFID_Latch[1].next_pc;
        IDEX_Latch[0].valid = 1;
        IDEX_Latch[0].opcode = opcode;
    }
    else if(opcode != 0x02 && opcode != 0x03 && opcode != 0){ // I-type
        cout<<"I-Type"<<endl;
        I_inst++;
        rs = (instruction >> 21) & 0x1f;
        rt = (instruction >> 16) & 0x1f;
        imm = instruction & 0xffff;
        
        /* Control Signal Generate */
        IDEX_Latch[0].EX.set_EX(opcode, 0);
        IDEX_Latch[0].MEM.set_MEM(opcode, 0);
        IDEX_Latch[0].WB.set_WB(opcode, 0);

        
        /* Read Register File */     
        IDEX_Latch[0].rs = rs;
        IDEX_Latch[0].rt = rt;
        IDEX_Latch[0].rd = 0;
        IDEX_Latch[0].imm = ExtImm(imm);
        IDEX_Latch[0].shamt = 0;
        IDEX_Latch[0].funct = 0;
        IDEX_Latch[0].next_pc = IFID_Latch[1].next_pc;
        IDEX_Latch[0].valid = 1;
        IDEX_Latch[0].opcode = opcode;
    } 
    else if(opcode == 0x02 || opcode == 0x03){ // J-type
        cout<<"J-Type"<<endl;
        J_inst++;
        imm = instruction & 0x3ffffff;
        jump_count++;
        pc = JumpAddr(pc, imm);
        
        /* Control Signal Generate */   

        IDEX_Latch[0].EX.set_EX(opcode, 0);
        IDEX_Latch[0].MEM.set_MEM(opcode, 0);
        IDEX_Latch[0].WB.set_WB(opcode, 0);
        IDEX_Latch[0].rs = 0;
        IDEX_Latch[0].rt = 0;
        IDEX_Latch[0].rd = 0;
        IDEX_Latch[0].imm = imm;
        IDEX_Latch[0].shamt = 0;
        IDEX_Latch[0].funct = 0;
        IDEX_Latch[0].opcode = opcode;
        IDEX_Latch[0].next_pc = IFID_Latch[1].next_pc;
        IDEX_Latch[0].valid = 1;
    }    

    return;
}

/* Execution Stage */

void Execution(){
    if(IDEX_Latch[1].valid == 0){
        return;
    }
    cout<<"=======EXE Stage======="<<endl;
    int data1 = R[IDEX_Latch[1].get_rs()];
    int data2 = R[IDEX_Latch[1].get_rt()];

    /* Checking Data Dependency */

    switch(Forward_Rs(IDEX_Latch[1].get_rs(), EXMEM_Latch[1].get_reg_dst(), MEMWB_Latch[1].get_reg_dst())){
        case 0:
            break;
        case 1: // dist 1
            if(IDEX_Latch[1].get_opcode() == 0x23){ // lw
                data1 = R[IDEX_Latch[1].get_rs()];
                break;
            }
            else if(IDEX_Latch[1].get_opcode() == 0x02 || IDEX_Latch[1].get_opcode() == 0x03){ // j inst
                break;
            }
            else{
                data1 = EXMEM_Latch[1].get_ALU_Result();
                break;
            }
            
        case 2: // dist 2
            if(MEMWB_Latch[1].WB.get_RegWrite() ==1 && MEMWB_Latch[1].WB.get_MemToReg() == 1 && MEMWB_Latch[1].WB.get_JalToReg() == 0){ // lw
                data1 = MEMWB_Latch[1].get_mem_data();
                break;
                }
            else if(MEMWB_Latch[1].WB.get_RegWrite() ==1 && MEMWB_Latch[1].WB.get_MemToReg() == 0 && MEMWB_Latch[1].WB.get_JalToReg() == 0){ // ALU_Result
                data1 = MEMWB_Latch[1].get_ALU_Result();
                break;
            }
            else if(MEMWB_Latch[1].WB.get_RegWrite() == 1 && MEMWB_Latch[1].WB.get_JalToReg() == 1){ // JAL TO Reg
                data1 = MEMWB_Latch[1].get_next_pc() + 4;
                break;
            }
            else if(IDEX_Latch[1].get_opcode() == 0x02 || IDEX_Latch[1].get_opcode() == 0x03){
                cout<<"J inst doesn't need Data Dependency"<<endl;
                break;
            }
        }

    switch(Forward_Rt(IDEX_Latch[1].get_rt(), EXMEM_Latch[1].get_reg_dst(), MEMWB_Latch[1].get_reg_dst())){
        case 0:
            break;
        case 1: // dist 1
            if(IDEX_Latch[1].get_opcode() == 0x23){ // lw
                data2 = R[IDEX_Latch[1].get_rt()];
                break;
            }
            else if(IDEX_Latch[1].get_opcode() == 0x02 || IDEX_Latch[1].get_opcode() == 0x03){
                cout<<"J inst doesn't need Data Dependency"<<endl;
                break;
            }
            else{
                data2 = EXMEM_Latch[1].get_ALU_Result();
                break;
            }

        case 2: // dist 2
            if(MEMWB_Latch[1].WB.get_RegWrite() == 1 && MEMWB_Latch[1].WB.get_MemToReg() == 1 && MEMWB_Latch[1].WB.get_JalToReg() == 0){ // lw
                data2 = MEMWB_Latch[1].get_mem_data();
                break;
            }
            else if(MEMWB_Latch[1].WB.get_RegWrite() == 1 && MEMWB_Latch[1].WB.get_MemToReg() == 0 && MEMWB_Latch[1].WB.get_JalToReg() == 0){ // ALU_Result
                data2 = MEMWB_Latch[1].get_ALU_Result();
                break;
            }
            else if(MEMWB_Latch[1].WB.get_RegWrite() == 1 && MEMWB_Latch[1].WB.get_JalToReg() == 1){ // JAL TO Reg
                data2 = MEMWB_Latch[1].get_next_pc() + 4;
                break;
            }
            else if(IDEX_Latch[1].get_opcode() == 0x02 || IDEX_Latch[1].get_opcode() == 0x03){
                cout<<"J inst doesn't need Data Dependency"<<endl;
                break;
            }
        }

    if((IDEX_Latch[1].EX.get_ALU_Src() == 0)){ // R-Type
        ALU_Result = ALU(data1, data2, IDEX_Latch[1].get_funct(), IDEX_Latch[1].get_shamt(), IDEX_Latch[1].get_opcode());
    }
    else if(IDEX_Latch[1].EX.get_ALU_Src() != 0 && IDEX_Latch[1].get_opcode() != 0x02 && IDEX_Latch[1].get_opcode() != 0x03){ // I-Type
        if(IDEX_Latch[1].get_opcode() == 0x2b || IDEX_Latch[1].get_opcode() == 0x23){
            ALU_Result = ALU(data1, IDEX_Latch[1].get_ext_imm(), 0, 0, IDEX_Latch[1].get_opcode());
            mem_addr = MemoryAddr(data1, IDEX_Latch[1].get_ext_imm());
        }
        else{
            ALU_Result = ALU(data1, IDEX_Latch[1].get_ext_imm(), 0, 0, IDEX_Latch[1].get_opcode());
        }
    }

    if(IDEX_Latch[1].EX.get_RegDst() == 0){
        EXMEM_Latch[0].reg_dst = IDEX_Latch[1].get_rt();
    }
    else if(IDEX_Latch[1].EX.get_RegDst() == 1){
        EXMEM_Latch[0].reg_dst = IDEX_Latch[1].get_rd();
    }
    else if(IDEX_Latch[1].EX.get_RegDst() == 2){
        EXMEM_Latch[0].reg_dst = 31;
    }

    EXMEM_Latch[0].write_data = data2;
    EXMEM_Latch[0].next_pc = IDEX_Latch[1].get_next_pc();
    EXMEM_Latch[0].MEM = IDEX_Latch[1].MEM;
    EXMEM_Latch[0].WB = IDEX_Latch[1].WB;
    EXMEM_Latch[0].ALU_Result = ALU_Result;
    EXMEM_Latch[0].addr = mem_addr;

    if(IDEX_Latch[1].EX.get_jr()){ // inst is jr, R-Type
        pc = data1;
        IFID_Latch[1].valid = 0;
        jump_count++;
    }

    int branch_taken = IDEX_Latch[1].EX.get_branch() && bcond;
    uint32_t branch_addr = BranchAddr(IDEX_Latch[1].get_next_pc(), IDEX_Latch[1].get_ext_imm());

    /* Verify Branch Prediction */
    if(IDEX_Latch[1].EX.get_ALU_Op() == 0x04 || IDEX_Latch[1].EX.get_ALU_Op() == 0x05){
        VerifyAlwaysNotTaken(branch_addr, branch_taken);
        // VerifyAlwaysTaken(pc, branch_addr, branch_taken);
        // VerifyBTFN(branch_addr, IDEX_Latch[1].get_next_pc(), branch_taken);
        // VerifyOneBitPredictor(branch_addr, IDEX_Latch[1].get_next_pc(), branch_taken);
        // VerifyTwoBitPredictor(branch_addr, IDEX_Latch[1].get_next_pc(), branch_taken);
        // VerifyGHPredictor(branch_addr, IDEX_Latch[1].get_next_pc(), branch_taken);
        // VerifyGSharePredictor(branch_addr, IDEX_Latch[1].get_next_pc(), branch_taken);
        // VerifyLBPredictor(branch_addr, IDEX_Latch[1].get_next_pc(), branch_taken);
    }

    EXMEM_Latch[0].valid = 1;
    return;
}

/* Memory Access Stage */
void MEM(){ 
    if(EXMEM_Latch[1].valid == 0){
        return;
    }
    cout<<"=========MEM Stage========="<<endl;

    /* Decompose Direct Mapped Cache */
    tag = (EXMEM_Latch[1].get_addr() >> 13) & 0x0007ffff;
    idx = (EXMEM_Latch[1].get_addr() >> 6) & 0x0000007f;
    offset = EXMEM_Latch[1].get_addr() & 0x0000003f;

    if(EXMEM_Latch[1].MEM.get_MemRead()){ // lw
        int res = 0;
        res = DMC[idx].DMC_Read(EXMEM_Latch[1].get_addr());

        if(res == MISS){ // miss 발생
            int idx_ = SCA(DMC);
            DMC[idx_].set_tag(tag);
            DMC[idx_].set_valid(1);
            DMC[idx_].set_dirty(0);
            DMC[idx_].set_data(EXMEM_Latch[1].get_addr());
            DMC[idx_].set_sca(0);
            
            MEMWB_Latch[0].mem_data = memory[EXMEM_Latch[1].get_addr() / 4];
            memory_access_count++;
        }
        else{    
            MEMWB_Latch[0].mem_data = res;
        }
        
    }

    else if(EXMEM_Latch[1].MEM.get_MemWrite()){ // sw
        if(DMC[idx].DMC_Write_Through(EXMEM_Latch[1].get_addr(), EXMEM_Latch[1].get_write_data()) == MISS){
            int idx_ = SCA(DMC);
            DMC[idx_].set_tag(tag);
            DMC[idx_].set_valid(1);
            DMC[idx_].set_dirty(0);
            DMC[idx_].set_data(EXMEM_Latch[1].get_addr());
            DMC[idx_].set_sca(0);
            DMC[idx_].DMC_Write_Through(EXMEM_Latch[1].get_addr(), EXMEM_Latch[1].get_write_data());
                
            memory_access_count++;
        }
        else{
            cout<<"CACHE SW OK"<<endl;
        }

        /* Write Back Policy */
        // if(DMC[idx].DMC_Write_Back(EXMEM_Latch[1].get_addr(), EXMEM_Latch[1].get_write_data()) == MISS){
        //     int idx_ = SCA(DMC);
        //     DMC[idx_].set_tag(tag);
        //     DMC[idx_].set_valid(1);
        //     DMC[idx_].set_dirty(0);
        //     DMC[idx_].set_data(EXMEM_Latch[1].get_addr());
        //     DMC[idx_].set_sca(0);
        //     memory[EXMEM_Latch[1].get_addr() / 4] = EXMEM_Latch[1].get_write_data();
        //     memory_access_count++;
        // }
        // else{
        //     cout<<"CACHE SW OK"<<endl;
        //     memory[EXMEM_Latch[1].get_addr() / 4] = EXMEM_Latch[1].get_write_data();
        // }
    }


    MEMWB_Latch[0].ALU_Result = EXMEM_Latch[1].get_ALU_Result();
    MEMWB_Latch[0].reg_dst = EXMEM_Latch[1].get_reg_dst();
    MEMWB_Latch[0].next_pc = EXMEM_Latch[1].get_next_pc();
    MEMWB_Latch[0].valid = 1;
    MEMWB_Latch[0].WB = EXMEM_Latch[1].WB;
    return;
}

/* Write Back Stage */
void WB(){
    if(MEMWB_Latch[1].valid == 0){
        return;
    }
    cout<<"=========WB Stage========="<<endl;
    if(MEMWB_Latch[1].WB.get_RegWrite()){
        if(MEMWB_Latch[1].WB.get_MemToReg() == 0x01){
            // Mem Data to Reg
            R[MEMWB_Latch[1].get_reg_dst()] = MEMWB_Latch[1].get_mem_data();
        }
        else if(MEMWB_Latch[1].WB.get_MemToReg() == 0x00){
            if(MEMWB_Latch[1].WB.get_JalToReg() == 1){
                // JAL TO REG
                R[MEMWB_Latch[1].get_reg_dst()] = MEMWB_Latch[1].get_next_pc()+4;
            }
            // ALU Result to Reg
            else{
                R[MEMWB_Latch[1].get_reg_dst()] = MEMWB_Latch[1].get_ALU_Result();
            }
        }
    }
    return;
}

/* Copy Latch (input -> output)*/
void copy_latch(){
    if(IFID_Latch[0].get_valid() == 1){
        IFID_Latch[1] = IFID_Latch[0];
    }
    if(IDEX_Latch[0].get_valid() == 1){
        IDEX_Latch[1] = IDEX_Latch[0];
    }
    if(EXMEM_Latch[0].get_valid() == 1){
        EXMEM_Latch[1] = EXMEM_Latch[0];
    }
    if(MEMWB_Latch[0].get_valid() == 1){
        MEMWB_Latch[1] = MEMWB_Latch[0];
    }
}

void flush_IF_ID(){
    IFID_Latch[0].valid = 0;
    IFID_Latch[1].valid = 0;
}

void flush_ID_EX(){
    IDEX_Latch[0].valid = 0;
    IDEX_Latch[1].valid = 0;
}