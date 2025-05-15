#pragma once

#include <cstdint>

class Mapper
{
public:
    Mapper(uint8_t prg_banks, uint8_t chr_banks);
    virtual ~Mapper() = default;

    virtual bool ReadCPU(uint16_t addr, uint32_t &mapped_addr) = 0;
    virtual bool WriteCPU(uint16_t addr, uint32_t &mapped_addr) = 0;
    virtual bool ReadPPU(uint16_t addr, uint32_t &mapped_addr) = 0;
    virtual bool WritePPU(uint16_t addr, uint32_t &mapped_addr) = 0;

protected:
    uint8_t mBanksPRG;
    uint8_t mBanksCHR;
};