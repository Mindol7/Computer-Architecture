#ifndef _LATCH_HPP__
#define _LATCH_HPP__

#include "base.hpp"
#include "stage.hpp"

/* Control Unit Class */
class EX_Signal{
    private:
        int RegDst = 0;
        bool ALU_Src = false;
        int ALU_Op = -1;
        bool j = false;
        bool jr = false, jal = false;
        bool branch = false;
        bool sign = false;
    public:
        EX_Signal();
        EX_Signal(int RegDst_, bool ALU_Src_, int ALU_Op_, bool j_,
                    bool jr_, bool jal_, bool branch_, bool sign_);
        int get_RegDst() const;
        bool get_ALU_Src() const;
        bool get_j() const;
        bool get_jr() const;
        bool get_jal() const;
        bool get_sign() const;
        int get_ALU_Op() const;
        bool get_branch() const;
        void set_EX(int opcode_, int funct_);
};

class MEM_Signal{
    private:
        bool MemWrite = false;
        bool MemRead = false;
        bool PcSrc1 = false;
    public:
        MEM_Signal();
        MEM_Signal(bool MemWrite_, bool MemRead_, bool PcSrc1_);
        bool get_MemWrite() const; 
        bool get_MemRead() const;
        bool get_PcSrc1() const;
        void set_MEM(int opcode_, int funct_);
};

class WB_Signal{
    private:
        bool MemToReg = false;
        bool RegWrite = false;
        bool JalToReg = false;
        bool PcSrc2 = false;
    public:
        WB_Signal();
        WB_Signal(bool MemToReg_, bool RegWrite_, bool JalToReg_, bool PcSrc2_);
        bool get_MemToReg() const;
        bool get_RegWrite() const;
        bool get_JalToReg() const;
        bool get_PcSrc2() const;
        void set_WB(int opcode_, int funct_);
};

/* IF/ID Latch */

class IF_ID{
    public:
        int valid;
        int next_pc;
        int instruction;
    public:
        IF_ID();
        IF_ID(int valid_, int next_pc_, int inst_);
        int get_valid() const;
        int get_next_pc() const;
        int get_inst() const;
        void set_latch(int valid_, int next_pc_, int instruction_);
};

/* ID/EX Latch */

class ID_EX{
    public:
        EX_Signal EX;
        MEM_Signal MEM;
        WB_Signal WB;
        int valid;
        int next_pc;
        int ReadData1, ReadData2;
        int rs, rt, rd;
        uint32_t imm;
        int shamt, funct;
        int opcode;
    public:
        ID_EX();
        ID_EX(EX_Signal ex_, MEM_Signal mem_, WB_Signal wb_,int valid_, int next_pc_, int ReadData1_, int ReadData2_, int rs_, int rt_, int rd_, uint32_t imm_, int shamt_, int funct_, int opcode_);
        int get_valid() const;
        int get_next_pc() const;
        int get_ReadData1() const;
        int get_ReadData2() const;
        int get_rs() const;
        int get_rt() const;
        int get_rd() const;
        int get_ext_imm() const;
        int get_shamt() const;
        int get_funct() const;
        int get_opcode() const;
        void set_latch(EX_Signal ex_, MEM_Signal mem_, WB_Signal wb_, int valid_, int next_pc_, int ReadData1_, int ReadData2_,
                int rs_, int rt_, int rd_, uint32_t imm_, int shamt_, int funct_, int opcode_);
};

/* EX/MEM Latch */

class EX_MEM{
    public:
        MEM_Signal MEM;
        WB_Signal WB;
        int valid;
        int next_pc;
        int ALU_Result;
        int bcond;
        int write_data;
        int reg_dst;
        uint32_t addr;
    public:
        EX_MEM();
        EX_MEM(MEM_Signal mem_, WB_Signal wb_, int valid_, int next_pc_, int ALU_Result_, int bcond_, int write_data_, int reg_dst_, uint32_t addr_);
        int get_valid() const;
        int get_next_pc() const;
        int get_ALU_Result() const;
        int get_bcond() const;
        int get_write_data() const;
        int get_reg_dst() const;
        uint32_t get_addr() const;
        void set_latch(MEM_Signal mem_, WB_Signal wb_, int valid_, int next_pc_, int ALU_Result_, int bcond_,
                    int write_data_, int reg_dst_, uint32_t addr_);
};

/* MEM/WB Latch */

class MEM_WB{
    public:
        WB_Signal WB;
        int valid;
        int next_pc;
        int reg_dst;
        int ALU_Result;
        int mem_data;
    public:
        MEM_WB();
        MEM_WB(WB_Signal wb_, int valid_, int next_pc_, int reg_dst_, int ALU_Result_, int mem_data_);
        int get_valid() const;
        int get_next_pc() const;
        int get_ALU_Result() const;
        int get_reg_dst() const;
        int get_mem_data() const;
        void set_latch(WB_Signal wb_, int valid_, int next_pc_, int reg_dst_, int ALU_Result_, int mem_data_);
};

// /* Latch Initial */
// extern IF_ID IFID_Latch[2];
// extern ID_EX IDEX_Latch[2];
// extern EX_MEM EXMEM_Latch[2];
// extern MEM_WB MEMWB_Latch[2];

#endif