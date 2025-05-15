#pragma once

#include "CPU.h"
#include "PPU.h"
#include "Cartridge.h"

#include <array>
#include <cstdint>
#include <memory>

class Bus
{
public:
    Bus();
    ~Bus();

    using CPURAM = std::array<uint8_t, 2 * 1024>;

    // devices connected to the bus
    CPU mCPU;
    CPURAM mCPURAM;
    PPU mPPU;
    std::shared_ptr<Cartridge> mCartridge;

    void WriteCPU(uint16_t addr, uint8_t data);
    uint8_t ReadCPU(uint16_t addr, bool bReadOnly = false);

    void InsertCartridge(const std::shared_ptr<Cartridge>& cartridge);
    void Reset();
    void Clock(); // perform one system tick of the emulation

private:
    uint32_t mSystemClockCounter = 0;
};