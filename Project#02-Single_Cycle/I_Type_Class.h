#ifndef I_TYPE_CLASS_H
#define I_TYPE_CLASS_H
#include "mips_class.h"
#include <cstdint>
/*
** File Name : I_Type_Class.h
** Contents : Declare I_Type Class & Related Function
** Author : Min-Hyuk-Cho
** Date : 24/05/01
*/

class I_Type:public MIPS_{
    private:
        int i_instruction;
        uint32_t i_opcode, rs, rt;
        int32_t imm, s_imm;
        uint32_t BranchAddr;
    public:
        I_Type(int instruction);
        
        void tok_inst() override; // J-Type instruction을 tok하는 함수
        void select_operation() override;
        void addi(uint32_t _rs, uint32_t _rt, int32_t _imm);
        void addiu(uint32_t _rs, uint32_t _rt, int32_t _imm);
        void andi(uint32_t _rs, uint32_t _rt, int32_t _imm);
        void beq(uint32_t _rs, uint32_t _rt, uint32_t _branchaddr);
        void bne(uint32_t _rs, uint32_t _rt, int32_t _branchaddr);
        void lui(uint32_t _rt, int32_t _imm);
        void lw(uint32_t _rs, uint32_t _rt, int32_t _imm);
        void ori(uint32_t _rs, uint32_t _rt, int32_t _imm);
        void slti(uint32_t _rs, uint32_t _rt, int32_t _imm);
        void sltiu(uint32_t _rs, uint32_t _rt, int32_t _imm);
        void sw(uint32_t _rs, int32_t _imm, uint32_t _rt);
};


#endif