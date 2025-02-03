#ifndef _BRANCHPREDICTOR_HPP__
#define _BRANCHPREDICTOR_HPP__

#include <cstdint>

extern uint32_t AlwaysNotTaken(uint32_t pc_);
extern uint32_t AlwaysTaken(uint32_t pc_);
extern int BTFN_Predictor(uint32_t pc_);
extern int OneBitPredictor(uint32_t pc_);
extern int TwoBitPredictor(uint32_t pc_);
extern int GlobalHistoryPredictor(uint32_t pc_, int GHR);
extern int GsharePredictor(uint32_t pc_, int GHR);
extern int LocalBranchPredictor(uint32_t pc_);

#endif