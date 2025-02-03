#include <cstdint>
#include <iostream>
using namespace std;

#include "addr.hpp"

uint32_t ExtImm(uint16_t imm_){
    if(imm_ & 0x8000){    
        return (imm_ | 0xffff0000);
    }
    else{
        return (imm_ & 0x0000ffff);
    }
}

uint32_t SignExt(uint16_t imm_){
    return (imm_ | 0xffff0000);
}

uint32_t UnsignExt(uint16_t imm_){
    return (imm_ & 0x0000ffff);
}

uint32_t BranchAddr(uint32_t pc, uint32_t imm_){
    return pc + (imm_ << 2);
}

uint32_t JumpAddr(uint32_t pc, uint32_t addr_){
    return (pc & 0xf0000000) | (addr_<< 2);
}

extern uint32_t MemoryAddr(uint32_t rs_, uint32_t imm_){
    uint32_t memory_addr = rs_ + imm_;
    return memory_addr;
}