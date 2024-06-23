#ifndef R_TYPE_CLASS_H
#define R_TYPE_CLASS_H
#include "mips_class.h"
#include <cstdint>
/*
** File Name : R_Type_class.h
** Contents : Declare R_Type Class & Related Function
** Author : Min-Hyuk-Cho
** Date : 24/05/01
*/

class R_Type:public MIPS_{
    private:
        int r_instruction;
        uint32_t r_opcode, rs, rt, rd, shamt, funct;
    public:
        R_Type(int instruction);
        void tok_inst() override;
        void select_operation() override;
        void add(uint32_t _rs, uint32_t _rt, uint32_t _rd);
        void addu(uint32_t _rs, uint32_t _rt, uint32_t _rd);
        void _and(uint32_t _rs, uint32_t _rt, uint32_t _rd);
        void jump_register(uint32_t _rs);
        void _nor(uint32_t _rs, uint32_t _rt, uint32_t _rd);
        void _or(uint32_t _rs, uint32_t _rt, uint32_t _rd);
        void slt(uint32_t _rs, uint32_t _rt ,uint32_t _rd);
        void sltu(uint32_t _rs, uint32_t _rt, uint32_t _rd);
        void sll(uint32_t _rt, uint32_t _rd, uint32_t _shamt);
        void srl(uint32_t _rt, uint32_t _rd, uint32_t _shamt);
        void move(uint32_t _rs, uint32_t _rd);
        void sub(uint32_t _rs, uint32_t _rt, uint32_t _rd);
        void subu(uint32_t _rs, uint32_t _rt, uint32_t _rd);
        
};

#endif