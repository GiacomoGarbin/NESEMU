#include "Bus.h"

#define OLC_PGE_APPLICATION
#include "PGE/olcPixelGameEngine.h"

#include <cstdint>
#include <memory>
#include <sstream>
#include <string>

class Demo : public olc::PixelGameEngine
{
public:
    Demo()
        : mIsEmulationRunning(false), mResidualTime(0.0f), mSelectedPalette(0)
    {
        sAppName = "Demo3";
    }

    bool OnUserCreate() override
    {
        mCartridge = std::make_shared<Cartridge>("../ROMS/donkey kong.nes");

        mNES.InsertCartridge(mCartridge);

        mMapASM = mNES.mCPU.Disassemble(0x0000, 0xFFFF);

        mNES.mCPU.Reset();

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        Clear(olc::DARK_BLUE);

        if (mIsEmulationRunning)
        {
            if (mResidualTime > 0.0f)
            {
                mResidualTime -= fElapsedTime;
            }
            else
            {
                mResidualTime += (1.0f / 60.0f) - fElapsedTime;

                do
                {
                    mNES.Clock();
                } while (mNES.mPPU.mIsFrameComplete == false);

                mNES.mPPU.mIsFrameComplete = false;
            }
        }
        else
        {

            if (GetKey(olc::Key::C).bPressed)
            {
                do
                {
                    mNES.Clock();
                } while (mNES.mCPU.IsComplete() == false);

                do
                {
                    mNES.Clock();
                } while (mNES.mCPU.IsComplete() == true);
            }

            if (GetKey(olc::Key::F).bPressed)
            {
                do
                {
                    mNES.Clock();
                } while (mNES.mPPU.mIsFrameComplete == false);

                do
                {
                    mNES.Clock();
                } while (mNES.mCPU.IsComplete() == false);

                mNES.mPPU.mIsFrameComplete = false;
            }
        }

        if (GetKey(olc::Key::R).bPressed)
        {
            mNES.mCPU.Reset();
        }

        if (GetKey(olc::Key::SPACE).bPressed)
        {
            mIsEmulationRunning = !mIsEmulationRunning;
        }

        if (GetKey(olc::Key::P).bPressed)
        {
            (++mSelectedPalette) &= 0x07;
        }

        DrawCPU(516, 2);
        DrawCode(516, 72, 26);

        const int swatch_size = 6;

        // for each palette
        for (int p = 0; p < 8; ++p)
        {
            // for each index
            for (int i = 0; i < 4; ++i)
            {
                FillRect(516 + p * (swatch_size * 5) + i * swatch_size, 340, swatch_size, swatch_size, mNES.mPPU.GetColourFromPaletteRAM(p, i));
            }
        }

        DrawRect(516 + mSelectedPalette * (swatch_size * 5) - 1, 339, swatch_size * 4, swatch_size, olc::WHITE);

        olc::Sprite& pattern_table_0 = mNES.mPPU.GetPatternTable(0, mSelectedPalette);
        olc::Sprite& pattern_table_1 = mNES.mPPU.GetPatternTable(1, mSelectedPalette);

        DrawSprite(516, 348, &pattern_table_0);
        DrawSprite(648, 348, &pattern_table_1);

        DrawSprite(0, 0, &mNES.mPPU.GetScreen(), 2);

        for (uint8_t y = 0; y < 30; ++y)
        {
            for (uint8_t x = 0; x < 32; ++x)
            {
                uint8_t id = mNES.mPPU.mNameTable[0][y * 32 + x];
                // DrawString(x*16, y*16, CPU::ToHex(id, 2));
                DrawPartialSprite(x * 16, y * 16, &pattern_table_1, (id & 0x0F) << 3, ((id >> 4) & 0x0F) << 3, 8, 8, 2);
            }
        }

        return true;
    }

private:
    void DrawRAM(int x, int y, uint16_t addr, int rows, int cols)
    {
        for (int row = 0; row < rows; ++row)
        {
            std::string offset = "$" + CPU::ToHex(addr, 4) + ":";
            for (int col = 0; col < cols; ++col)
            {
                offset += " " + CPU::ToHex(mNES.ReadCPU(addr, true), 2);
                addr += 1;
            }
            DrawString(x, y, offset);
            y += 10;
        }
    }

    void DrawCPU(int x, int y)
    {
        auto IsFlagSet = [&](CPU::FLAGS flag)
        {
            return mNES.mCPU.IsFlagSet(flag) ? olc::GREEN : olc::RED;
        };

        DrawString(x, y, "STATUS:");
        DrawString(x + 64, y, "N", IsFlagSet(CPU::FLAGS::N));
        DrawString(x + 80, y, "O", IsFlagSet(CPU::FLAGS::O));
        DrawString(x + 96, y, "-", IsFlagSet(CPU::FLAGS::U));
        DrawString(x + 112, y, "B", IsFlagSet(CPU::FLAGS::B));
        DrawString(x + 128, y, "D", IsFlagSet(CPU::FLAGS::D));
        DrawString(x + 144, y, "I", IsFlagSet(CPU::FLAGS::I));
        DrawString(x + 160, y, "Z", IsFlagSet(CPU::FLAGS::Z));
        DrawString(x + 178, y, "C", IsFlagSet(CPU::FLAGS::C));

        DrawString(x, y + 10, "PC: $" + CPU::ToHex(mNES.mCPU.mPC, 4));
        DrawString(x, y + 20, "A: $" + CPU::ToHex(mNES.mCPU.mA, 2) + " [" + std::to_string(mNES.mCPU.mA) + "]");
        DrawString(x, y + 30, "X: $" + CPU::ToHex(mNES.mCPU.mX, 2) + " [" + std::to_string(mNES.mCPU.mX) + "]");
        DrawString(x, y + 40, "Y: $" + CPU::ToHex(mNES.mCPU.mY, 2) + " [" + std::to_string(mNES.mCPU.mY) + "]");
        DrawString(x, y + 50, "SP: $" + CPU::ToHex(mNES.mCPU.mSP, 4));
    }

    void DrawCode(int x, int y, int lines)
    {
        auto i = mMapASM.find(mNES.mCPU.mPC);
        int line = (lines >> 1) * 10 + y;

        if (i != mMapASM.end())
        {
            DrawString(x, line, i->second, olc::CYAN);

            while (line < (lines * 10) + y)
            {
                line += 10;

                if (++i != mMapASM.end())
                {
                    DrawString(x, line, i->second);
                }
            }
        }

        i = mMapASM.find(mNES.mCPU.mPC);
        line = (lines >> 1) * 10 + y;

        if (i != mMapASM.end())
        {
            while (line > y)
            {
                line -= 10;

                if (--i != mMapASM.end())
                {
                    DrawString(x, line, i->second);
                }
            }
        }
    }

    Bus mNES;
    std::shared_ptr<Cartridge> mCartridge;

    CPU::MapASM mMapASM;

    bool mIsEmulationRunning;
    float mResidualTime;

    uint8_t mSelectedPalette;
};

int main()
{
    Demo demo;

    if (demo.Construct(780, 480, 2, 2))
    {
        demo.Start();
    }

    return 0;
}