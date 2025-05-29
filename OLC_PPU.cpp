// OLC

#include "PPU.h"

PPU::PPU()
    : mSpriteScreen(256, 240), mSpriteNameTable{{256, 240}, {256, 240}}, mSpritePatternTable{{128, 128}, {128, 128}}
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

// olc::Sprite& PPU::GetNameTable(uint8_t i)
// {
// 	// As of now unused, but a placeholder for nametable visualisation in teh future
// 	return mSpriteNameTable[i];
// }

uint8_t PPU::ReadCPU(uint16_t addr, bool rdonly)
{
    uint8_t data = 0x00;

    if (rdonly)
    {
        // Reading from PPU registers can affect their contents
        // so this read only option is used for examining the
        // state of the PPU without changing its state. This is
        // really only used in debug mode.
        switch (addr)
        {
        case 0x0000: // Control
            data = mControl.reg;
            break;
        case 0x0001: // Mask
            data = mMask.reg;
            break;
        case 0x0002: // Status
            data = mStatus.reg;
            break;
        case 0x0003: // OAM Address
            break;
        case 0x0004: // OAM Data
            break;
        case 0x0005: // Scroll
            break;
        case 0x0006: // PPU Address
            break;
        case 0x0007: // PPU Data
            break;
        }
    }
    else
    {
        // These are the live PPU registers that repsond
        // to being read from in various ways. Note that not
        // all the registers are capable of being read from
        // so they just return 0x00
        switch (addr)
        {
            // Control - Not readable
        case 0x0000:
            break;

            // Mask - Not Readable
        case 0x0001:
            break;

            // Status
        case 0x0002:
            // Reading from the mStatus register has the effect of resetting
            // different parts of the circuit. Only the top three bits
            // contain mStatus information, however it is possible that
            // some "noise" gets picked up on the bottom 5 bits which
            // represent the last PPU bus transaction. Some games "may"
            // use this noise as valid data (even though they probably
            // shouldn't)
            data = (mStatus.reg & 0xE0) | (mDataBufferPPU & 0x1F);

            // Clear the vertical blanking flag
            mStatus.vertical_blank = 0;

            // Reset Loopy's Address latch flag
            mAddressLatch = 0;
            break;

            // OAM Address
        case 0x0003:
            break;

            // OAM Data
        case 0x0004:
            break;

            // Scroll - Not Readable
        case 0x0005:
            break;

            // PPU Address - Not Readable
        case 0x0006:
            break;

            // PPU Data
        case 0x0007:
            // Reads from the NameTable ram get delayed one mCycle,
            // so output buffer which contains the data from the
            // previous read request
            data = mDataBufferPPU;
            // then update the buffer for next time
            mDataBufferPPU = ReadPPU(mVramAddr.reg);
            // However, if the address was in the palette range, the
            // data is not delayed, so it returns immediately
            if (mVramAddr.reg >= 0x3F00)
                data = mDataBufferPPU;
            // All reads from PPU data automatically increment the nametable
            // address depending upon the mode set in the mControl register.
            // If set to vertical mode, the increment is 32, so it skips
            // one whole nametable row; in horizontal mode it just increments
            // by 1, moving to the next column
            mVramAddr.reg += (mControl.increment_mode ? 32 : 1);
            break;
        }
    }

    return data;
}

