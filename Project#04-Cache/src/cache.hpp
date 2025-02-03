#ifndef _CACHE_HPP__
#define _CACHE_HPP__
#include <cstdint>

/* Direct Mapped Cache, 64Byte*/
class DMC_{
    private:
        uint32_t DMC_tag;
        uint32_t sca;
        uint32_t valid = 0;
        uint32_t dirty;
        int data[16];
    public:
        DMC_();
        void DMC_Init();
        void Fill_DMC(uint32_t tag_, uint32_t sca_, uint32_t valid_, uint32_t dirty_, uint32_t addr_);
        uint32_t DMC_Read(uint32_t addr_);

        int DMC_Write_Through(uint32_t addr_, int data_);
        int DMC_Write_Back(uint32_t addr_, int data_);

        uint32_t get_sca() const;

        void set_tag(uint32_t tag_);
        void set_sca(uint32_t sca_);
        void set_valid(uint32_t valid_);
        void set_dirty(uint32_t dirty_);
        void set_data(uint32_t addr_);      
};


#endif