#ifndef _ADDR_HPP__
#define _ADDR_HPP__

#include <cstdint>

#include "base.hpp"

extern uint32_t ExtImm(uint16_t imm_);
extern uint32_t SignExt(uint16_t imm_);
extern uint32_t UnsignExt(uint16_t imm_);
extern uint32_t BranchAddr(uint32_t pc, uint32_t imm_);
extern uint32_t JumpAddr(uint32_t pc, uint32_t addr_);
extern uint32_t MemoryAddr(uint32_t rs_, uint32_t imm_);
#endif 