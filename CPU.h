#pragma once

#include <cstdint>

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

private:
    Bus* mBus;

    // methods to access the BUS
    void Write(uint16_t addr, uint8_t data);
    uint8_t Read(uint16_t addr);

    // methods to access the status register
    uint8_t GetFlag(FLAGS flag);
    void SetFlag(FLAGS flag, bool enable);
};