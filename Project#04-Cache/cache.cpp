#include "base.hpp"
#include "cache.hpp"
#include "alg.hpp"
#include "stage.hpp"
using namespace std;

class DMC_;

DMC_::DMC_(){
    DMC_tag = 0; valid = 0; sca = 0; dirty = 0;
    for(int i = 0; i<16; i++){
        data[i] = 0;
    }
};

void DMC_::DMC_Init(){
    DMC_tag = 0;
    sca = 0;
    valid = 0;
    dirty = 0;
    for(int i = 0; i<16; i++){
        data[i] = 0;
    }
}

void DMC_::Fill_DMC(uint32_t tag_, uint32_t sca_, uint32_t valid_, uint32_t dirty_, uint32_t addr_){
    DMC_tag = tag_;
    sca = sca_;
    valid = valid_;
    dirty = dirty_;
    addr_ = addr_ & 0xffffffc;
    for(int i = 0; i<16; i++){
        data[i] = memory[addr_ / 4 + i];
    }
}

uint32_t DMC_::DMC_Read(uint32_t addr_){
    if(valid == 0){
        cache_miss++; // Cold Miss
        Fill_DMC(tag, 0, 1, 0, addr_); 
        return data[offset / 4];
    }
    else{
        if(DMC_tag == tag){ // valid = 1 & hit
            cache_hit++;
            set_sca(1);
            return data[offset / 4];
        }
        else{ // Coflict Miss
            cache_miss++;
            return MISS;
        }
    }
}

int DMC_::DMC_Write_Through(uint32_t addr_, int data_){
    /* Write through */

    if(valid == 0){
        cache_miss++;
        Fill_DMC(tag, 0, 1, 0, addr_);
        data[offset / 4] = data_;
        memory[addr_ / 4] = data_;
        set_dirty(1);
        return 1;
    }
    else{
        if(DMC_tag == tag){ // valid & hit
            cache_hit++;
            set_sca(1);
            data[offset / 4] = data_;
            memory[addr_ / 4] = data_;
            set_dirty(1);
            return 1;
        }
        else{
            cache_miss++;
            return MISS;
        }
    }
}

int DMC_::DMC_Write_Back(uint32_t addr_, int data_){
    /* Write Back */

    if(valid == 0){
        cache_miss++;
        Fill_DMC(tag, 0, 1, 0, addr_);
        data[offset / 4] = data_;
        set_dirty(1);
        return 2;
    }
    else{
        if(DMC_tag == tag){ // valid & hit
            cache_hit++;
            set_sca(1);
            data[offset / 4] = data_;
            set_dirty(1);
            return 2;
        }
        else{
            cache_miss++;
            return MISS;
        }
    }
}

uint32_t DMC_::get_sca() const{
    return sca;
}

void DMC_::set_tag(uint32_t tag_){
    DMC_tag = tag_;
}

void DMC_::set_sca(uint32_t sca_){
    sca = sca_;
}

void DMC_::set_valid(uint32_t valid_){
    valid = valid_;
}

void DMC_::set_dirty(uint32_t dirty_){
    dirty = dirty_;
}

void DMC_::set_data(uint32_t addr_){
    for(int i = 0; i<16; i++){
        data[i] = memory[addr_ / 4 + i];
    }
}