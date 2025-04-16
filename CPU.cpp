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

// Instructions

uint8_t CPU::Fetch()
{
    // we want to fetch data for all instructions but those that use implied address mode

    if (mLookup[mOpcode].addrmode != &CPU::IMP)
    {
        mFetch = Read(mAddrABS);
    }
    return mFetch;
}

uint8_t CPU::AND()
{
    // bitwise logic AND between the accumulator register and the fetched data

    Fetch();
    mA &= mFetch;
    SetFlag(FLAGS::Z, mA == 0x00);
    SetFlag(FLAGS::N, mA & 0x80);
    return 1; // can potentially require an additional cycle if page boundary is crossed
}

uint8_t CPU::BCS()
{
    return BranchIf(GetFlag(FLAGS::C) != 0x00);
}

uint8_t CPU::BCC()
{
    return BranchIf(GetFlag(FLAGS::C) == 0x00);
}

uint8_t CPU::BEQ()
{
    return BranchIf(GetFlag(FLAGS::Z) != 0x00);
}

uint8_t CPU::BMI()
{
    return BranchIf(GetFlag(FLAGS::N) != 0x00);
}

uint8_t CPU::BNE()
{
    return BranchIf(GetFlag(FLAGS::Z) == 0x00);
}

uint8_t CPU::BPL()
{
    return BranchIf(GetFlag(FLAGS::N) == 0x00);
}

uint8_t CPU::BVC()
{
    return BranchIf(GetFlag(FLAGS::O) == 0x00);
}

uint8_t CPU::BVS()
{
    return BranchIf(GetFlag(FLAGS::O) != 0x00);
}

uint8_t CPU::CLC()
{
    SetFlag(FLAGS::C, false);
    return 0;
}

uint8_t CPU::CLD()
{
    SetFlag(FLAGS::D, false);
    return 0;
}

uint8_t CPU::ADC()
{
    return AddValue(uint16_t(mFetch));
}

uint8_t CPU::SBC()
{
    return AddValue(~uint16_t(mFetch));
}

uint8_t CPU::PHA()
{
    PushValue(mA);
    return 0;
}

uint8_t CPU::PLA()
{
    mA = PopValue();
    SetFlag(FLAGS::Z, mA == 0x00);
    SetFlag(FLAGS::N, (mA & 0x80) != 0x00);
    return 0;
}

uint8_t CPU::RTI()
{
    mSR = PopValue();
    mSR &= ~FLAGS::B;

    mPC = uint16_t(PopValue());
    mPC |= uint16_t(PopValue()) << 8;

    return 0;
}

uint8_t CPU::XXX()
{
    // TODO: should we do something?
}

// Signals

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

void CPU::Reset()
{
    mA = 0x00;
    mX = 0x00;
    mY = 0x00;
    mSP = 0xFD;
    mSR = 0x00;

    JumpAddrABS(0xFFFC);

    mAddrABS = 0x0000;
    mAddrREL = 0x0000;
    mFetch = 0x00;

    mCycles = 8;
}

void CPU::IRQ()
{
    if (GetFlag(FLAGS::I) == 0x00)
    {
        CallInterrupt(0xFFFE);
    }
}

void CPU::NMI()
{
    CallInterrupt(0xFFFA);
}

// Helpers

uint8_t CPU::BranchIf(const bool cond)
{
    if (cond)
    {
        mCycles++;
        mAddrABS = mPC + mAddrREL;

        if ((mAddrABS & 0xFF00) != (mPC & 0xFF00))
        {
            // add a second clock cycle if the branch needs to cross a page boundary
            mCycles++;
        }

        mPC = mAddrABS;
    }
    return 0;
}

uint8_t CPU::AddValue(const uint16_t value)
{
    Fetch();
    uint16_t temp = uint16_t(mA) + value + uint16_t(GetFlag(FLAGS::C));
    SetFlag(FLAGS::C, (temp & 0x0100) != 0x0000);
    SetFlag(FLAGS::Z, (temp & 0x00FF) == 0x0000);
    SetFlag(FLAGS::N, (temp & 0x0080) != 0x0000);
    SetFlag(FLAGS::O, ((~(uint16_t(mA) ^ uint16_t(mFetch)) & (uint16_t(mA) ^ temp)) & 0x0080) != 0x0000);
    mA = uint8_t(temp & 0x00FF);
    return 1;
}

void CPU::JumpAddrABS(const uint16_t addr)
{
    mAddrABS = addr;
    uint16_t lo = Read(mAddrABS + 0);
    uint16_t hi = Read(mAddrABS + 1);
    mPC = (hi << 8) | lo;
}

void CPU::PushValue(const uint8_t value)
{
    Write(0x0100 + uint16_t(mSP), value);
    mSP--;
}

uint8_t CPU::PopValue()
{
    mSP++;
    return Read(0x0100 + uint16_t(mSP));
}

void CPU::CallInterrupt(const uint16_t routine)
{
    PushValue((mPC >> 8) & 0xFF);
    PushValue(mPC & 0xFF);

    SetFlag(FLAGS::B, false);
    SetFlag(FLAGS::I, true);
    PushValue(mSR);

    JumpAddrABS(routine);

    mCycles = 7;
}