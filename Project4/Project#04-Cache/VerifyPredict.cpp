#include "base.hpp"
#include "VerifyPredict.hpp"
#include "BranchPredictor.hpp"
#include "stage.hpp"
#include <iostream>
using namespace std;
/* Verify Branch Predict */

/* Verify Static Branch Prediction */

void VerifyAlwaysNotTaken(uint32_t addr_, int taken_){
    if(taken_ != 1){ // Predict
        predict_count++;
    }
    else{ // Mispredict
        flush_IF_ID();
        flush_ID_EX();
        pc = addr_; 
        mispredict_count++;
    }
}

void VerifyAlwaysTaken(uint32_t pc_, uint32_t addr_, int taken_){
    if(BTB[pc_] != 0){
        if(taken_ == 1){ // Predict
            predict_count++;
        }
        else{
            flush_IF_ID();
            flush_ID_EX();
            pc = pc_ + 4;
            mispredict_count++;
        }
    }
    else{
        if(taken_ == 0){
            predict_count++;
        }
        else{
            flush_IF_ID();
            flush_ID_EX();
            BTB[pc_] = addr_;
            pc = addr_;
            mispredict_count++;
        }
    }
}

void VerifyBTFN(uint32_t addr_, uint32_t pc_, int taken_){
    if(BTB[pc_] < pc_){
        if(taken_){
            predict_count++;
        }
        else{
            flush_IF_ID();
            flush_ID_EX();
            pc = pc + 4;
            mispredict_count++;
        }
    }
    else{ // Forward
        if(!taken_){
            predict_count++;
        }
        else{
            flush_IF_ID();
            flush_ID_EX();
            pc = addr_;
            if(addr_ < pc_){
                BTB[pc_] = addr_; // BTB Update
            }
            mispredict_count++;
        }
    }
}

/* Verify Dynamic Branch Predictor */

void VerifyOneBitPredictor(uint32_t addr_, uint32_t pc_, int taken_){
    if(BTB[pc_] != 0){ 
        if(TakenHistory[pc_] == 0x0){ // Predict Not Taken
            if(taken_){
                flush_IF_ID();
                flush_ID_EX();
                pc = addr_;
                BTB[pc_] = addr_;
                TakenHistory[pc_] = 1;  
                mispredict_count++;
            }
            else{
                TakenHistory[pc_] = 0;
                predict_count++;
            }
        }
        else{ // Predict Taken
            if(taken_){
                TakenHistory[pc_] = 1;
                predict_count++;                
            }
            else{
                flush_IF_ID();
                flush_ID_EX();
                pc = addr_;
                BTB[pc_] = addr_;
                TakenHistory[pc_] = 0;
                mispredict_count++;
            }
        }
    }
    else{ // 에측하는게 처음임
        if(TakenHistory[pc_] == 0x0){ // Predict Not Taken
            if(taken_){
                flush_IF_ID();
                flush_ID_EX();
                pc = addr_;
                BTB[pc_] = addr_;
                TakenHistory[pc_] = 1;
                mispredict_count++;
            }
            else{
                TakenHistory[pc_] = 0;
                predict_count++;
            }
        }
        else{ // Predict Taken
            if(taken_){
                TakenHistory[pc_] = 1;
                predict_count++;                
            }
            else{
                flush_IF_ID();
                flush_ID_EX();
                pc = addr_;
                BTB[pc_] = addr_;
                TakenHistory[pc_] = 0;
                mispredict_count++;
            }
        }
    }
}

