#pragma once

#include "Mapper000.h"

#include <cstdint>
#include <string>
#include <memory>
#include <vector>

class Cartridge
{
public:
    Cartridge(const std::string &filename);
    ~Cartridge();

    // communication with CPU bus
    bool WriteCPU(uint16_t addr, uint8_t data);
    bool ReadCPU(uint16_t addr, uint8_t &data);
    // communication with PPU bus
    bool WritePPU(uint16_t addr, uint8_t data);
    bool ReadPPU(uint16_t addr, uint8_t &data);

private:
    // memory
    std::vector<uint8_t> mPRG;
    std::vector<uint8_t> mCHR;

    uint8_t mMapperID;
    uint8_t mBanksPRG;
    uint8_t mBanksCHR;

    std::shared_ptr<Mapper> mMapper;
};