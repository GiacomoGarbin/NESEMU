#include "PPU.h"

PPU::PPU()
    : mSpriteScreen(256, 240), mSpriteNameTable{{256, 240}, {256, 240}}, mSpritePatternTable{{128, 128}, {128, 128}}, mIsFrameComplete(false), mScanline(0), mCycle(0)
{
    // https://www.nesdev.org/wiki/PPU_palettes#2C02

    mPaletteScreen[0x00] = olc::Pixel(84, 84, 84);
    mPaletteScreen[0x01] = olc::Pixel(0, 30, 116);
    mPaletteScreen[0x02] = olc::Pixel(8, 16, 144);
    mPaletteScreen[0x03] = olc::Pixel(48, 0, 136);
    mPaletteScreen[0x04] = olc::Pixel(68, 0, 100);
    mPaletteScreen[0x05] = olc::Pixel(92, 0, 48);
    mPaletteScreen[0x06] = olc::Pixel(84, 4, 0);
    mPaletteScreen[0x07] = olc::Pixel(60, 24, 0);
    mPaletteScreen[0x08] = olc::Pixel(32, 42, 0);
    mPaletteScreen[0x09] = olc::Pixel(8, 58, 0);
    mPaletteScreen[0x0A] = olc::Pixel(0, 64, 0);
    mPaletteScreen[0x0B] = olc::Pixel(0, 60, 0);
    mPaletteScreen[0x0C] = olc::Pixel(0, 50, 60);
    mPaletteScreen[0x0D] = olc::Pixel(0, 0, 0);
    mPaletteScreen[0x0E] = olc::Pixel(0, 0, 0);
    mPaletteScreen[0x0F] = olc::Pixel(0, 0, 0);

    mPaletteScreen[0x10] = olc::Pixel(152, 150, 152);
    mPaletteScreen[0x11] = olc::Pixel(8, 76, 196);
    mPaletteScreen[0x12] = olc::Pixel(48, 50, 236);
    mPaletteScreen[0x13] = olc::Pixel(92, 30, 228);
    mPaletteScreen[0x14] = olc::Pixel(136, 20, 176);
    mPaletteScreen[0x15] = olc::Pixel(160, 20, 100);
    mPaletteScreen[0x16] = olc::Pixel(152, 34, 32);
    mPaletteScreen[0x17] = olc::Pixel(120, 60, 0);
    mPaletteScreen[0x18] = olc::Pixel(84, 90, 0);
    mPaletteScreen[0x19] = olc::Pixel(40, 114, 0);
    mPaletteScreen[0x1A] = olc::Pixel(8, 124, 0);
    mPaletteScreen[0x1B] = olc::Pixel(0, 118, 40);
    mPaletteScreen[0x1C] = olc::Pixel(0, 102, 120);
    mPaletteScreen[0x1D] = olc::Pixel(0, 0, 0);
    mPaletteScreen[0x1E] = olc::Pixel(0, 0, 0);
    mPaletteScreen[0x1F] = olc::Pixel(0, 0, 0);

    mPaletteScreen[0x20] = olc::Pixel(236, 238, 236);
    mPaletteScreen[0x21] = olc::Pixel(76, 154, 236);
    mPaletteScreen[0x22] = olc::Pixel(120, 124, 236);
    mPaletteScreen[0x23] = olc::Pixel(176, 98, 236);
    mPaletteScreen[0x24] = olc::Pixel(228, 84, 236);
    mPaletteScreen[0x25] = olc::Pixel(236, 88, 180);
    mPaletteScreen[0x26] = olc::Pixel(236, 106, 100);
    mPaletteScreen[0x27] = olc::Pixel(212, 136, 32);
    mPaletteScreen[0x28] = olc::Pixel(160, 170, 0);
    mPaletteScreen[0x29] = olc::Pixel(116, 196, 0);
    mPaletteScreen[0x2A] = olc::Pixel(76, 208, 32);
    mPaletteScreen[0x2B] = olc::Pixel(56, 204, 108);
    mPaletteScreen[0x2C] = olc::Pixel(56, 180, 204);
    mPaletteScreen[0x2D] = olc::Pixel(60, 60, 60);
    mPaletteScreen[0x2E] = olc::Pixel(0, 0, 0);
    mPaletteScreen[0x2F] = olc::Pixel(0, 0, 0);

    mPaletteScreen[0x30] = olc::Pixel(236, 238, 236);
    mPaletteScreen[0x31] = olc::Pixel(168, 204, 236);
    mPaletteScreen[0x32] = olc::Pixel(188, 188, 236);
    mPaletteScreen[0x33] = olc::Pixel(212, 178, 236);
    mPaletteScreen[0x34] = olc::Pixel(236, 174, 236);
    mPaletteScreen[0x35] = olc::Pixel(236, 174, 212);
    mPaletteScreen[0x36] = olc::Pixel(236, 180, 176);
    mPaletteScreen[0x37] = olc::Pixel(228, 196, 144);
    mPaletteScreen[0x38] = olc::Pixel(204, 210, 120);
    mPaletteScreen[0x39] = olc::Pixel(180, 222, 120);
    mPaletteScreen[0x3A] = olc::Pixel(168, 226, 144);
    mPaletteScreen[0x3B] = olc::Pixel(152, 226, 180);
    mPaletteScreen[0x3C] = olc::Pixel(160, 214, 228);
    mPaletteScreen[0x3D] = olc::Pixel(160, 162, 160);
    mPaletteScreen[0x3E] = olc::Pixel(0, 0, 0);
    mPaletteScreen[0x3F] = olc::Pixel(0, 0, 0);
}

PPU::~PPU()
{
}

void PPU::WriteCPU(uint16_t addr, uint8_t data)
{
    switch (addr)
    {
    case 0x0000: // control
        break;
    case 0x0001: // mask
        break;
    case 0x0002: // status
        break;
    case 0x0003: // OAM addr
        break;
    case 0x0004: // OAM data
        break;
    case 0x0005: // scroll
        break;
    case 0x0006: // PPU addr
        break;
    case 0x0007: // PPU data
        break;
    }
}

uint8_t PPU::ReadCPU(uint16_t addr, bool bReadOnly)
{
    uint8_t data = 0x00;

    switch (addr)
    {
    case 0x0000: // control
        break;
    case 0x0001: // mask
        break;
    case 0x0002: // status
        break;
    case 0x0003: // OAM addr
        break;
    case 0x0004: // OAM data
        break;
    case 0x0005: // scroll
        break;
    case 0x0006: // PPU addr
        break;
    case 0x0007: // PPU data
        break;
    }

    return data;
}

void PPU::WritePPU(uint16_t addr, uint8_t data)
{
    if (mCartridge->WritePPU(addr, data))
    {
    }
}

uint8_t PPU::ReadPPU(uint16_t addr, bool bReadOnly)
{
    uint8_t data = 0;

    if (mCartridge->ReadPPU(addr, data))
    {
    }

    return data;
}

void PPU::ConnectCartridge(const std::shared_ptr<Cartridge> &cartridge)
{
    mCartridge = cartridge;
}

void PPU::Clock()
{
    mSpriteScreen.SetPixel(mCycle - 1, mScanline, mPaletteScreen[(rand() % 2) ? 0x3F : 0x30]);

    mCycle++;

    if (mCycle >= 314)
    {
        mCycle = 0;
        mScanline++;

        if (mScanline >= 261)
        {
            mScanline = -1;
            mIsFrameComplete = true;
        }
    }
}
