#include "I_Type_Class.h"
#include "R_Type_Class.h"
#include "J_Type_Class.h"
#include <iostream>
using namespace std;

/*
** File Name : I_Type_class.cpp
** Contents : Implement I_Type Class & Related Function
** Author : Min-Hyuk-Cho
** Date : 24/05/01
*/

I_Type::I_Type(int instruction):MIPS_(instruction){
        i_instruction = MIPS_::get_inst();
        i_opcode = MIPS_::opcode_tok();
    }

void I_Type::tok_inst(){
    cout<<"=======Decode State======="<<endl;
    rs = (i_instruction >> 21) & 0x1f;
    rt = (i_instruction >> 16) & 0x1f;
    imm = (i_instruction & 0xffff);
    s_imm = (imm & 0x8000) ? (imm | 0xffff0000) : (imm); // Extend 32bits

    
    // BranchAddr = s_imm << 2; // BranchAddr just is composed by shifting left 2bits the s_imm
    BranchAddr = (s_imm << 2);
    cout<<"Opcode : 0x"<<hex<<i_opcode<<", rs : "<<rs<<", rt : "<<rt<<", imm : "<<imm<<endl;
}

void I_Type::select_operation(){
    cout<<"=======Execute State======="<<endl;
    switch(i_opcode){ // I-Type is classified using opcode
        case 0x08:
            addi(rs, rt, s_imm); // addi rs rt imm
            break;
        case 0x09:
            addiu(rs, rt, s_imm); // addiu rs rt imm
            break;
        case 0x0c:
            andi(rs, rt, imm); // andi rs rt imm
            break;
        case 0x04:
            beq(rs, rt, BranchAddr); // beq rs rt imm
            break;
        case 0x05:
            bne(rs, rt, BranchAddr); // bne rs rt imm
            break;
        case 0x0f:
            lui(rt, imm); // lui rt imm
            break;
        case 0x23:
            lw(rs, rt, s_imm); // lw rt imm(rs)
            break;
        case 0x0d:
            ori(rs, rt, imm); // ori rt rs imm
            break;
        case 0x0a:
            slti(rs, rt, s_imm); // slti rt rs imm
            break;
        case 0x0b:
            sltiu(rs, rt, s_imm); // sltiu rt rs imm
            break;
        case 0x2b:
            sw(rs, s_imm, rt); // sw rt imm(rs)
            break;
        default:
            cout<<"There is no matching Instruction"<<endl;
            break;
    }
}

void I_Type::addi(uint32_t _rs, uint32_t _rt, int32_t _imm){
    cout<<"=======ADDI Operation======="<<endl;
    R[_rt] = R[_rs] + _imm; 
    pc = pc + 1;
    cout<<"=======Write Back======="<<endl;
    cout<<"R["<<_rt<<"] is "<<R[_rt]<<endl;
    cout<<"PC: "<<pc<<endl;
}

void I_Type::addiu(uint32_t _rs, uint32_t _rt, int32_t _imm){
    R[_rs] = static_cast<uint32_t>(R[_rs]);
    _imm = static_cast<uint32_t>(_imm);
    if(R[_rs] == 0){
        cout<<"=======Load Imm Operation======="<<endl;
        R[_rt] = R[_rs] + _imm;
    }
    else{
        cout<<"=======ADDIU Operation======="<<endl;
        R[_rt] = R[_rs] + _imm; 
    }
    R[_rt] = static_cast<uint32_t>(R[_rt]);
    pc = pc + 1;
    cout<<"=======Write Back======="<<endl;
    cout<<"R["<<_rt<<"] is "<<R[_rt]<<endl;
    cout<<"PC: "<<pc<<endl;
}

void I_Type::andi(uint32_t _rs, uint32_t _rt, int32_t _imm){
    cout<<"=======ANDI Operation======="<<endl;
    R[_rt] = R[_rs] & _imm; 
    pc = pc + 1;
    cout<<"=======Write Back======="<<endl;
    cout<<"R["<<_rt<<"] is "<<R[_rt]<<endl;
    cout<<"PC: "<<pc<<endl;
}

