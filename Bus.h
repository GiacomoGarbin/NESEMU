#pragma once

#include "CPU.h"

#include <array>
#include <cstdint>

class Bus
{
public:
    Bus();
    ~Bus();

    using RAM = std::array<uint8_t, 64*1024>;

    // devices connected to the bus
    CPU mCPU;
    RAM mRAM;

    void Write(uint16_t addr, uint8_t data);
    uint8_t Read(uint16_t addr, bool bReadOnly = false);
};