void PPU::WriteCPU(uint16_t addr, uint8_t data)
{
    switch (addr)
    {
    case 0x0000: // Control
        mControl.reg = data;
        mTramAddr.nametable_x = mControl.nametable_x;
        mTramAddr.nametable_y = mControl.nametable_y;
        break;
    case 0x0001: // Mask
        mMask.reg = data;
        break;
    case 0x0002: // Status
        break;
    case 0x0003: // OAM Address
        break;
    case 0x0004: // OAM Data
        break;
    case 0x0005: // Scroll
        if (mAddressLatch == 0)
        {
            // First write to scroll register contains X offset in pixel space
            // which we split into coarse and fine x values
            mFineX = data & 0x07;
            mTramAddr.coarse_x = data >> 3;
            mAddressLatch = 1;
        }
        else
        {
            // First write to scroll register contains Y offset in pixel space
            // which we split into coarse and fine Y values
            mTramAddr.fine_y = data & 0x07;
            mTramAddr.coarse_y = data >> 3;
            mAddressLatch = 0;
        }
        break;
    case 0x0006: // PPU Address
        if (mAddressLatch == 0)
        {
            // PPU address bus can be accessed by CPU via the ADDR and DATA
            // registers. The fisrt write to this register latches the high byte
            // of the address, the second is the low byte. Note the writes
            // are stored in the tram register...
            mTramAddr.reg = (uint16_t)((data & 0x3F) << 8) | (mTramAddr.reg & 0x00FF);
            mAddressLatch = 1;
        }
        else
        {
            // ...when a whole address has been written, the internal vram address
            // buffer is updated. Writing to the PPU is unwise during rendering
            // as the PPU will maintam the vram address automatically whilst
            // rendering the mScanline position.
            mTramAddr.reg = (mTramAddr.reg & 0xFF00) | data;
            mVramAddr = mTramAddr;
            mAddressLatch = 0;
        }
        break;
    case 0x0007: // PPU Data
        WritePPU(mVramAddr.reg, data);
        // All writes from PPU data automatically increment the nametable
        // address depending upon the mode set in the mControl register.
        // If set to vertical mode, the increment is 32, so it skips
        // one whole nametable row; in horizontal mode it just increments
        // by 1, moving to the next column
        mVramAddr.reg += (mControl.increment_mode ? 32 : 1);
        break;
    }
}

uint8_t PPU::ReadPPU(uint16_t addr, bool rdonly) const
{
    uint8_t data = 0x00;
    addr &= 0x3FFF;

    if (mCartridge->ReadPPU(addr, data))
    {
    }
    else if (addr >= 0x0000 && addr <= 0x1FFF)
    {
        // If the cartridge cant map the address, have
        // a physical location ready here
        data = mPatterns[(addr & 0x1000) >> 12][addr & 0x0FFF];
    }
    else if (addr >= 0x2000 && addr <= 0x3EFF)
    {
        addr &= 0x0FFF;

        if (mCartridge->mMirror == Cartridge::MIRRORING::VERTICAL)
        {
            // Vertical
            if (addr >= 0x0000 && addr <= 0x03FF)
                data = mNameTable[0][addr & 0x03FF];
            if (addr >= 0x0400 && addr <= 0x07FF)
                data = mNameTable[1][addr & 0x03FF];
            if (addr >= 0x0800 && addr <= 0x0BFF)
                data = mNameTable[0][addr & 0x03FF];
            if (addr >= 0x0C00 && addr <= 0x0FFF)
                data = mNameTable[1][addr & 0x03FF];
        }
        else if (mCartridge->mMirror == Cartridge::MIRRORING::HORIZONTAL)
        {
            // Horizontal
            if (addr >= 0x0000 && addr <= 0x03FF)
                data = mNameTable[0][addr & 0x03FF];
            if (addr >= 0x0400 && addr <= 0x07FF)
                data = mNameTable[0][addr & 0x03FF];
            if (addr >= 0x0800 && addr <= 0x0BFF)
                data = mNameTable[1][addr & 0x03FF];
            if (addr >= 0x0C00 && addr <= 0x0FFF)
                data = mNameTable[1][addr & 0x03FF];
        }
    }
    else if (addr >= 0x3F00 && addr <= 0x3FFF)
    {
        addr &= 0x001F;
        if (addr == 0x0010)
            addr = 0x0000;
        if (addr == 0x0014)
            addr = 0x0004;
        if (addr == 0x0018)
            addr = 0x0008;
        if (addr == 0x001C)
            addr = 0x000C;
        data = mPalettes[addr] & (mMask.grayscale ? 0x30 : 0x3F);
    }

    return data;
}

