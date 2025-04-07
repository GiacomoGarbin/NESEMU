#pragma once

#include <cstdint>
#include <string>
#include <vector>

class Bus;

class CPU
{
public:
    CPU();
    ~CPU();

    enum FLAGS
    {
        C = (1 << 0), // carry bit
        Z = (1 << 1), // zero
        I = (1 << 2), // disable interrupts
        D = (1 << 3), // decimal mode
        B = (1 << 4), // break
        U = (1 << 5), // unused
        O = (1 << 6), // overflow
        N = (1 << 7), // negative
    };

    // registers

    uint8_t  mA;  // A register
    uint8_t  mX;  // X register
    uint8_t  mY;  // Y register
    uint8_t  mSP; // stack pointer
    uint16_t mPC; // program counter
    uint8_t  mSR; // status register

    void ConnectBus(Bus* pBus);

    // addressing modes (12)

    uint8_t ABS(); // absolute address
    uint8_t ABX(); // absolute address, X register offset
    uint8_t ABY(); // absolute address, Y register offset
    uint8_t IMM(); // immediate data source
    uint8_t IMP(); // implied data source
    uint8_t IND(); // indirect (aka pointer)
    uint8_t IZX(); // indirect zero page, X register offset
    uint8_t IZY(); // indirect zero page, Y register offset
    uint8_t REL(); // relative
    uint8_t ZP0(); // zero page
    uint8_t ZPX(); // zero page, X register offset
    uint8_t ZPY(); // zero page, Y register offset

    // opcodes (56)

    // TODO: declare opcode methods

    uint8_t XXX(); // illegal opcode fallback

    // signals

    void Clock();
    void Reset();
    void IRQ();   // interrupt request
    void NMI();   // non-maskable interrupt request

    uint8_t Fetch();
    uint8_t mFetch;

    uint16_t mAddrABS; // absolute address
    uint16_t mAddrREL; // relative address
    uint8_t  mOpcode;  // current opcode
    uint8_t  mCycles;  // number of cycles left for the duration of the current instruction

private:
    Bus* mBus;

    // methods to access the BUS
    void Write(uint16_t addr, uint8_t data);
    uint8_t Read(uint16_t addr);

    // methods to access the status register
    uint8_t GetFlag(FLAGS flag);
    void SetFlag(FLAGS flag, bool enable);

    struct Instruction
    {
        std::string name;
        uint8_t (CPU::* operate)(void) = nullptr;
        uint8_t (CPU::* addrmode)(void) = nullptr;
        uint8_t cycles = 0;
    };

    std::vector<Instruction> mLookup;
};