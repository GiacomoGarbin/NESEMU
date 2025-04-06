#pragma once

#include <cstdint>

class Bus;

class CPU
{
public:
    CPU();
    ~CPU();

    void ConnectBus(Bus* pBus);

private:
    Bus* mBus;

    void Write(uint16_t addr, uint8_t data);
    uint8_t Read(uint16_t addr);
};