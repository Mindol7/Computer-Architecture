#include <iostream>
#include <cstdint>
#include <cmath>
using namespace std;
#include "alu.hpp"

/* Implement ALU */

int ALU(int data1_, int data2_, int funct_, int shamt_, int opcode_){ // operation_ from ALU_Control
    uint32_t result = 0;
    if(opcode_ == 0){ // R-Type
        switch(funct_){
            case 0x20: // addi
                cout<<"ADD Inst"<<endl;
                result = data1_ + data2_;
                break;
            case 0x21: // addu
                if(data2_ == 0){
                    cout<<"MOV Inst"<<endl;
                    result = static_cast<uint32_t>(data1_) + static_cast<uint32_t>(data2_);
                    break;
                }
                cout<<"ADDU Inst"<<endl;
                result = static_cast<uint32_t>(data1_) + static_cast<uint32_t>(data2_); 
                break;
            case 0x24: // and
                cout<<"AND Inst"<<endl;
                result = data1_ & data2_;
                break;
            case 0x08: // jr
                cout<<"JR Inst"<<endl;
                result = 0;
                break;
            case 0x27: // nor
                cout<<"NOR Inst"<<endl;
                result = ~(data1_ | data2_);
                break;
            case 0x25: // or
                cout<<"OR Inst"<<endl;
                result = data1_ | data2_;
                break;
            case 0x2b: // sltu
                cout<<"SLTU Inst"<<endl;
                result = (static_cast<uint32_t>(data1_) < static_cast<uint32_t>(data2_)) ? 1 : 0; 
                break;
            case 0x00: // sll
                if(shamt_ == 0){
                    cout<<"NOP INST"<<endl;
                    break;
                }
                cout<<"SLL Inst"<<endl;
                result = data2_ << shamt_;
                break;
            case 0x02: // srl
                cout<<"SRL Inst"<<endl;
                result = data2_ >> shamt_;
            case 0x22: // sub
                cout<<"SUB Inst"<<endl;
                result = data1_ - data2_;
            case 0x23: // subu
                cout<<"SUBU Inst"<<endl;
                result = static_cast<int>(abs(static_cast<long long>(data1_) - static_cast<long long>(data2_)));
                break;
        }
    }
    else if(opcode_ != 0){ // I-Type or J-Type
        switch(opcode_){
            case 0x08: // addi
                cout<<"ADDI Inst"<<endl;
                result = data1_ + data2_; 
                break;
            case 0x09: // addiu
                if(data1_ == 0){ // li
                        cout<<"Load Imm Inst"<<endl; 
                        result = static_cast<uint32_t>(data1_) + static_cast<uint32_t>(data2_);
                        result = result & 0x0000ffff; // test code
                        cout<<"Result : "<<result<<endl;
                        break;
                    }
                cout<<"ADDIU Inst"<<endl;
                result = static_cast<uint32_t>(data1_) + static_cast<uint32_t>(data2_);
                break;
            case 0x0c: // andi
                cout<<"ANDI Inst"<<endl;
                result = data1_ & data2_;
            case 0x04: // beq
                cout<<"BEQ Inst"<<endl;
                if(data1_ == data2_){
                    bcond = 1;
                    branch_count++;
                }
                else{
                    bcond = 0;
                }
                break;
            case 0x05: // bne
                cout<<"BNE Inst"<<endl;
                if(data1_ != data2_){
                    bcond = 1;
                    branch_count++;
                }
                else{
                    bcond = 0;
                }
                break;
            case 0x02: // j 
                cout<<"J Inst"<<endl;
                J_inst++;
                break;
            case 0x03: // jal
                cout<<"JAL Inst"<<endl;
                J_inst++;
                break;
            case 0x0f: // lui
                cout<<"LUI Inst"<<endl;
                result = (data2_ << 16) & 0xffff0000;
                break;
            case 0x23: // lw
                cout<<"LW Inst"<<endl;
                break;
            case 0x0d: // ori
                cout<<"ORI Inst"<<endl;
                result = data1_ | data2_;
                break;
            case 0x0a: // slti
                cout<<"SLTI Inst"<<endl;
                result = (data1_ < data2_) ? 1 : 0;
                break;
            case 0x0b: // sltiu
                cout<<"SLTIU Inst"<<endl;
                result = (static_cast<uint32_t>(data1_) < static_cast<uint32_t>(data2_)) ? 1 : 0;
                break;
            case 0x2b: // sw
                cout<<"SW Inst"<<endl;
                break;
        }
    }
    else{
        ERROR;
        exit(-1);
    }

    return result;
}