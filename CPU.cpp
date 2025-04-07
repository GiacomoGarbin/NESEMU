#include "CPU.h"

#include "Bus.h"

CPU::CPU()
    : mBus(nullptr)
    , mA(0x00)
    , mX(0x00)
    , mY(0x00)
    , mSP(0x00)
    , mPC(0x0000)
    , mSR(0x00)
    , mFetch(0x00)
    , mAddrABS(0x0000)
    , mAddrREL(0x0000)
    , mOpcode(0x00)
    , mCycles(0x00)
{
    mLookup =
    {
        // TODO: initialize instruction lookup table
    };
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

uint8_t CPU::GetFlag(FLAGS flag)
{
    return (mSR & flag);
}

void CPU::SetFlag(FLAGS flag, bool enable)
{
    if (enable)
    {
        mSR |= flag;
    }
    else
    {
        mSR &= ~flag;
    }
}