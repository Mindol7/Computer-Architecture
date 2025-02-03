#include "base.hpp"
#include "latch.hpp"
#include <iostream>
using namespace std;
class IF_ID;
class ID_EX;
class EX_MEM;
class MEM_WB;

EX_Signal::EX_Signal(){};
EX_Signal::EX_Signal(int RegDst_, bool ALU_Src_, int ALU_Op_, bool j_, bool jr_, bool jal_, bool branch_, bool sign_){
    RegDst = RegDst_; ALU_Src = ALU_Src_; ALU_Op = ALU_Op_; j = j_; jr = jr_;
    jal = jal_; branch = branch_; sign = sign_;
};
int EX_Signal::get_RegDst() const{return RegDst;}
bool EX_Signal::get_ALU_Src() const{return ALU_Src;}
bool EX_Signal::get_j() const{return j;}
bool EX_Signal::get_jr() const{return jr;}
bool EX_Signal::get_jal() const{return jal;}
bool EX_Signal::get_sign() const{return sign;}
int EX_Signal::get_ALU_Op() const{return ALU_Op;}
bool EX_Signal::get_branch() const{return branch;}
void EX_Signal::set_EX(int opcode_, int funct_){
    if(opcode_ == 0x3){ // jal
        RegDst = 2;
    }
    else if(opcode_ == 0x0){
        RegDst = 1;
    }
    else{
        RegDst = 0;
    }
    ALU_Src = (opcode_ != 0 && opcode_ != 0x04 && opcode_ != 0x05);
    j = (opcode_ == 0x02 || opcode_ == 0x03);
    jr = (opcode_ == 0 && funct_ == 0x08);
    jal = (opcode_ == 0x03);
    // sign?
    branch = (opcode_ == 0x04 || opcode_ == 0x05);
    ALU_Op = opcode_;
}

MEM_Signal::MEM_Signal(){};
MEM_Signal::MEM_Signal(bool MemWrite_, bool MemRead_, bool PcSrc1_){
    MemWrite = MemWrite_; MemRead = MemRead_; PcSrc1 = PcSrc1_;
}
bool MEM_Signal::get_MemWrite() const{return MemWrite;}
bool MEM_Signal::get_MemRead() const{return MemRead;}
bool MEM_Signal::get_PcSrc1() const{return PcSrc1;}
void MEM_Signal::set_MEM(int opcode_, int funct_){
    MemWrite = (opcode_ == 0x2b); // sw
    MemRead = (opcode_ == 0x23); // lw
    PcSrc1 = (opcode_ == 0x2 || opcode_ == 0x3);
}

WB_Signal::WB_Signal(){};
WB_Signal::WB_Signal(bool MemToReg_, bool RegWrite_, bool JalToReg_, bool PcSrc2_){
    MemToReg = MemToReg_; RegWrite = RegWrite_; JalToReg = JalToReg_; PcSrc2 = PcSrc2_;
};
bool WB_Signal::get_MemToReg() const{return MemToReg;}
bool WB_Signal::get_RegWrite() const{return RegWrite;}
bool WB_Signal::get_JalToReg() const{return JalToReg;}
bool WB_Signal::get_PcSrc2() const{return PcSrc2;}
void WB_Signal::set_WB(int opcode_, int funct_){
    MemToReg = (opcode_ == 0x23); // lw
    RegWrite = (opcode_ != 0x4 && opcode_ != 0x5 && opcode_ != 0x2 && opcode_ != 0x2b);
    JalToReg = (opcode_ == 0x3); // jal
    PcSrc2 = (opcode_ == 0x4 || opcode_ == 0x5);
}

// jal, ori?

/* Latch Implement */

IF_ID::IF_ID(){}
IF_ID::IF_ID(int valid_, int next_pc_, int inst_):valid(0), next_pc(0), instruction(0){
    valid = valid_;
    next_pc = next_pc_;
    instruction = inst_;
}
int IF_ID::get_valid() const{return valid;}

int IF_ID::get_next_pc() const{return next_pc;}

int IF_ID::get_inst() const{return instruction;}

void IF_ID::set_latch(int valid_, int next_pc_, int instruction_){
    valid = valid_;
    next_pc = next_pc_;
    instruction = instruction_;
}