void VerifyTwoBitPredictor(uint32_t addr_, uint32_t pc_, int taken_){
    if(BTB[pc_] != 0){
        if(TakenHistory[pc_] < 2){ // Predict Not Taken (0x00, 0x01)
            if(taken_){
                flush_IF_ID();
                flush_ID_EX();
                pc = addr_;
                BTB[pc_] = addr_;
                TakenHistory[pc_]++;
                if(TakenHistory[pc_] > 3){
                    TakenHistory[pc_] = 3;
                }  
                mispredict_count++;
            }
            else{
                TakenHistory[pc_]--;
                if(TakenHistory[pc_] < 0){
                    TakenHistory[pc_] = 0;
                }
                predict_count++;
            }
        }
        else{ // Predict Taken (0x02, 0x03)
            if(taken_){
                TakenHistory[pc_]++;
                if(TakenHistory[pc_] > 3){
                    TakenHistory[pc_] = 3;
                } 
                predict_count++;                
            }
            else{
                flush_IF_ID();
                flush_ID_EX();
                pc = addr_;
                BTB[pc_] = addr_;
                TakenHistory[pc_]--;
                if(TakenHistory[pc_] < 0){
                    TakenHistory[pc_] = 0;
                }
                mispredict_count++;
            }
        }
    }
    else{ // 에측하는게 처음임
        if(TakenHistory[pc_] < 2){ // Predict Not Taken
            if(taken_){
                flush_IF_ID();
                flush_ID_EX();
                pc = addr_;
                BTB[pc_] = addr_;
                TakenHistory[pc_]++;
                if(TakenHistory[pc_] > 3){
                    TakenHistory[pc_] = 3;
                }
                mispredict_count++;
            }
            else{
                TakenHistory[pc_]--;
                if(TakenHistory[pc_] < 0){
                    TakenHistory[pc_] = 0;
                }
                predict_count++;
            }
        }
        else{ // Predict Taken
            if(taken_){
                TakenHistory[pc_]++;
                if(TakenHistory[pc_] > 3){
                    TakenHistory[pc_] = 3;
                } 
                predict_count++;                
            }
            else{
                flush_IF_ID();
                flush_ID_EX();
                pc = addr_;
                BTB[pc_] = addr_;
                TakenHistory[pc_]--;
                if(TakenHistory[pc_] < 0){
                    TakenHistory[pc_] = 0;
                }
                mispredict_count++;
            }
        }
    }
}

void VerifyGHPredictor(uint32_t addr_, uint32_t pc_, int taken_){
    if(BTB[pc_] != 0){
        if(TakenHistory[GHR] < 2){ // Predict Not Taken
            if(taken_){
                flush_IF_ID();
                flush_ID_EX();
                pc = addr_;
                BTB[pc_] = addr_;
                TakenHistory[GHR]++;
                if(TakenHistory[GHR] > 3){
                    TakenHistory[GHR] = 3;
                }  
                mispredict_count++;
            }
            else{
                TakenHistory[GHR]--;
                if(TakenHistory[GHR] < 0){
                    TakenHistory[GHR] = 0;
                }
                predict_count++;
            }
        }
        else{ // Predict Taken
            if(taken_){
                TakenHistory[GHR]++;
                if(TakenHistory[GHR] > 3){
                    TakenHistory[GHR] = 3;
                } 
                predict_count++;                
            }
            else{
                flush_IF_ID();
                flush_ID_EX();
                pc = addr_;
                BTB[pc_] = addr_;
                TakenHistory[GHR]--;
                if(TakenHistory[GHR] < 0){
                    TakenHistory[GHR] = 0;
                }
                mispredict_count++;
            }
        }
    }
    else{ 
        if(TakenHistory[GHR] < 2){ // Predict Not Taken
            if(taken_){
                flush_IF_ID();
                flush_ID_EX();
                pc = addr_;
                BTB[pc_] = addr_;
                TakenHistory[GHR]++;
                if(TakenHistory[GHR] > 3){
                    TakenHistory[GHR] = 3;
                }
                mispredict_count++;
            }
            else{
                TakenHistory[GHR]--;
                if(TakenHistory[GHR] < 0){
                    TakenHistory[GHR] = 0;
                }
                predict_count++;
            }
        }
        else{ // Predict Taken
            if(taken_){
                TakenHistory[GHR]++;
                if(TakenHistory[GHR] > 3){
                    TakenHistory[GHR] = 3;
                } 
                predict_count++;                
            }
            else{
                flush_IF_ID();
                flush_ID_EX();
                pc = addr_;
                BTB[pc_] = addr_;
                TakenHistory[GHR]--;
                if(TakenHistory[GHR] < 0){
                    TakenHistory[GHR] = 0;
                }
                mispredict_count++;
            }
        }
    }
}

