#include "Bus.h"

Bus::Bus()
{
    // clear the RAM

    for (uint8_t &byte : mCPURAM)
    {
        byte = 0x00;
    }

    // connect the CPU to this BUS
    mCPU.ConnectBus(this);
}

Bus::~Bus()
{
}

void Bus::WriteCPU(uint16_t addr, uint8_t data)
{
    if (mCartridge->WriteCPU(addr, data))
    {
    }
    else if ((0x0000 <= addr) && (addr <= 0x1FFF))
    {
        mCPURAM[addr & 0x07FF] = data;
    }
    else if ((0x2000 <= addr) && (addr <= 0x3FFF))
    {
        mPPU.WriteCPU(addr & 0x0007, data);
    }
}

uint8_t Bus::ReadCPU(uint16_t addr, bool bReadOnly)
{
    uint8_t data = 0x00;

    if (mCartridge->ReadCPU(addr, data))
    {
    }
    else if ((0x0000 <= addr) && (addr <= 0x1FFF))
    {
        return mCPURAM[addr & 0x07FF];
    }
    else if ((0x2000 <= addr) && (addr <= 0x3FFF))
    {
        return mPPU.ReadCPU(addr & 0x0007, bReadOnly);
    }

    return data;
}

void Bus::InsertCartridge(const std::shared_ptr<Cartridge> &cartridge)
{
    mCartridge = cartridge;
    mPPU.ConnectCartridge(cartridge);
}

void Bus::Reset()
{
    mCPU.Reset();
    mSystemClockCounter = 0;
}

void Bus::Clock()
{
    mPPU.Clock();

    if ((mSystemClockCounter % 3) == 0)
    {
        mCPU.Clock();
    }

    if (mPPU.mNMI)
    {
        mPPU.mNMI = false;
        mCPU.NMI();
    }

    mSystemClockCounter++;
}