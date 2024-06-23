#ifndef J_TYPE_CLASS_H
#define J_TYPE_CLASS_H
#include "mips_class.h"

/*
** File Name : J_Type_Class.h
** Contents : Declare J_Type_ Class & Related Function
** Author : Min-Hyuk-Cho
** Date : 24/05/01
*/

class J_Type:public MIPS_{
    private:
        int j_instruction;
        uint32_t j_opcode, address, ext_address;
        uint32_t JumpAddr;
    public:
        J_Type(int instruction);
        void tok_inst() override;
        void select_operation() override;
        void jump();
        void jump_and_link();
};

#endif