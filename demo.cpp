#include "Bus.h"

#define OLC_PGE_APPLICATION
#include "PGE/olcPixelGameEngine.h"

#include <cstdint>
#include <sstream>
#include <string>

class Demo : public olc::PixelGameEngine
{
public:
    Demo()
    {
        sAppName = "Demo";
    }

    bool OnUserCreate() override
    {
        // *=$8000
        // LDX #10
        // STX $0000
        // LDX #3
        // STX $0001
        // LDY $0000
        // LDA #0
        // CLC
        // loop
        // ADC $0001
        // DEY
        // BNE loop
        // STA $0002
        // NOP
        // NOP
        // NOP

        // https://www.masswerk.at/6502/assembler.html

        // convert hex string into bytes

        std::stringstream ss;
        ss << "A2 0A 8E 00 00 A2 03 8E 01 00 AC 00 00 A9 00 18 6D 01 00 88 D0 FA 8D 02 00 EA EA EA";
        uint16_t offset = 0x8000;

        while (!ss.eof())
        {
            std::string byte;
            ss >> byte;
            mNES.mRAM[offset++] = uint8_t(std::stoul(byte, nullptr, 16));
        }

        // set reset vector
        mNES.mRAM[0xFFFC] = 0x00;
        mNES.mRAM[0xFFFD] = 0x80;

        mMapASM = mNES.mCPU.Disassemble(0x0000, 0xFFFF);

        // reset
        mNES.mCPU.Reset();

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        Clear(olc::DARK_BLUE);

        if (GetKey(olc::Key::SPACE).bPressed)
        {
            do
            {
                mNES.mCPU.Clock();
            }
            while (!mNES.mCPU.IsComplete());
        }

        if (GetKey(olc::Key::R).bPressed)
        {
            mNES.mCPU.Reset();
        }

        if (GetKey(olc::Key::I).bPressed)
        {
            mNES.mCPU.IRQ();
        }

        if (GetKey(olc::Key::N).bPressed)
        {
            mNES.mCPU.NMI();
        }

        DrawRAM(2,   2, 0x0000, 16, 16);
        DrawRAM(2, 182, 0x8000, 16, 16);
        DrawCPU(448, 2);
        DrawCode(448, 72, 26);

        DrawString(10, 370, "SPACE = Step Instruction | R = Reset | I = IRQ | N = NMI");

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
                offset += " " + CPU::ToHex(mNES.Read(addr, true), 2);
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
        DrawString(x +  64, y, "N", IsFlagSet(CPU::FLAGS::N));
        DrawString(x +  80, y, "O", IsFlagSet(CPU::FLAGS::O));
        DrawString(x +  96, y, "-", IsFlagSet(CPU::FLAGS::U));
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

    CPU::MapASM mMapASM;
};

int main()
{
    Demo demo;

    if (demo.Construct(680, 480, 2, 2))
    {
        demo.Start();
    }

    return 0;
}