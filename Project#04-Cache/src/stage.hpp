#ifndef _STAGE_HPP__
#define _STAGE_HPP__

extern void Fetch();
extern void Decode();
extern void Execution();
extern void MEM();
extern void WB();
extern void copy_latch();
extern void flush_IF_ID();
extern void flush_ID_EX();
#endif 