ID_EX::ID_EX(){}
ID_EX::ID_EX(EX_Signal ex_, MEM_Signal mem_, WB_Signal wb_, int valid_, int next_pc_, int ReadData1_, int ReadData2_, int rs_, int rt_, int rd_, uint32_t imm_, int shamt_, int funct_, int opcode_){
    EX = ex_;
    MEM = mem_;
    WB = wb_;
    valid = valid_;
    next_pc = next_pc_;
    ReadData1 = ReadData1_;
    ReadData2 = ReadData2_;
    rs = rs_; rt = rt_; rd = rd_;
    imm = imm_;
    shamt = shamt_;
    funct = funct_;
    opcode = opcode_;
}
int ID_EX::get_valid() const{return valid;}
int ID_EX::get_next_pc() const{return next_pc;}
int ID_EX::get_ReadData1() const{return ReadData1;}
int ID_EX::get_ReadData2() const{return ReadData2;}
int ID_EX::get_rs() const{return rs;}
int ID_EX::get_rt() const{return rt;}
int ID_EX::get_rd() const{return rd;}
int ID_EX::get_ext_imm() const{return imm;}
int ID_EX::get_shamt() const{return shamt;}
int ID_EX::get_funct() const{return funct;}
int ID_EX::get_opcode() const{return opcode;}
void ID_EX::set_latch(EX_Signal ex_, MEM_Signal mem_, WB_Signal wb_, int valid_, int next_pc_, int ReadData1_, int ReadData2_,
                int rs_, int rt_, int rd_, uint32_t imm_, int shamt_, int funct_, int opcode_){
                ex_.set_EX(opcode_, funct_);
                mem_.set_MEM(opcode_, funct_);
                wb_.set_WB(opcode_, funct_);
                valid = valid_; next_pc = next_pc_; ReadData1 = ReadData1_; ReadData2 = ReadData2_;
                rs = rs_; rt = rt_; rd = rd_; imm = imm_; shamt = shamt_; funct = funct_; opcode = opcode_;
            } // signal은 decode단계에서 초기화되어야함

EX_MEM::EX_MEM(){}
EX_MEM::EX_MEM(MEM_Signal mem_, WB_Signal wb_, int valid_, int next_pc_, int ALU_Result_, int bcond_, int write_data_, int reg_dst_, uint32_t addr_){
    MEM = mem_;
    WB = wb_;
    valid = valid_;
    next_pc = next_pc_;
    ALU_Result = ALU_Result_;
    bcond = bcond_;
    write_data = write_data_;
    reg_dst = reg_dst_;
    addr = addr_;
}
int EX_MEM::get_valid() const{return valid;}
int EX_MEM::get_next_pc() const{return next_pc;}
int EX_MEM::get_ALU_Result() const{return ALU_Result;}
int EX_MEM::get_bcond() const{return bcond;}
int EX_MEM::get_write_data() const{return write_data;}
int EX_MEM::get_reg_dst() const{return reg_dst;}
uint32_t EX_MEM::get_addr() const{return addr;}
void EX_MEM::set_latch(MEM_Signal mem_, WB_Signal wb_, int valid_, int next_pc_, int ALU_Result_, int bcond_,
                    int write_data_, int reg_dst_, uint32_t addr_){
                         valid = valid_; next_pc = next_pc_, ALU_Result = ALU_Result_; bcond = bcond_;
                         write_data = write_data_; reg_dst = reg_dst_; addr = addr_;
                }

MEM_WB::MEM_WB(){}
MEM_WB::MEM_WB(WB_Signal wb_, int valid_, int next_pc_, int reg_dst_, int ALU_Result_, int mem_data_){
    WB = wb_;
    valid = valid_;
    next_pc = next_pc_;
    reg_dst = reg_dst_;
    ALU_Result = ALU_Result_;
    mem_data = mem_data_;
}
int MEM_WB::get_valid() const{return valid;}
int MEM_WB::get_next_pc() const{return next_pc;}
int MEM_WB::get_ALU_Result() const{return ALU_Result;}
int MEM_WB::get_reg_dst() const{return reg_dst;}
int MEM_WB::get_mem_data() const{return mem_data;}
void MEM_WB::set_latch(WB_Signal wb_, int valid_, int next_pc_, int reg_dst_, int ALU_Result_, int mem_data_){
    valid = valid_; next_pc = next_pc_; reg_dst = reg_dst_; ALU_Result = ALU_Result_; mem_data = mem_data_;
}

