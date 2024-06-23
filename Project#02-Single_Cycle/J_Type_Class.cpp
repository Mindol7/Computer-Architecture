#include "J_Type_Class.h"
#include "R_Type_Class.h"
#include "I_Type_Class.h"
#include <iostream>
using namespace std;

/*
** File Name : J_Type_class.cpp
** Contents : Implement J_Type Class & Related Function
** Author : Min-Hyuk-Cho
** Date : 24/05/01
*/

J_Type::J_Type(int instruction):MIPS_(instruction){
        j_instruction = MIPS_::get_inst();
        j_opcode = MIPS_::opcode_tok();
}

void J_Type::tok_inst(){
    cout<<"=======Decode State======="<<endl;
    address = (j_instruction & 0x03ffffff);
    address = address << 2;
    JumpAddr = (pc & 0x3f000000) | address;

    cout<<"Opcode : 0x"<<hex<<j_opcode<<", address : "<<address<<endl;
}

void J_Type::select_operation(){
    cout<<"=======Execute State======="<<endl;
    switch(j_opcode){
        case 0x02:
            jump();
            return;
        case 0x03:
            jump_and_link();
            return;
        default:
            cout<<"There is no matching operation"<<endl;
            return;
    }
}

void J_Type::jump(){
    cout<<"=======Jump Operation======="<<endl;
    pc = JumpAddr / 4;
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

void J_Type::jump_and_link(){
    cout<<"=======Jump And Link Operantion======="<<endl;
    R[31] = pc + 2;
    
    pc = JumpAddr / 4;
    do{
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
    }while(pc != R[31] - 2);

    pc = R[31];
    cout<<"Return PC: "<<pc<<endl;
}