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
    uint8_t ReadPPU(uint16_t addr, bool bReadOnly = false) const;

    void ConnectCartridge(const std::shared_ptr<Cartridge> &cartridge);
    void Clock();

private:
    // devices connected to the PPU bus
    std::shared_ptr<Cartridge> mCartridge;
    // memory
public:
    uint8_t mNameTable[2][1024];

private:
    uint8_t mPalettes[32];
    uint8_t mPatterns[2][4096];

    olc::Pixel mPaletteScreen[0x40];
    olc::Sprite mSpriteScreen;
    olc::Sprite mSpriteNameTable[2];
    olc::Sprite mSpritePatternTable[2];

public:
    olc::Sprite &GetScreen() { return mSpriteScreen; }
    olc::Sprite &GetNameTable(uint8_t i) { return mSpriteNameTable[i]; }
    olc::Sprite &GetPatternTable(uint8_t i, uint8_t palette);
    const olc::Pixel &GetColourFromPaletteRAM(uint8_t palette, uint8_t pixel) const;

    bool mIsFrameComplete;
    bool mNMI;

private:
    int16_t mScanline;
    int16_t mCycle;

    // registers

    union
    {
        struct
        {
            uint8_t unused : 5;
            uint8_t sprite_overflow : 1;
            uint8_t sprite_zero_hit : 1;
            uint8_t vertical_blank : 1;
        };
        uint8_t reg;
    } mStatus;

    union
    {
        struct
        {
            uint8_t grayscale : 1;
            uint8_t render_background_left : 1;
            uint8_t render_sprites_left : 1;
            uint8_t render_background : 1;
            uint8_t render_sprites : 1;
            uint8_t enhance_red : 1;
            uint8_t enhance_green : 1;
            uint8_t enhance_blue : 1;
        };
        uint8_t reg;
    } mMask;

    union
    {
        struct
        {
            uint8_t nametable_x : 1;
            uint8_t nametable_y : 1;
            uint8_t increment_mode : 1;
            uint8_t pattern_sprite : 1;
            uint8_t pattern_background : 1;
            uint8_t sprite_size : 1;
            uint8_t slave_mode : 1; // unused
            uint8_t enable_nmi : 1;
        };
        uint8_t reg;
    } mControl;

    uint8_t mAddressLatch; // are we writing to the low or high byte?
    uint8_t mDataBufferPPU;

    union LoopyRegister
    {
        struct
        {
            uint16_t coarse_x : 5;
            uint16_t coarse_y : 5;
            uint16_t nametable_x : 1;
            uint16_t nametable_y : 1;
            uint16_t fine_y : 5;
            uint16_t unused_x : 1;
        };
        uint16_t reg;
    };

    LoopyRegister mVramAddr;
    LoopyRegister mTramAddr;

    uint8_t mFineX;

    uint8_t mBackgroundNextTileID;
    uint8_t mBackgroundNextTileAttrib;
    uint8_t mBackgroundNextTileLSB;
    uint8_t mBackgroundNextTileMSB;

    uint16_t mBackgroundShifterPatternLo;
    uint16_t mBackgroundShifterPatternHi;
    uint16_t mBackgroundShifterAttribLo;
    uint16_t mBackgroundShifterAttribHi;

private:
    struct ObjectAttributeEntry
    {
        // the order of the structure members is what the NES expects
        uint8_t y;         // y position of the sprite
        uint8_t id;        // ID of the tile from pattern memory
        uint8_t attribute; // flags defining how the sprite should be rendered
        uint8_t x;         // x position of the sprite
    };

    ObjectAttributeEntry mOAM[64];

public:
    uint8_t *mPtrOAM;

private:
    uint8_t mAddrOAM;
};