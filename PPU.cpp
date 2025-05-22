#include "PPU.h"

PPU::PPU()
    : mSpriteScreen(256, 240), mSpriteNameTable{{256, 240}, {256, 240}}, mSpritePatternTable{{128, 128}, {128, 128}}, mIsFrameComplete(false), mNMI(false), mScanline(0), mCycle(0), mAddressLatch(0), mDataBufferPPU(0), mAddressPPU(0)
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
        mControl.reg = data;
        break;

    case 0x0001: // mask
        mMask.reg = data;
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
        if (mAddressLatch == 0)
        {
            mAddressPPU = (mAddressPPU & 0x00FF) | (data << 8);
            mAddressLatch = 1;
        }
        else
        {
            mAddressPPU = (mAddressPPU & 0xFF00) | data;
            mAddressLatch = 0;
        }
        break;

    case 0x0007: // PPU data
        WritePPU(mAddressPPU, data);
        mAddressPPU += mControl.increment_mode ? 32 : 1;
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
        // we are only interested in the top 3 bits, the unused bits tend to be
        // filled with noise or what was last on the internal data buffer
        data = (mStatus.reg & 0xE0) | (mDataBufferPPU & 0x1F);
        // reading the status register has some side effects:
        mStatus.vertical_blank = 0;
        mAddressLatch = 0;
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
        // delayed read for almost all of the PPU address range
        data = mDataBufferPPU;
        mDataBufferPPU = ReadPPU(mAddressPPU);
        // expect for where out palettes reside
        if (mAddressPPU > 0x3F00)
        {
            data = mDataBufferPPU;
        }
        mAddressPPU++;
        break;
    }

    return data;
}

void PPU::WritePPU(uint16_t addr, uint8_t data)
{
    if (mCartridge->WritePPU(addr, data))
    {
    }
    else if ((0x0000 <= addr) && (addr <= 0x1FFF))
    {
        // pattern memory

        mPatterns[(addr & 0x1000) >> 12][addr & 0x0FFF] = data;
    }
    else if ((0x2000 <= addr) && (addr <= 0x3EFF))
    {
        // name table

        addr &= 0x0FFF;

        switch (mCartridge->mMirror)
        {
        case Cartridge::MIRRORING::VERTICAL:
            if (((0x0000 <= addr) && (addr <= 0x03FF)) || ((0x0800 <= addr) && (addr <= 0x0BFF)))
            {
                mNameTable[0][addr & 0x03FF] = data;
            }
            if (((0x0400 <= addr) && (addr <= 0x07FF)) || ((0x0C00 <= addr) && (addr <= 0x0FFF)))
            {
                mNameTable[1][addr & 0x03FF] = data;
            }
            break;

        case Cartridge::MIRRORING::HORIZONTAL:
            if (((0x0000 <= addr) && (addr <= 0x03FF)) || ((0x0400 <= addr) && (addr <= 0x07FF)))
            {
                mNameTable[0][addr & 0x03FF] = data;
            }
            if (((0x0800 <= addr) && (addr <= 0x0BFF)) || ((0x0C00 <= addr) && (addr <= 0x0FFF)))
            {
                mNameTable[1][addr & 0x03FF] = data;
            }
            break;
        }
    }
    else if ((0x3F00 <= addr) && (addr <= 0x3FFF))
    {
        // palette memory

        addr &= 0x001F;

        // mirroring
        if (addr == 0x0010)
            addr = 0x0000;
        if (addr == 0x0014)
            addr = 0x0004;
        if (addr == 0x0018)
            addr = 0x0008;
        if (addr == 0x001C)
            addr = 0x000C;

        mPalettes[addr] = data;
    }
}

