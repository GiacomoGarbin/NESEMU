#include "CPU.h"

#include "Bus.h"

CPU::CPU()
    : mBus(nullptr)
{

}

CPU::~CPU()
{

}

void CPU::ConnectBus(Bus* pBus)
{
    mBus = pBus;
}

void CPU::Write(uint16_t addr, uint8_t data)
{
    mBus->Write(addr, data);
}

uint8_t CPU::Read(uint16_t addr)
{
    return mBus->Read(addr, false);
}