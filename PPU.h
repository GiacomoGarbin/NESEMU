#pragma once

#include "Cartridge.h"

#include <cstdint>
#include <memory>

#include "PGE/olcPixelGameEngine.h"

class PPU
{
public:
    PPU();
    ~PPU();

    // communication with CPU bus
    void WriteCPU(uint16_t addr, uint8_t data);
    uint8_t ReadCPU(uint16_t addr, bool bReadOnly = false);

    // communication with PPU bus
    void WritePPU(uint16_t addr, uint8_t data);
    uint8_t ReadPPU(uint16_t addr, bool bReadOnly = false);

    void ConnectCartridge(const std::shared_ptr<Cartridge> &cartridge);
    void Clock();

private:
    // devices connected to the PPU bus
    std::shared_ptr<Cartridge> mCartridge;
    // memory
    uint8_t mNameTable[2][1024];
    uint8_t mPalettes[32];

    olc::Pixel mPaletteScreen[0x40];
    olc::Sprite mSpriteScreen;
    olc::Sprite mSpriteNameTable[2];
    olc::Sprite mSpritePatternTable[2];

public:
    olc::Sprite &GetScreen() { return mSpriteScreen; }
    olc::Sprite &GetNameTable(uint8_t i) { return mSpriteNameTable[i]; }
    olc::Sprite &GetPatternTable(uint8_t i) { return mSpritePatternTable[i]; }
    bool mIsFrameComplete;

private:
    int16_t mScanline;
    int16_t mCycle;
};