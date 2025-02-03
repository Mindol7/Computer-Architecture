#include "alg.hpp"
using namespace std;

uint32_t SCA(DMC_ * DMC){
    int ret = -2;
    while(1){
        for(int i = 0; i<128; i++){
            if(DMC[i].get_sca() == 0){
                ret = i;
                break;
            }
            else{
                DMC[i].set_sca(0);
            }
        }
        if(ret != -2) break;
    }
    return ret;
}