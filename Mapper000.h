#pragma once

#include "Mapper.h"

class Mapper000 : public Mapper
{
public:
    Mapper000(uint8_t prg_banks, uint8_t chr_banks);
    ~Mapper000();

    bool ReadCPU(uint16_t addr, uint32_t &mapped_addr) override;
    bool WriteCPU(uint16_t addr, uint32_t &mapped_addr) override;
    bool ReadPPU(uint16_t addr, uint32_t &mapped_addr) override;
    bool WritePPU(uint16_t addr, uint32_t &mapped_addr) override;
};