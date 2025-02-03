#include "BranchPredictor.hpp"
#include "base.hpp"
#include "latch.hpp"
#include "stage.hpp"

/* Static Branch Preditior*/
uint32_t AlwaysNotTaken(uint32_t pc_){ // No Need BTB
    pc_ = pc_ + 4;
    return pc_; // Not Taken
}

uint32_t AlwaysTaken(uint32_t pc_){
    if(BTB[pc_] != 0){
        pc_ = BTB[pc_];
    }
    else{
        pc_ = pc_ + 4;
    }
    return pc_; // Taken
}

int BTFN_Predictor(uint32_t pc_){
    predict_pc = pc_;
    if(BTB[pc_] == 0){
        return 0;
    }
    else{
        if(BTB[pc_] < pc_){ // Backward Taken
            return 1;
        }
        else{ // Forward Not Taken
            return 0;
        }
    }
}

/* Dynamic Branch Predictor */

/* Last Time Predictor (One level) */
int OneBitPredictor(uint32_t pc_){
    int hit = 0, taken = 0;
    if(BTB[pc_] != 0){ // Target Address exist
        hit = 1;
        switch(TakenHistory[pc_]){
            case 0x0:
                taken = 0;
                break;
            case 0x1:
                taken = 1;
                break;
        }
    }
    return (hit & taken);
}

int TwoBitPredictor(uint32_t pc_){
    int hit = 0, taken = 0;
    if(BTB[pc_] != 0){
        hit = 1;
        switch(TakenHistory[pc_]){
            case 0x00:
            case 0x01:
                taken = 0;
                break;
            case 0x10:
            case 0x11:
                taken = 1;
                break;
        }
    }
    return (hit && taken);
}

/* Two-level Predictor*/
int GlobalHistoryPredictor(uint32_t pc_, int GHR){
    int hit = 0, taken = 0;
    if(BTB[pc_] != 0){
        hit = 1;
        switch(TakenHistory[GHR]){
            case 0x00:
            case 0x01:
                taken = 0;
                break;
            case 0x10:
            case 0x11:
                taken = 1;
                break;
        }
    }
    return (hit & taken); // pc+4 need after return
}

int GsharePredictor(uint32_t pc_, int GHR){
    int hit = 0, taken = 0;
    if(BTB[pc_] != 0){
        hit = 1;
        switch(TakenHistory[pc_ ^ GHR]){
            case 0x00:
            case 0x01:
                taken = 0;
                break;
            case 0x10:
            case 0x11:
                taken = 1;
                break;
        }
    }
    return (hit & taken);
}

int LocalBranchPredictor(uint32_t pc_){
    int hit = 0, taken = 0;
    if(BTB[pc_] != 0){
        hit = 1;
        switch(TakenHistory[PHT[pc_]]){
            case 0x00:
            case 0x01:
                taken = 0;
                break;
            case 0x10:
            case 0x11:
                taken = 1;
                break;
        }
    }
    return (hit & taken);
}