#include "R_Type_Class.h"
#include "J_Type_Class.h"
#include "I_Type_Class.h"
#include <iostream>
#include <cstdint>
using namespace std;

/*
** File Name : R_Type_class.cpp
** Contents : Implement R_Type Class & Related Function
** Author : Min-Hyuk-Cho
** Date : 24/05/01
*/

R_Type::R_Type(int instruction):MIPS_(instruction){
        r_instruction = MIPS_::get_inst();
        r_opcode = MIPS_::opcode_tok();
}

void R_Type::tok_inst(){
    cout<<"=======Decode State======="<<endl;
    rs = (r_instruction >> 21) & 0x1f; // Extract 5-bits
    rt = (r_instruction >> 16) & 0x1f; // Extract 5-bits
    rd = (r_instruction >> 11) & 0x1f; // Extract 5-bits
    shamt = (r_instruction >> 6) & 0x1f; // Extract 5-bits
    funct = (r_instruction) & 0x3f;  // Extract 6-bits

    cout<<"Opcode : 0x"<<hex<<r_opcode<<", rs : "<<rs<<", rt : "<<rt<<", rd : "<<rd<<", shamt : "<<shamt<<", funct : "<<funct<<endl;
}

// Execution State
void R_Type::select_operation(){
    cout<<"=======Execution State======="<<endl;
    switch (funct){ // R-type is classified using funct
        case 0x20: // add rs, rt, rd
            add(rs, rt, rd);
            break;
        case 0x21: // addu rs, rt, rd; if rt == 0, then move rd, rs   
            addu(rs, rt, rd);
            break;
        case 0x24:
            _and(rs, rt, rd); // AND rd rs rt
            break;
        case 0x08:
            jump_register(rs); // J rs
            break;
        case 0x27:
            _nor(rs, rt, rd); // NOR rd, rs, rt 
            break;
        case 0x25:
            _or(rs, rt, rd); // OR rd, rs, rt
            break;
        case 0x2a:
            slt(rs, rt, rd); // SLT rd, rs, rt
            break;
        case 0x2b:
            sltu(rs, rt, rd); // SLTU rd, rs, rt
            break;
        case 0x00:
            sll(rt, rd, shamt); // SLL rd, rt, shamt
            break;
        case 0x02:
            srl(rt, rd, shamt); // SRL rd, rt, shamt
            break;
        case 0x22:
            sub(rs, rt, rd); // SUB rd, rs, rt
            break;
        case 0x23:
            subu(rs, rt, rd); // SUBU rd, rs, rt
            break;
        default:
            cout<<"There is no matching operation"<<endl;
            break;
            
    }
}

void R_Type::add(uint32_t _rs, uint32_t _rt, uint32_t _rd){
    cout<<"=======ADD Operation======="<<endl;
    R[_rd] = R[_rs] + R[_rd]; 
    cout<<"R["<<_rd<<"] = "<<"R["<<_rs<<"] + R["<<_rt<<"]"<<endl;
    pc = pc + 1;
    cout<<"=======Write Back======="<<endl;
    cout<<"R["<<_rd<<"] is "<<R[_rd]<<endl;
    cout<<"PC: "<<pc<<endl;
}

void R_Type::addu(uint32_t _rs, uint32_t _rt, uint32_t _rd){
    if(R[_rt] == 0){
        cout<<"=======MOVE Operation======="<<endl;
        R[_rd] = R[_rs];
        cout<<"R["<<_rd<<"] = "<<"R["<<_rs<<"]"<<endl;
        cout<<"R["<<_rd<<"] is "<<R[_rd]<<endl;  
    }
    else{    
        cout<<"=======ADDU Operation======="<<endl;
        cout<<"R[3]"<<R[_rs]<<", R[2]"<<R[_rt]<<endl;
        R[_rs] = static_cast<uint32_t>(R[_rs]);
        R[_rt] = static_cast<uint32_t>(R[_rt]);
        R[_rd] = R[_rs] + R[_rt];
        R[_rd] = static_cast<uint32_t>(R[_rd]);
        cout<<"R["<<_rd<<"] = "<<"R["<<_rs<<"] + R["<<_rt<<"]"<<endl;
    }
    pc = pc + 1;
    cout<<"=======Write Back======="<<endl;
    cout<<"R["<<_rd<<"] is "<<R[_rd]<<endl;
    cout<<"PC: "<<pc<<endl;
}

void R_Type::_and(uint32_t _rs, uint32_t _rt, uint32_t _rd){
    cout<<"=======AND Operation======="<<endl;
    R[_rd] = R[_rs] & R[_rd]; 
    cout<<"R["<<_rd<<"] = "<<"R["<<_rs<<"] & R["<<_rt<<"]"<<endl;
    pc = pc + 1;
    cout<<"=======Write Back======="<<endl;
    cout<<"R["<<_rd<<"] is "<<R[_rd]<<endl;
    cout<<"PC: "<<pc<<endl;
}

void R_Type::jump_register(uint32_t _rs){
    cout<<"=======JUMP Register Operation======="<<endl;
    pc = R[_rs];
    jump_inst = Memory[pc];
    while(1){
        if(R[31] == 0xffffffff){
            cout<<"Program Finish!!!"<<endl;
            return;
        }
        else{
            jump_inst = Memory[pc];
            MIPS_ * jump_mips = new MIPS_(jump_inst);
            jump_opcode = jump_mips ->opcode_tok();

            // R-type
            if(jump_opcode == 0){
                    cout<<"R-Type is Created"<<endl;
                    r_count++;
                    MIPS_ * r_type = new R_Type(Memory[pc]);
                    r_type->tok_inst();
                    r_type->select_operation();
                    delete r_type;
                }
            // J-type
            else if(jump_opcode == 2 || jump_opcode == 3){
                cout<<"J-Type is Created"<<endl;
                j_count++;
                MIPS_ * j_type = new J_Type(Memory[pc]);
                j_type->tok_inst();
                j_type->select_operation();
                delete j_type;
            }
            // I-type
            else{
                cout<<"I-Type is Created"<<endl;
                i_count++;
                MIPS_ * i_type = new I_Type(Memory[pc]);
                i_type->tok_inst();
                i_type->select_operation();
                delete i_type;
            }    
            delete jump_mips;
        }
    }
}

