#include "base.hpp"
#include "fr.hpp"
#include <iostream>
using namespace std;
/* Forwarding Unit */

int Forward_Rs(uint32_t rs_, uint32_t regDst1_, uint32_t regDst2_){
    if(rs_ == regDst1_){return 1;} // dist = 1
    else if(rs_ == regDst2_){return 2;} // dist = 2
    else{return 0;}
}
int Forward_Rt(uint32_t rt_, uint32_t regDst1_, uint32_t regDst2_){
    if(rt_ == regDst1_){return 1;} // dist = 1
    else if(rt_ == regDst2_){return 2;} // dist = 2
    else{return 0;}
}