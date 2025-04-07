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

void CPU::Clock()
{
    if (mCycles == 0)
    {
        mOpcode = Read(mPC);
        mPC++;

        // get starting number of cycles
        mCycles = mLookup[mOpcode].cycles;

        uint8_t additional_cycle_1 = (this->*mLookup[mOpcode].addrmode)();
        uint8_t additional_cycle_2 = (this->*mLookup[mOpcode].operate)();

        mCycles += (additional_cycle_1 & additional_cycle_2);
    }

    mCycles--;
}

// Addressing Modes

uint8_t CPU::IMP()
{
    // there is no data as part of the instruction
    // however it can operate upon the accumulator, so we set mFetch to the content of the A register

    mFetch = mA;
    return 0;
}

uint8_t CPU::IMM()
{
    // data is supplied as part of the instruction (the next byte)

    mAddrABS = mPC++;
    return 0;
}

uint8_t CPU::ZP0()
{
    // the byte of data we are interesting in reading for this instruction
    // can be found somewhere in page zero (i.e. the high-byte is zero)
    
    mAddrABS = Read(mPC);
    mPC++;
    mAddrABS &= 0x00FF;
    return 0;
}

uint8_t CPU::ZPX()
{
    // the address supplied with the instruction had the content of the X register added to it
    // useful for iterating regions of memory, such as arrays

    mAddrABS = (Read(mPC) + mX);
    mPC++;
    mAddrABS &= 0x00FF;
    return 0;
}

uint8_t CPU::ZPY()
{
    // the address supplied with the instruction had the content of the Y register added to it
    // useful for iterating regions of memory, such as arrays

    mAddrABS = (Read(mPC) + mX);
    mPC++;
    mAddrABS &= 0x00FF;
    return 0;
}

uint8_t CPU::ABS()
{
    // the data supplied with the instruction consists of the low byte and high byte of the 16-byte address

    uint16_t lo = Read(mPC);
    mPC++;
    uint16_t hi = Read(mPC);
    mPC++;

    mAddrABS = ((hi << 8) | lo);
    
    return 0;
}

uint8_t CPU::ABX()
{
    // the data supplied with the instruction consists of the low byte and high byte of the 16-byte address
    // the address supplied with the instruction had the content of the X register added to it

    uint16_t lo = Read(mPC);
    mPC++;
    uint16_t hi = Read(mPC);
    mPC++;
    
    mAddrABS = ((hi << 8) | lo);
    mAddrABS += mX;

    // if after incrementing with the X register, the whole address has changed to a different page,
    // wa may need an additional clock cycle

    if ((mAddrABS >> 8) != hi)
    {
        return 1;
    }

    return 0;
}

uint8_t CPU::ABY()
{
    // the data supplied with the instruction consists of the low byte and high byte of the 16-byte address
    // the address supplied with the instruction had the content of the Y register added to it

    uint16_t lo = Read(mPC);
    mPC++;
    uint16_t hi = Read(mPC);
    mPC++;
    
    mAddrABS = ((hi << 8) | lo);
    mAddrABS += mY;

    // if after incrementing with the Y register, the whole address has changed to a different page,
    // wa may need an additional clock cycle

    if ((mAddrABS >> 8) != hi)
    {
        return 1;
    }

    return 0;
}

uint8_t CPU::IND()
{
    // the data supplied with the instruction consists of the low byte and high byte of the 16-byte address
    // the address supplied with the instruction is a pointer, we interrogate that location to get the actual address
    // which is where the data we actually want resides

    uint16_t lo = Read(mPC);
    mPC++;
    uint16_t hi = Read(mPC);
    mPC++;
    
    uint16_t ptr = ((hi << 8) | lo);

    if (lo == 0x00FF)
    {
        // simulate page boundary hardware bug
        mAddrABS = ((Read(ptr & 0xFF00) << 8) | Read(ptr + 0));
    }
    else
    {
        mAddrABS = ((Read(ptr + 1) << 8) | Read(ptr + 0));
    }

    return 0;
}

uint8_t CPU::IZX()
{
    // indirect addressing mode of the zero page with X register offset
    // the supplied address references somewhere in the zero page
    // we offset that 1-byte address by the contents of the X register
    // to read the 2-byte actual address we need for the instruction

    uint16_t zp = Read(mPC);
    mPC++;

    uint16_t lo = Read((zp + uint16_t(mX) + 0) & 0x00FF);
    uint16_t hi = Read((zp + uint16_t(mX) + 1) & 0x00FF);
    
    mAddrABS = ((hi << 8) | lo);

    return 0;
}

uint8_t CPU::IZY()
{
    // we read 1-byte which is an offset in the zero page
    // we read the 2-byte actual address from that location
    // and we offset that address by the contents of the Y register

    uint16_t zp = Read(mPC);
    mPC++;

    uint16_t lo = Read((zp + 0) & 0x00FF);
    uint16_t hi = Read((zp + 1) & 0x00FF);
    
    mAddrABS = ((hi << 8) | lo);
    mAddrABS += mY;

    // if after incrementing with the Y register, the whole address has changed to a different page,
    // wa may need an additional clock cycle

    if ((mAddrABS >> 8) != hi)
    {
        return 1;
    }

    return 0;
}

uint8_t CPU::REL()
{
    // relative addressing mode, it only applies to braching instructions
    // branch instructions can't jump further away than 127 memory locations

    mAddrREL = Read(mPC);
    mPC++;

    if (mAddrREL & 0x0080)
    {
        // jumping backwards, adjust the address for binary arithmetic
        mAddrREL |= 0xFF00;
    }

    return 0;
}