void R_Type::_nor(uint32_t _rs, uint32_t _rt, uint32_t _rd){
    cout<<"=======NOR Operation======="<<endl;
    R[_rd] = ~(R[_rs] | R[_rt]);
    cout<<"R["<<_rd<<"] = "<<"R["<<_rs<<"] nor R["<<_rt<<"]"<<endl;
    
    cout<<"=======Write Back======="<<endl;
    pc = pc + 1;
    cout<<"R["<<_rd<<"] is "<<R[_rd]<<endl;
    cout<<"PC: "<<pc<<endl;
}

void R_Type::_or(uint32_t _rs, uint32_t _rt, uint32_t _rd){
    cout<<"=======OR Operation======="<<endl;
    R[_rd] = R[rs] | R[rt];
    pc = pc + 1;
    cout<<"R["<<_rd<<"] = "<<"R["<<_rs<<"] | R["<<_rt<<"]"<<endl;
    cout<<"=======Write Back======="<<endl;
    cout<<"R["<<_rd<<"] is "<<R[_rd]<<endl;
    cout<<"PC: "<<pc<<endl;
}

void R_Type::slt(uint32_t _rs, uint32_t _rt, uint32_t _rd){
    cout<<"=======SLT Operation======="<<endl;
    R[_rd] = (R[_rs] < R[_rt]) ? 1 : 0;
    pc = pc + 1;
    cout<<"R["<<_rd<<"] (R["<<_rs<<"] < R["<<_rt<<"]) ? 1 : 0"<<endl;
    cout<<"=======Write Back======="<<endl;
    cout<<"R["<<_rd<<"] is "<<R[_rd]<<endl;
    cout<<"PC: "<<pc<<endl;
}

void R_Type::sltu(uint32_t _rs, uint32_t _rt, uint32_t _rd){
    cout<<"=======SLTU Operation======="<<endl;
    R[_rs] = static_cast<uint32_t>(R[_rs]);
    R[_rt] = static_cast<uint32_t>(R[_rt]);
    R[_rd] = (R[_rs] < R[_rt]) ? 1 : 0;
    pc = pc + 1;
    cout<<"R["<<_rd<<"] (R["<<_rs<<"] < R["<<_rt<<"]) ? 1 : 0"<<endl;
    cout<<"=======Write Back======="<<endl;
    cout<<"R["<<_rd<<"] is "<<R[_rd]<<endl;
    cout<<"PC: "<<pc<<endl;
}

void R_Type::sll(uint32_t _rt, uint32_t _rd, uint32_t _shamt){
    shamt = static_cast<uint32_t>(_shamt);
    if(shamt == 0 && _rt == 0 && _rd == 0){
        cout<<"=======NOP Operation======="<<endl;
        pc = pc + 1;
        cout<<"PC: "<<pc<<endl;
    }
    else{
        cout<<"=======SLL Operation======="<<endl;
        R[_rd] = R[_rt] << shamt;
        pc = pc + 1;
        cout<<"R["<<_rd<<"] = "<<"R["<<_rt<<"] <<"<<_shamt<<endl;
        cout<<"=======Write Back======="<<endl;
        cout<<"R["<<_rd<<"] is "<<R[_rd]<<endl;
        cout<<"PC: "<<pc<<endl;
    }
}

void R_Type::srl(uint32_t _rt, uint32_t _rd, uint32_t _shamt){
    cout<<"=======SRL Operation======="<<endl;
    shamt = static_cast<uint32_t>(_shamt);
    R[_rd] = R[_rt] >> shamt;
    pc = pc + 1;
    cout<<"R["<<_rd<<"] = "<<"R["<<_rt<<"] >>"<<_shamt<<endl;
    cout<<"=======Write Back======="<<endl;
    cout<<"R["<<_rd<<"] is "<<R[_rd]<<endl;
    cout<<"PC: "<<pc<<endl;
}

void R_Type::sub(uint32_t _rs, uint32_t _rt, uint32_t _rd){
    cout<<"=======SUB Operation======="<<endl;
    R[_rd] = R[_rs] - R[_rt];
    pc = pc + 1;
    cout<<"R["<<_rd<<"] = "<<"R["<<_rs<<"] - R["<<_rt<<"]"<<endl;
    cout<<"=======Write Back======="<<endl;
    cout<<"R["<<_rd<<"] is "<<R[_rd]<<endl;
    cout<<"PC: "<<pc<<endl;
}

void R_Type::subu(uint32_t _rs, uint32_t _rt, uint32_t _rd){
    cout<<"=======SUBU Operation======="<<endl;
    R[_rs] = static_cast<uint32_t>(R[_rs]);
    R[_rt] = static_cast<uint32_t>(R[_rt]);
    R[_rd] = R[_rs] - R[_rt];
    R[_rd] = static_cast<uint32_t>(R[_rd]);
    cout<<"R["<<_rd<<"] = "<<"R["<<_rs<<"] - R["<<_rt<<"]"<<endl; 
    pc = pc + 1;
    cout<<"=======Write Back======="<<endl;
    cout<<"R["<<_rd<<"] is "<<unsigned(R[_rd])<<endl;
    cout<<"PC: "<<pc<<endl;
}