void I_Type::beq(uint32_t _rs, uint32_t _rt, uint32_t _branchaddr){
    cout<<"=======BEQ Operation======="<<endl;
    if(R[_rs] == R[_rt]){
        pc = ((pc*4 + 4) + _branchaddr) / 4; 
        branch_count++;

        jump_inst = Memory[pc];
        cout<<"Target Jump PC : "<<pc<<endl;
        MIPS_ * jump_mips = new MIPS_(jump_inst);
        jump_opcode = jump_mips->opcode_tok();

        // R-type
        if(jump_opcode == 0){
            cout<<"R-Type is Created"<<endl;
            r_count++;
            MIPS_ * r_type = new R_Type(Memory[pc]);
            r_type->tok_inst();
            r_type->select_operation();
        }
        // J-type
        else if(jump_opcode == 2 || jump_opcode == 3){
            cout<<"J-Type is Created"<<endl;
            j_count++;
            MIPS_ * j_type_inst = new J_Type(Memory[pc]);
            j_type_inst->tok_inst();
            j_type_inst->select_operation();
        }
        // I-type
        else{
            cout<<"I-Type is Created"<<endl;
            i_count++;
            MIPS_ * i_type_inst = new I_Type(Memory[pc]);
            i_type_inst->tok_inst();
            i_type_inst->select_operation();
        }
    }
    else{    
        pc = pc + 1;
    }
    cout<<"=======Write Back======="<<endl;
    cout<<"PC: "<<pc<<endl;
}

void I_Type::bne(uint32_t _rs, uint32_t _rt, int32_t _branchaddr){
    if(R[_rt] == 0){
        cout<<"=======BNEZ Operation======="<<endl;
        if(R[_rs] != 0){
            pc = ((pc*4 + 4) + _branchaddr) / 4;
            branch_count++;

            jump_inst = Memory[pc]; 
            cout<<"Target Jump PC : "<<pc<<endl;
            MIPS_ * jump_mips = new MIPS_(jump_inst);
            jump_opcode = jump_mips->opcode_tok();
            if(jump_opcode == 0){
                cout<<"R-Type is Created"<<endl;
                r_count++;
                MIPS_ * r_type = new R_Type(Memory[pc]);
                r_type->tok_inst();
                r_type->select_operation();
            }
            // J-type
            else if(jump_opcode == 2 || jump_opcode == 3){
                cout<<"J-Type is Created"<<endl;
                j_count++;
                MIPS_ * j_type_inst = new J_Type(Memory[pc]);
                j_type_inst->tok_inst();
                j_type_inst->select_operation();
            }
            // I-type
            else{
                cout<<"I-Type is Created"<<endl;
                i_count++;
                MIPS_ * i_type_inst = new I_Type(Memory[pc]);
                i_type_inst->tok_inst();
                i_type_inst->select_operation();
            }
        }

        else{
            pc = pc + 1;
        }
        cout<<"=======Write Back======="<<endl;
        cout<<"PC: "<<pc<<endl;
    }

    else{
        cout<<"=======BNE Operation======="<<endl;
        if(R[_rs] != R[_rt]){
            pc = ((pc*4 + 4) + _branchaddr) / 4;
            branch_count++;

            jump_inst = Memory[pc];
            MIPS_ * jump_mips = new MIPS_(jump_inst);
            jump_opcode = jump_mips->opcode_tok();

            //R-Type
            if(jump_opcode == 0){
                cout<<"R-Type is Created"<<endl;
                r_count++;
                MIPS_ * r_type = new R_Type(Memory[pc]);
                r_type->tok_inst();
                r_type->select_operation();
                }
            // J-type
            else if(jump_opcode == 2 || jump_opcode == 3){
                cout<<"J-Type is Created"<<endl;
                j_count++;
                MIPS_ * j_type_inst = new J_Type(Memory[pc]);
                j_type_inst->tok_inst();
                j_type_inst->select_operation();
                }
            // I-type
            else{
                cout<<"I-Type is Created"<<endl;
                i_count++;
                MIPS_ * i_type_inst = new I_Type(Memory[pc]);
                i_type_inst->tok_inst();
                i_type_inst->select_operation();
                }
        }

        else{
            pc = pc + 1;
        }
        cout<<"=======Write Back======="<<endl;
        cout<<"PC: "<<pc<<endl;
    }
}

