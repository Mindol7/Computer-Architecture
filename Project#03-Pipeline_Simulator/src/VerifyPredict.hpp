#ifndef _VERTIFY_PREDICT_HPP__
#define _VERTIFY_PREDICT_HPP__

#include <cstdint>

extern void VerifyAlwaysNotTaken(uint32_t addr_, int taken_);
extern void VerifyAlwaysTaken(uint32_t pc_, uint32_t addr_, int taken_);
extern void VerifyBTFN(uint32_t addr_, uint32_t pc_, int taken_);
extern void VerifyOneBitPredictor(uint32_t addr_, uint32_t pc_, int taken_);
extern void VerifyTwoBitPredictor(uint32_t addr_, uint32_t pc_, int taken_);
extern void VerifyGHPredictor(uint32_t addr_, uint32_t pc_, int taken_);
extern void VerifyGSharePredictor(uint32_t addr_, uint32_t pc_, int taken_);
extern void VerifyLBPredictor(uint32_t addr_, uint32_t pc_, int taken_);

#endif