void PPU::WritePPU(uint16_t addr, uint8_t data)
{
    addr &= 0x3FFF;

    if (mCartridge->WritePPU(addr, data))
    {
    }
    else if (addr >= 0x0000 && addr <= 0x1FFF)
    {
        mPatterns[(addr & 0x1000) >> 12][addr & 0x0FFF] = data;
    }
    else if (addr >= 0x2000 && addr <= 0x3EFF)
    {
        addr &= 0x0FFF;
        if (mCartridge->mMirror == Cartridge::MIRRORING::VERTICAL)
        {
            // Vertical
            if (addr >= 0x0000 && addr <= 0x03FF)
                mNameTable[0][addr & 0x03FF] = data;
            if (addr >= 0x0400 && addr <= 0x07FF)
                mNameTable[1][addr & 0x03FF] = data;
            if (addr >= 0x0800 && addr <= 0x0BFF)
                mNameTable[0][addr & 0x03FF] = data;
            if (addr >= 0x0C00 && addr <= 0x0FFF)
                mNameTable[1][addr & 0x03FF] = data;
        }
        else if (mCartridge->mMirror == Cartridge::MIRRORING::HORIZONTAL)
        {
            // Horizontal
            if (addr >= 0x0000 && addr <= 0x03FF)
                mNameTable[0][addr & 0x03FF] = data;
            if (addr >= 0x0400 && addr <= 0x07FF)
                mNameTable[0][addr & 0x03FF] = data;
            if (addr >= 0x0800 && addr <= 0x0BFF)
                mNameTable[1][addr & 0x03FF] = data;
            if (addr >= 0x0C00 && addr <= 0x0FFF)
                mNameTable[1][addr & 0x03FF] = data;
        }
    }
    else if (addr >= 0x3F00 && addr <= 0x3FFF)
    {
        addr &= 0x001F;
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

void PPU::ConnectCartridge(const std::shared_ptr<Cartridge> &cartridge)
{
    this->mCartridge = cartridge;
}

// void PPU::Reset()
// {
// 	mFineX = 0x00;
// 	mAddressLatch = 0x00;
// 	mDataBufferPPU = 0x00;
// 	mScanline = 0;
// 	mCycle = 0;
// 	mBackgroundNextTileID = 0x00;
// 	mBackgroundNextTileAttrib = 0x00;
// 	mBackgroundNextTileLSB = 0x00;
// 	mBackgroundNextTileMSB = 0x00;
// 	mBackgroundShifterPatternLo = 0x0000;
// 	mBackgroundShifterPatternHi = 0x0000;
// 	mBackgroundShifterAttribLo = 0x0000;
// 	mBackgroundShifterAttribHi = 0x0000;
// 	mStatus.reg = 0x00;
// 	mMask.reg = 0x00;
// 	mControl.reg = 0x00;
// 	mVramAddr.reg = 0x0000;
// 	mTramAddr.reg = 0x0000;
// }

void PPU::Clock()
{
    // As we progress through scanlines and cycles, the PPU is effectively
    // a state machine going through the motions of fetching background
    // information and sprite information, compositing them into a pixel
    // to be output.

    // The lambda functions (functions inside functions) contain the various
    // actions to be performed depending upon the output of the state machine
    // for a given mScanline/mCycle combination

    // ==============================================================================
    // Increment the background tile "pointer" one tile/column horizontally
    auto IncrementScrollX = [&]()
    {
        // Note: pixel perfect scrolling horizontally is handled by the
        // data shifters. Here we are operating in the spatial domain of
        // tiles, 8x8 pixel blocks.

        // Ony if rendering is enabled
        if (mMask.render_background || mMask.render_sprites)
        {
            // A single name table is 32x30 tiles. As we increment horizontally
            // we may cross into a neighbouring nametable, or wrap around to
            // a neighbouring nametable
            if (mVramAddr.coarse_x == 31)
            {
                // Leaving nametable so wrap address round
                mVramAddr.coarse_x = 0;
                // Flip target nametable bit
                mVramAddr.nametable_x = ~mVramAddr.nametable_x;
            }
            else
            {
                // Staying in current nametable, so just increment
                mVramAddr.coarse_x++;
            }
        }
    };

    // ==============================================================================
    // Increment the background tile "pointer" one mScanline vertically
    auto IncrementScrollY = [&]()
    {
        // Incrementing vertically is more complicated. The visible nametable
        // is 32x30 tiles, but in memory there is enough room for 32x32 tiles.
        // The bottom two rows of tiles are in fact not tiles at all, they
        // contain the "attribute" information for the entire table. This is
        // information that describes which palettes are used for different
        // regions of the nametable.

        // In addition, the NES doesnt scroll vertically in chunks of 8 pixels
        // i.e. the height of a tile, it can perform fine scrolling by using
        // the fine_y component of the register. This means an increment in Y
        // first adjusts the fine offset, but may need to adjust the whole
        // row offset, since fine_y is a value 0 to 7, and a row is 8 pixels high

        // Ony if rendering is enabled
        if (mMask.render_background || mMask.render_sprites)
        {
            // If possible, just increment the fine y offset
            if (mVramAddr.fine_y < 7)
            {
                mVramAddr.fine_y++;
            }
            else
            {
                // If we have gone beyond the height of a row, we need to
                // increment the row, potentially wrapping into neighbouring
                // vertical nametables. Dont forget however, the bottom two rows
                // do not contain tile information. The coarse y offset is used
                // to identify which row of the nametable we want, and the fine
                // y offset is the specific "mScanline"

                // Reset fine y offset
                mVramAddr.fine_y = 0;

                // Check if we need to swap vertical nametable targets
                if (mVramAddr.coarse_y == 29)
                {
                    // We do, so reset coarse y offset
                    mVramAddr.coarse_y = 0;
                    // And flip the target nametable bit
                    mVramAddr.nametable_y = ~mVramAddr.nametable_y;
                }
                else if (mVramAddr.coarse_y == 31)
                {
                    // In case the pointer is in the attribute memory, we
                    // just wrap around the current nametable
                    mVramAddr.coarse_y = 0;
                }
                else
                {
                    // None of the above boundary/wrapping conditions apply
                    // so just increment the coarse y offset
                    mVramAddr.coarse_y++;
                }
            }
        }
    };

    // ==============================================================================
    // Transfer the temporarily stored horizontal nametable access information
    // into the "pointer". Note that fine x scrolling is not part of the "pointer"
    // addressing mechanism
    auto TransferAddressX = [&]()
    {
        // Ony if rendering is enabled
        if (mMask.render_background || mMask.render_sprites)
        {
            mVramAddr.nametable_x = mTramAddr.nametable_x;
            mVramAddr.coarse_x = mTramAddr.coarse_x;
        }
    };

    // ==============================================================================
    // Transfer the temporarily stored vertical nametable access information
    // into the "pointer". Note that fine y scrolling is part of the "pointer"
    // addressing mechanism
    auto TransferAddressY = [&]()
    {
        // Ony if rendering is enabled
        if (mMask.render_background || mMask.render_sprites)
        {
            mVramAddr.fine_y = mTramAddr.fine_y;
            mVramAddr.nametable_y = mTramAddr.nametable_y;
            mVramAddr.coarse_y = mTramAddr.coarse_y;
        }
    };

    // ==============================================================================
    // Prime the "in-effect" background tile shifters ready for outputting next
    // 8 pixels in mScanline.
    auto LoadBackgroundShifters = [&]()
    {
        // Each PPU update we calculate one pixel. These shifters shift 1 bit along
        // feeding the pixel compositor with the binary information it needs. Its
        // 16 bits wide, because the top 8 bits are the current 8 pixels being drawn
        // and the bottom 8 bits are the next 8 pixels to be drawn. Naturally this means
        // the required bit is always the MSB of the shifter. However, "fine x" scrolling
        // plays a part in this too, whcih is seen later, so in fact we can choose
        // any one of the top 8 bits.
        mBackgroundShifterPatternLo = (mBackgroundShifterPatternLo & 0xFF00) | mBackgroundNextTileLSB;
        mBackgroundShifterPatternHi = (mBackgroundShifterPatternHi & 0xFF00) | mBackgroundNextTileMSB;

        // Attribute bits do not change per pixel, rather they change every 8 pixels
        // but are synchronised with the pattern shifters for convenience, so here
        // we take the bottom 2 bits of the attribute word which represent which
        // palette is being used for the current 8 pixels and the next 8 pixels, and
        // "inflate" them to 8 bit words.
        mBackgroundShifterAttribLo = (mBackgroundShifterAttribLo & 0xFF00) | ((mBackgroundNextTileAttrib & 0b01) ? 0xFF : 0x00);
        mBackgroundShifterAttribHi = (mBackgroundShifterAttribHi & 0xFF00) | ((mBackgroundNextTileAttrib & 0b10) ? 0xFF : 0x00);
    };

    // ==============================================================================
    // Every mCycle the shifters storing pattern and attribute information shift
    // their contents by 1 bit. This is because every mCycle, the output progresses
    // by 1 pixel. This means relatively, the state of the shifter is in sync
    // with the pixels being drawn for that 8 pixel section of the mScanline.
    auto UpdateShifters = [&]()
    {
        if (mMask.render_background)
        {
            // Shifting background tile pattern row
            mBackgroundShifterPatternLo <<= 1;
            mBackgroundShifterPatternHi <<= 1;

            // Shifting palette attributes by 1
            mBackgroundShifterAttribLo <<= 1;
            mBackgroundShifterAttribHi <<= 1;
        }
    };

    // All but 1 of the secanlines is visible to the user. The pre-render mScanline
    // at -1, is used to configure the "shifters" for the first visible mScanline, 0.
    if (mScanline >= -1 && mScanline < 240)
    {
        if (mScanline == 0 && mCycle == 0)
        {
            // "Odd Frame" mCycle skip
            mCycle = 1;
        }

        if (mScanline == -1 && mCycle == 1)
        {
            // Effectively start of new frame, so clear vertical blank flag
            mStatus.vertical_blank = 0;
        }

        if ((mCycle >= 2 && mCycle < 258) || (mCycle >= 321 && mCycle < 338))
        {
            UpdateShifters();

            // In these cycles we are collecting and working with visible data
            // The "shifters" have been preloaded by the end of the previous
            // mScanline with the data for the start of this mScanline. Once we
            // leave the visible region, we go dormant until the shifters are
            // preloaded for the next mScanline.

            // Fortunately, for background rendering, we go through a fairly
            // repeatable sequence of events, every 2 clock cycles.
            switch ((mCycle - 1) % 8)
            {
            case 0:
                // Load the current background tile pattern and attributes into the "shifter"
                LoadBackgroundShifters();

                // Fetch the next background tile ID
                // "(mVramAddr.reg & 0x0FFF)" : Mask to 12 bits that are relevant
                // "| 0x2000"                 : Offset into nametable space on PPU address bus
                mBackgroundNextTileID = ReadPPU(0x2000 | (mVramAddr.reg & 0x0FFF));

                // Explanation:
                // The bottom 12 bits of the loopy register provide an index into
                // the 4 nametables, regardless of nametable mirroring configuration.
                // nametable_y(1) nametable_x(1) coarse_y(5) coarse_x(5)
                //
                // Consider a single nametable is a 32x32 array, and we have four of them
                //   0                1
                // 0 +----------------+----------------+
                //   |                |                |
                //   |                |                |
                //   |    (32x32)     |    (32x32)     |
                //   |                |                |
                //   |                |                |
                // 1 +----------------+----------------+
                //   |                |                |
                //   |                |                |
                //   |    (32x32)     |    (32x32)     |
                //   |                |                |
                //   |                |                |
                //   +----------------+----------------+
                //
                // This means there are 4096 potential locations in this array, which
                // just so happens to be 2^12!
                break;
            case 2:
                // Fetch the next background tile attribute. OK, so this one is a bit
                // more involved :P

                // Recall that each nametable has two rows of cells that are not tile
                // information, instead they represent the attribute information that
                // indicates which palettes are applied to which area on the screen.
                // Importantly (and frustratingly) there is not a 1 to 1 correspondance
                // between background tile and palette. Two rows of tile data holds
                // 64 attributes. Therfore we can assume that the attributes affect
                // 8x8 zones on the screen for that nametable. Given a working resolution
                // of 256x240, we can further assume that each zone is 32x32 pixels
                // in screen space, or 4x4 tiles. Four system palettes are allocated
                // to background rendering, so a palette can be specified using just
                // 2 bits. The attribute byte therefore can specify 4 distinct palettes.
                // Therefore we can even further assume that a single palette is
                // applied to a 2x2 tile combination of the 4x4 tile zone. The very fact
                // that background tiles "share" a palette locally is the reason why
                // in some games you see distortion in the colours at screen edges.

                // As before when choosing the tile ID, we can use the bottom 12 bits of
                // the loopy register, but we need to make the implementation "coarser"
                // because instead of a specific tile, we want the attribute byte for a
                // group of 4x4 tiles, or in other words, we divide our 32x32 address
                // by 4 to give us an equivalent 8x8 address, and we offset this address
                // into the attribute section of the target nametable.

                // Reconstruct the 12 bit loopy address into an offset into the
                // attribute memory

                // "(mVramAddr.coarse_x >> 2)"        : integer divide coarse x by 4,
                //                                      from 5 bits to 3 bits
                // "((mVramAddr.coarse_y >> 2) << 3)" : integer divide coarse y by 4,
                //                                      from 5 bits to 3 bits,
                //                                      shift to make room for coarse x

                // Result so far: YX00 00yy yxxx

                // All attribute memory begins at 0x03C0 within a nametable, so OR with
                // result to select target nametable, and attribute byte offset. Finally
                // OR with 0x2000 to offset into nametable address space on PPU bus.
                mBackgroundNextTileAttrib = ReadPPU(0x23C0 | (mVramAddr.nametable_y << 11) | (mVramAddr.nametable_x << 10) | ((mVramAddr.coarse_y >> 2) << 3) | (mVramAddr.coarse_x >> 2));

                // Right we've read the correct attribute byte for a specified address,
                // but the byte itself is broken down further into the 2x2 tile groups
                // in the 4x4 attribute zone.

                // The attribute byte is assembled thus: BR(76) BL(54) TR(32) TL(10)
                //
                // +----+----+			    +----+----+
                // | TL | TR |			    | ID | ID |
                // +----+----+ where TL =   +----+----+
                // | BL | BR |			    | ID | ID |
                // +----+----+			    +----+----+
                //
                // Since we know we can access a tile directly from the 12 bit address, we
                // can analyse the bottom bits of the coarse coordinates to provide us with
                // the correct offset into the 8-bit word, to yield the 2 bits we are
                // actually interested in which specifies the palette for the 2x2 group of
                // tiles. We know if "coarse y % 4" < 2 we are in the top half else bottom half.
                // Likewise if "coarse x % 4" < 2 we are in the left half else right half.
                // Ultimately we want the bottom two bits of our attribute word to be the
                // palette selected. So shift as required...
                if (mVramAddr.coarse_y & 0x02)
                    mBackgroundNextTileAttrib >>= 4;
                if (mVramAddr.coarse_x & 0x02)
                    mBackgroundNextTileAttrib >>= 2;
                mBackgroundNextTileAttrib &= 0x03;
                break;

                // Compared to the last two, the next two are the easy ones... :P

            case 4:
                // Fetch the next background tile LSB bit plane from the pattern memory
                // The Tile ID has been read from the nametable. We will use this id to
                // index into the pattern memory to find the correct sprite (assuming
                // the sprites lie on 8x8 pixel boundaries in that memory, which they do
                // even though 8x16 sprites exist, as background tiles are always 8x8).
                //
                // Since the sprites are effectively 1 bit deep, but 8 pixels wide, we
                // can represent a whole sprite row as a single byte, so offsetting
                // into the pattern memory is easy. In total there is 8KB so we need a
                // 13 bit address.

                // "(mControl.pattern_background << 12)"  : the pattern memory selector
                //                                         from mControl register, either 0K
                //                                         or 4K offset
                // "((uint16_t)mBackgroundNextTileID << 4)"    : the tile id multiplied by 16, as
                //                                         2 lots of 8 rows of 8 bit pixels
                // "(mVramAddr.fine_y)"                  : Offset into which row based on
                //                                         vertical scroll offset
                // "+ 0"                                 : Mental clarity for plane offset
                // Note: No PPU address bus offset required as it starts at 0x0000
                mBackgroundNextTileLSB = ReadPPU((mControl.pattern_background << 12) + ((uint16_t)mBackgroundNextTileID << 4) + (mVramAddr.fine_y) + 0);

                break;
            case 6:
                // Fetch the next background tile MSB bit plane from the pattern memory
                // This is the same as above, but has a +8 offset to select the next bit plane
                mBackgroundNextTileMSB = ReadPPU((mControl.pattern_background << 12) + ((uint16_t)mBackgroundNextTileID << 4) + (mVramAddr.fine_y) + 8);
                break;
            case 7:
                // Increment the background tile "pointer" to the next tile horizontally
                // in the nametable memory. Note this may cross nametable boundaries which
                // is a little complex, but essential to implement scrolling
                IncrementScrollX();
                break;
            }
        }

        // End of a visible mScanline, so increment downwards...
        if (mCycle == 256)
        {
            IncrementScrollY();
        }

        //...and reset the x position
        if (mCycle == 257)
        {
            LoadBackgroundShifters();
            TransferAddressX();
        }

        // Superfluous reads of tile id at end of mScanline
        if (mCycle == 338 || mCycle == 340)
        {
            mBackgroundNextTileID = ReadPPU(0x2000 | (mVramAddr.reg & 0x0FFF));
        }

        if (mScanline == -1 && mCycle >= 280 && mCycle < 305)
        {
            // End of vertical blank period so reset the Y address ready for rendering
            TransferAddressY();
        }
    }

    if (mScanline == 240)
    {
        // Post Render Scanline - Do Nothing!
    }

    if (mScanline >= 241 && mScanline < 261)
    {
        if (mScanline == 241 && mCycle == 1)
        {
            // Effectively end of frame, so set vertical blank flag
            mStatus.vertical_blank = 1;

            // If the mControl register tells us to emit a NMI when
            // entering vertical blanking period, do it! The CPU
            // will be informed that rendering is complete so it can
            // perform operations with the PPU knowing it wont
            // produce visible artefacts
            if (mControl.enable_nmi)
                mNMI = true;
        }
    }

    // Composition - We now have background pixel information for this mCycle
    // At this point we are only interested in background

    uint8_t bg_pixel = 0x00;   // The 2-bit pixel to be rendered
    uint8_t bg_palette = 0x00; // The 3-bit index of the palette the pixel indexes

    // We only render backgrounds if the PPU is enabled to do so. Note if
    // background rendering is disabled, the pixel and palette combine
    // to form 0x00. This will fall through the colour tables to yield
    // the current background colour in effect
    if (mMask.render_background)
    {
        // Handle Pixel Selection by selecting the relevant bit
        // depending upon fine x scolling. This has the effect of
        // offsetting ALL background rendering by a set number
        // of pixels, permitting smooth scrolling
        uint16_t bit_mux = 0x8000 >> mFineX;

        // Select Plane pixels by extracting from the shifter
        // at the required location.
        uint8_t p0_pixel = (mBackgroundShifterPatternLo & bit_mux) > 0;
        uint8_t p1_pixel = (mBackgroundShifterPatternHi & bit_mux) > 0;

        // Combine to form pixel index
        bg_pixel = (p1_pixel << 1) | p0_pixel;

        // Get palette
        uint8_t bg_pal0 = (mBackgroundShifterAttribLo & bit_mux) > 0;
        uint8_t bg_pal1 = (mBackgroundShifterAttribHi & bit_mux) > 0;
        bg_palette = (bg_pal1 << 1) | bg_pal0;
    }

    // Now we have a final pixel colour, and a palette for this mCycle
    // of the current mScanline. Let's at long last, draw that ^&%*er :P

    mSpriteScreen.SetPixel(mCycle - 1, mScanline, GetColourFromPaletteRAM(bg_palette, bg_pixel));

    // Fake some noise for now
    // mSpriteScreen.SetPixel(mCycle - 1, mScanline, mPaletteScreen[(rand() % 2) ? 0x3F : 0x30]);

    // Advance renderer - it never stops, it's relentless
    mCycle++;
    if (mCycle >= 341)
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
    return mPaletteScreen[ReadPPU(0x3F00 + (palette * 4) + pixel) & 0x3F];
}
