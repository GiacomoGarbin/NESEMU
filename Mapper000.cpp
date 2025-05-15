#include "Mapper000.h"

Mapper000::Mapper000(uint8_t prg_banks, uint8_t chr_banks)
    : Mapper(prg_banks, chr_banks)
{
}

Mapper000::~Mapper000()
{
}

bool Mapper000::ReadCPU(uint16_t addr, uint32_t &mapped_addr)
{
    if ((0x8000 <= addr) && (addr <= 0xFFFF))
    {
        mapped_addr = addr & ((mBanksPRG > 1) ? 0x7FFF : 0x3FFF);
        return true;
    }

    return false;
}

bool Mapper000::WriteCPU(uint16_t addr, uint32_t &mapped_addr)
{
    if ((0x8000 <= addr) && (addr <= 0xFFFF))
    {
        mapped_addr = addr & ((mBanksPRG > 1) ? 0x7FFF : 0x3FFF);
        return true;
    }

    return false;
}

bool Mapper000::ReadPPU(uint16_t addr, uint32_t &mapped_addr)
{
    if ((0x0000 <= addr) && (addr <= 0x1FFF))
    {
        mapped_addr = addr;
        return true;
    }

    return false;
}

bool Mapper000::WritePPU(uint16_t addr, uint32_t &mapped_addr)
{
    if ((0x0000 <= addr) && (addr <= 0x1FFF))
    {
        // treat as RAM
        mapped_addr = addr;
        return true;
    }

    return false;
}
