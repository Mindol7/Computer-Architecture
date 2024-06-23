/*
** [Mobile System Engineering 32204292 Cho-Min-Hyuk]
** Project : Single-Cycle Programming
** Submit Date : 24/05/01
*/

/*
** File Name : main.cpp
** Contents : Initialize Variable & Write main Function
*/ 

#include <iostream>
#include <fstream>
#include "mips_class.h"
#include "R_Type_Class.h"
#include "I_Type_Class.h"
#include "J_Type_Class.h"
using namespace std;

int Memory[0xfffffff] = {0, }; // Memory Max Size
int R[32] = {0, }; // Register 0 ~ 31, Initialize All value in Register is 0.

// All Variable is set 0.
int pc = 0x00000000; 
int idx = 0;
int memory_access_count = 0;
int branch_count = 0;
int r_count = 0, i_count = 0, j_count = 0;
int cycle_count = 1;
int temp_pc = 0;
int jump_inst = 0x00000000, jump_opcode = 0x000000;

int main(){
    // Read .bin file
    fstream input_file("../test_prog/simple3.bin", ios::in);
    
    if(input_file.fail() == true){
        cerr<<"Fail to the open file"<<endl;
        exit(1);
    }
    int var;
    int ret = 0x00000000;

    cout<<"===============Convert Little Endian to Big Endian==============="<<endl;
    cout<<"================================================================="<<endl;
    while(input_file.eof() != true){
        input_file.read(reinterpret_cast<char*>(&var), sizeof(var));
        
        ret = (int)var;
        cout<<"Little Endian : 0x"<<hex<<ret<<endl;

        // Conver to Big Endian
        // Loading
        unsigned int b1, b2, b3, b4;
        b1 = ret & 0x000000ff;
        b2 = ret & 0x0000ff00;
        b3 = ret & 0x00ff0000;
        b4 = (ret>>24) & 0x000000ff;

        int res = (b1<<24) | (b2<<8) | (b3>>8) | b4;
        cout<<"Big Endian : 0x"<<hex<<res<<endl;
        int instruction = res;
        
        Memory[idx] = instruction;
        idx++;
    }
    input_file.close();
    R[29] = 0x1000000;
    R[31] = 0xffffffff;
    cout<<"================================================================="<<endl;
    while(1){
        // Fetch
        cout<<"=======Fetch State======="<<endl;
        int opcode;
        MIPS_ * mips_inst = new MIPS_(Memory[pc]);
        opcode = mips_inst->opcode_tok();

        if(Memory[pc] == 0 && Memory[pc+1] == 0 || pc == 0xffffffff){
            cout<<"Program Halt"<<endl;
            break;
        }
        
        cout<<"The instruction is "<< Memory[pc]<<endl;

        // R-type
        if(opcode == 0){
            cout<<"R-Type is Created"<<endl;
            r_count++;
            MIPS_ * r_type_inst = new R_Type(Memory[pc]);
            r_type_inst->tok_inst();
            r_type_inst->select_operation();
            delete r_type_inst;
        }
        // J-type
        else if(opcode == 2 || opcode == 3){
            cout<<"J-Type is Created"<<endl;
            j_count++;
            MIPS_ * j_type_inst = new J_Type(Memory[pc]);
            j_type_inst->tok_inst();
            j_type_inst->select_operation();
            delete j_type_inst;
        }
        // I-type
        else{
            cout<<"I-Type is Created"<<endl;
            i_count++;
            MIPS_ * i_type_inst = new I_Type(Memory[pc]);
            i_type_inst->tok_inst();
            i_type_inst->select_operation();
            delete i_type_inst;
        }    
        cout<<"Cycle "<<cycle_count++<<" is Complete"<<endl;
        delete mips_inst;
    }

    cout<<"=======The Program is Finished======="<<endl;
    cout<<"=======Completion of The Program======="<<endl;
    cout<<"The Result (r2) : "<<dec<<R[2]<<endl;
    cout<<"Total Executed Instruction : "<<r_count+i_count+j_count<<endl;
    cout<<"R-Type Instruction : "<<r_count<<endl;
    cout<<"I-Type Instruction : "<<i_count<<endl;
    cout<<"J-Type Instruction : "<<j_count<<endl;
    cout<<"Memory Access Instructions : "<<memory_access_count<<endl;
    cout<<"Branch Instructions : "<<branch_count<<endl;
    cout<<"====================================="<<endl;
    
    cout<<"Bye"<<endl;
    cout<<"================================================================="<<endl;
    return 0;
}