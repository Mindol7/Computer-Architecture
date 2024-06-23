#ifndef CLASS_H
#define CLASS_H
#include <cstdint>
/*
** File Name : mips_class.h
** Contents : Declare MIPS_ Class & Related Function
** Author : Min-Hyuk-Cho
** Date : 24/05/01
*/

extern int Memory[0xfffffff]; // Memory Max Size
extern int R[32]; // Register 0 ~ 31
extern int pc; // Program Counter -> PC = PC + 4 (int = 4byte 이므로 코드 상에서는 PC = PC + 1로 작성)
extern int idx; // Program을 Memory에 등록할 때, idx를 통해 등록 시킴
extern int memory_access_count; // Memory Access State Count
extern int branch_count; // Branch Count
extern int r_count ,i_count, j_count; // R,I,J Create Count
extern int cycle_count; // Total Program Cycle Count
extern int temp_pc; // Jump 후 복귀할 때 기존 주소를 복귀 시켜줌
extern int jump_inst, jump_opcode; // Jump Instruction & Jump Opcode

class MIPS_{
    private:
        int _instruction, _opcode;
    public:
        MIPS_(int _inst);
        int opcode_tok();
        int get_inst() const;
        virtual void tok_inst(){}; // Pure Virtual Funcion
        virtual void select_operation(){}; // Pure Virtual Function
};

#endif