void VerifyGSharePredictor(uint32_t addr_, uint32_t pc_, int taken_){
    if(BTB[pc_] != 0){
        if(TakenHistory[pc_ ^ GHR] < 2){ // Predict Not Taken
            if(taken_){
                flush_IF_ID();
                flush_ID_EX();
                pc = addr_;
                BTB[pc_] = addr_;
                TakenHistory[pc_ ^ GHR]++;
                if(TakenHistory[pc_ ^ GHR] > 3){
                    TakenHistory[pc_ ^ GHR] = 3;
                }  
                mispredict_count++;
            }
            else{
                TakenHistory[pc_ ^ GHR]--;
                if(TakenHistory[pc_ ^ GHR] < 0){
                    TakenHistory[pc_ ^ GHR] = 0;
                }
                predict_count++;
            }
        }
        else{ // Predict Taken
            if(taken_){
                TakenHistory[pc_ ^ GHR]++;
                if(TakenHistory[pc_ ^ GHR] > 3){
                    TakenHistory[pc_ ^ GHR] = 3;
                } 
                predict_count++;                
            }
            else{
                flush_IF_ID();
                flush_ID_EX();
                pc = addr_;
                BTB[pc_] = addr_;
                TakenHistory[pc_ ^ GHR]--;
                if(TakenHistory[pc_ ^ GHR] < 0){
                    TakenHistory[pc_ ^ GHR] = 0;
                }
                mispredict_count++;
            }
        }
    }
    else{ 
        if(TakenHistory[pc_ ^ GHR] == 0x0){ // Predict Not Taken
            if(taken_){
                flush_IF_ID();
                flush_ID_EX();
                pc = addr_;
                BTB[pc_] = addr_;
                TakenHistory[pc_ ^ GHR]++;
                if(TakenHistory[pc_ ^ GHR] > 3){
                    TakenHistory[pc_ ^ GHR] = 3;
                }
                mispredict_count++;
            }
            else{
                TakenHistory[pc_ ^ GHR]--;
                if(TakenHistory[pc_ ^ GHR] < 0){
                    TakenHistory[pc_ ^ GHR] = 0;
                }
                predict_count++;
            }
        }
        else{ // Predict Taken
            if(taken_){
                TakenHistory[pc_ ^ GHR]++;
                if(TakenHistory[pc_ ^ GHR] > 3){
                    TakenHistory[pc_ ^ GHR] = 3;
                } 
                predict_count++;                
            }
            else{
                flush_IF_ID();
                flush_ID_EX();
                pc = addr_;
                BTB[pc_] = addr_;
                TakenHistory[pc_ ^ GHR]--;
                if(TakenHistory[pc_ ^ GHR] < 0){
                    TakenHistory[pc_ ^ GHR] = 0;
                }
                mispredict_count++;
            }
        }
    }
}

void VerifyLBPredictor(uint32_t addr_, uint32_t pc_, int taken_){
    if(BTB[pc_] != 0){
        if(TakenHistory[PHT[pc_]] < 2){ // Predict Not Taken
            if(taken_){
                flush_IF_ID();
                flush_ID_EX();
                pc = addr_;
                BTB[pc_] = addr_;
                TakenHistory[PHT[pc_]]++;
                if(TakenHistory[PHT[pc_]] > 3){
                    TakenHistory[PHT[pc_]] = 3;
                }  
                mispredict_count++;
            }
            else{
                TakenHistory[PHT[pc_]]--;
                if(TakenHistory[PHT[pc_]] < 0){
                    TakenHistory[PHT[pc_]] = 0;
                }
                predict_count++;
            }
        }
        else{ // Predict Taken
            if(taken_){
                TakenHistory[PHT[pc_]]++;
                if(TakenHistory[PHT[pc_]] > 3){
                    TakenHistory[PHT[pc_]] = 3;
                } 
                predict_count++;                
            }
            else{
                flush_IF_ID();
                flush_ID_EX();
                pc = addr_;
                BTB[pc_] = addr_;
                TakenHistory[PHT[pc_]]--;
                if(TakenHistory[PHT[pc_]] < 0){
                    TakenHistory[PHT[pc_]] = 0;
                }
                mispredict_count++;
            }
        }
    }
    else{ // 에측하는게 처음임
        if(TakenHistory[PHT[pc_]] == 0x0){ // Predict Not Taken
            if(taken_){
                flush_IF_ID();
                flush_ID_EX();
                pc = addr_;
                BTB[pc_] = addr_;
                TakenHistory[PHT[pc_]]++;
                if(TakenHistory[PHT[pc_]] > 3){
                    TakenHistory[PHT[pc_]] = 3;
                } 
                mispredict_count++;
            }
            else{
                TakenHistory[PHT[pc_]]--;
                if(TakenHistory[PHT[pc_]] < 0){
                    TakenHistory[PHT[pc_]] = 0;
                }
                predict_count++;
            }
        }
        else{ // Predict Taken
            if(taken_){
                TakenHistory[PHT[pc_]]++;
                if(TakenHistory[PHT[pc_]] > 3){
                    TakenHistory[PHT[pc_]] = 3;
                } 
                predict_count++;                
            }
            else{
                flush_IF_ID();
                flush_ID_EX();
                pc = addr_;
                BTB[pc_] = addr_;
                TakenHistory[PHT[pc_]]--;
                if(TakenHistory[PHT[pc_]] < 0){
                    TakenHistory[PHT[pc_]] = 0;
                }
                mispredict_count++;
            }
        }
    }
}