uint8_t PPU::ReadPPU(uint16_t addr, bool bReadOnly) const
{
    uint8_t data = 0;

    if (mCartridge->ReadPPU(addr, data))
    {
    }
    else if ((0x0000 <= addr) && (addr <= 0x1FFF))
    {
        // pattern memory

        data = mPatterns[(addr & 0x1000) >> 12][addr & 0x0FFF];
    }
    else if ((0x2000 <= addr) && (addr <= 0x3EFF))
    {
        // name table

        addr &= 0x0FFF;

        switch (mCartridge->mMirror)
        {
        case Cartridge::MIRRORING::VERTICAL:
            if (((0x0000 <= addr) && (addr <= 0x03FF)) || ((0x0800 <= addr) && (addr <= 0x0BFF)))
            {
                data = mNameTable[0][addr & 0x03FF];
            }
            if (((0x0400 <= addr) && (addr <= 0x07FF)) || ((0x0C00 <= addr) && (addr <= 0x0FFF)))
            {
                data = mNameTable[1][addr & 0x03FF];
            }
            break;

        case Cartridge::MIRRORING::HORIZONTAL:
            if (((0x0000 <= addr) && (addr <= 0x03FF)) || ((0x0400 <= addr) && (addr <= 0x07FF)))
            {
                data = mNameTable[0][addr & 0x03FF];
            }
            if (((0x0800 <= addr) && (addr <= 0x0BFF)) || ((0x0C00 <= addr) && (addr <= 0x0FFF)))
            {
                data = mNameTable[1][addr & 0x03FF];
            }
            break;
        }
    }
    else if ((0x3F00 <= addr) && (addr <= 0x3FFF))
    {
        // palette memory

        addr &= 0x001F;

        // mirroring
        if (addr == 0x0010)
            addr = 0x0000;
        if (addr == 0x0014)
            addr = 0x0004;
        if (addr == 0x0018)
            addr = 0x0008;
        if (addr == 0x001C)
            addr = 0x000C;

        data = mPalettes[addr];
    }

    return data;
}

void PPU::ConnectCartridge(const std::shared_ptr<Cartridge> &cartridge)
{
    mCartridge = cartridge;
}

void PPU::Clock()
{
    if ((mScanline == -1) && (mCycle == 1))
    {
        mStatus.vertical_blank = 0;
    }

    if ((mScanline == 241) && (mCycle == 1))
    {
        mStatus.vertical_blank = 1;

        if (mControl.enable_nmi)
        {
            mNMI = true;
        }
    }

    // // temporary noise
    // mSpriteScreen.SetPixel(mCycle - 1, mScanline, mPaletteScreen[(rand() % 2) ? 0x3F : 0x30]);

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

olc::Sprite &PPU::GetPatternTable(uint8_t i, uint8_t palette)
{
    // for each pattern table, there are 16x16 tiles

    for (uint16_t y = 0; y < 16; ++y)
    {
        for (uint16_t x = 0; x < 16; ++x)
        {
            // each tile is 8x8 pixels, each pixel is 2 bits
            // therefore each tile is 16 bytes
            uint16_t offset = y * 256 + x * 16;

            for (uint16_t row = 0; row < 8; ++row)
            {
                // each pattern table is 4 KB
                // each tile row is 1 byte
                uint8_t tile_lsb = ReadPPU(i * 0x1000 + offset + row + 0);
                uint8_t tile_msb = ReadPPU(i * 0x1000 + offset + row + 8);

                for (uint16_t col = 0; col < 8; ++col)
                {
                    uint8_t pixel = (tile_lsb & 0x01) + (tile_msb & 0x01);

                    mSpritePatternTable[i].SetPixel(x * 8 + (7 - col),
                                                    y * 8 + row,
                                                    GetColourFromPaletteRAM(palette, pixel));

                    tile_lsb >>= 1;
                    tile_msb >>= 1;
                }
            }
        }
    }

    return mSpritePatternTable[i];
}

const olc::Pixel &PPU::GetColourFromPaletteRAM(uint8_t palette, uint8_t pixel) const
{
    return mPaletteScreen[ReadPPU(0x3F00 + (palette * 4) + pixel)];
}