void I_Type::lui(uint32_t _rt, int32_t _imm){
    cout<<"=======LUI Operation======="<<endl;
    R[_rt] = (_imm << 16);
    pc = pc + 1;
    cout<<"=======Write Back======="<<endl;
    cout<<"R["<<_rt<<"] is "<<R[_rt]<<endl;
    cout<<"PC: "<<pc<<endl;
}

void I_Type::lw(uint32_t _rs, uint32_t _rt, int32_t _imm){
    cout<<"=======LW Operation======="<<endl;
    cout<<"=======Memory Access======="<<endl;
    R[_rt] = Memory[static_cast<uint32_t>(R[_rs] + _imm)];
    cout<<"Memory : "<<Memory[static_cast<uint32_t>(R[_rs] + _imm)]<<endl;
    
    cout<<"R["<<_rt<<"] = Memory["<<"R["<<_rs<<"] + "<<_imm<<"]"<<endl;
    pc = pc + 1;
    memory_access_count++;
    cout<<"=======Write Back======="<<endl;
    cout<<"R["<<_rt<<"] is "<<R[_rt]<<endl;
    cout<<"PC: "<<pc<<endl;
}

void I_Type::ori(uint32_t _rs, uint32_t _rt, int32_t _imm){
    cout<<"=======ORI Operation======="<<endl;
    R[_rt] = R[_rs] | _imm;
    pc = pc + 1;
    cout<<"=======Write Back======="<<endl;
    cout<<"R["<<_rt<<"] is "<<R[_rt]<<endl;
    cout<<"PC: "<<pc<<endl;
}

void I_Type::slti(uint32_t _rs, uint32_t _rt, int32_t _imm){
    cout<<"=======SLTI Operation======="<<endl;
    R[_rt] = (R[_rs] < _imm) ? 1 : 0;
    pc = pc + 1;
    cout<<"=======Write Back======="<<endl;
    cout<<"R["<<_rt<<"] is "<<R[_rt]<<endl;
    cout<<"PC: "<<pc<<endl;
}

void I_Type::sltiu(uint32_t _rs, uint32_t _rt, int32_t _imm){
    cout<<"=======SLTIU Operation======="<<endl;
    R[_rs] = static_cast<uint32_t>(R[_rs]);
    R[_rt] = (R[_rs] < _imm) ? 1 : 0;
    pc = pc + 1;
    cout<<"=======Write Back======="<<endl;
    cout<<"R["<<_rt<<"] is "<<unsigned(R[_rt])<<endl;
    cout<<"PC: "<<pc<<endl;
}

void I_Type::sw(uint32_t _rs, int32_t _imm, uint32_t _rt){
    cout<<"=======SW Operation======="<<endl;
    cout<<"=======Memory Access======="<<endl;
    Memory[static_cast<uint32_t>(R[_rs] + _imm)] = R[_rt];
    cout<<"Memory : "<<Memory[static_cast<uint32_t>(R[_rs] + _imm)]<<endl;
    cout<<"Memory["<<R[_rs] + _imm<<"] = "<<"R["<<_rt<<"]"<<endl;
    pc = pc + 1;
    memory_access_count++;
    
    cout<<"=======Write Back======="<<endl;
    cout<<"R["<<_rt<<"] is "<<R[_rt]<<endl;
    cout<<"PC: "<<pc<<endl;
}