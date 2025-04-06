#include "Bus.h"

Bus::Bus()
{
    // clear the RAM

    for (uint8_t& byte : mRAM)
    {
        byte = 0x00;
    }

    // connect the CPU to this BUS
    mCPU.ConnectBus(this);
}

Bus::~Bus()
{

}

void Bus::Write(uint16_t addr, uint8_t data)
{
    if ((0x0000 <= addr) && (addr <= 0xFFFF))
    {
        mRAM[addr] = data;
    }
}

uint8_t Bus::Read(uint16_t addr, bool bReadOnly = false)
{
    if ((0x0000 <= addr) && (addr <= 0xFFFF))
    {
        return mRAM[addr];
    }

    return 0x00;
}