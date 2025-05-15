#include "Cartridge.h"

#include <fstream>

Cartridge::Cartridge(const std::string &filename)
    : mMapperID(0), mBanksPRG(0), mBanksCHR(0)
{
    // https://www.nesdev.org/wiki/INES

    // 0-3   Constant $4E $45 $53 $1A (ASCII "NES" followed by MS-DOS end-of-file)
    // 4     Size of PRG ROM in 16 KB units
    // 5     Size of CHR ROM in 8 KB units (value 0 means the board uses CHR RAM)
    // 6     Flags 6 - Mapper, mirroring, battery, trainer
    // 7     Flags 7 - Mapper, VS/Playchoice, NES 2.0
    // 8     Flags 8 - PRG-RAM size (rarely used extension)
    // 9     Flags 9 - TV system (rarely used extension)
    // 10    Flags 10 - TV system, PRG-RAM presence (unofficial, rarely used extension)
    // 11-15 Unused padding (should be filled with zero, but some rippers put their name across bytes 7-15)

    struct iNesHeader
    {
        char name[4];
        uint8_t prg_rom_size;
        uint8_t chr_rom_size;
        uint8_t mapper_1;
        uint8_t mapper_2;
        uint8_t prg_ram_size;
        uint8_t tv_system_1;
        uint8_t tv_system_2;
        char unused[5];
    };
    static_assert(sizeof(iNesHeader) == 16);

    iNesHeader header;

    std::ifstream ifs;
    ifs.open(filename, std::ifstream::binary);

    if (ifs.is_open())
    {
        ifs.read(reinterpret_cast<char *>(&header), sizeof(iNesHeader));

        if (header.mapper_1 & 0x04)
        {
            ifs.seekg(512, std::ios_base::cur);
        }

        mMapperID = (header.mapper_2 & 0xF0) | (header.mapper_1 >> 4);

        uint8_t file_type = 1;

        if (file_type == 0)
        {
            // TODO: unimplemented
        }

        if (file_type == 1)
        {
            mBanksPRG = header.prg_rom_size;
            mPRG.resize(mBanksPRG * 16384);
            ifs.read(reinterpret_cast<char *>(mPRG.data()), mPRG.size());

            mBanksCHR = header.chr_rom_size;
            mCHR.resize(mBanksCHR * 8192);
            ifs.read(reinterpret_cast<char *>(mCHR.data()), mCHR.size());
        }

        if (file_type == 2)
        {
            // TODO: unimplemented
        }

        switch (mMapperID)
        {
        case 0:
            mMapper = std::make_shared<Mapper000>(mBanksPRG, mBanksCHR);
            break;

        default:
            break;
        }

        ifs.close();
    }
}

Cartridge::~Cartridge()
{
}

bool Cartridge::WriteCPU(uint16_t addr, uint8_t data)
{
    uint32_t mapped_addr = 0;

    if (mMapper->WriteCPU(addr, mapped_addr))
    {
        mPRG[mapped_addr] = data;
        return true;
    }

    return false;
}

bool Cartridge::ReadCPU(uint16_t addr, uint8_t &data)
{
    uint32_t mapped_addr = 0;

    if (mMapper->ReadCPU(addr, mapped_addr))
    {
        data = mPRG[mapped_addr];
        return true;
    }

    return false;
}

bool Cartridge::WritePPU(uint16_t addr, uint8_t data)
{
    uint32_t mapped_addr = 0;

    if (mMapper->WritePPU(addr, mapped_addr))
    {
        mCHR[mapped_addr] = data;
        return true;
    }

    return false;
}

bool Cartridge::ReadPPU(uint16_t addr, uint8_t &data)
{
    uint32_t mapped_addr = 0;

    if (mMapper->ReadPPU(addr, mapped_addr))
    {
        data = mCHR[mapped_addr];
        return true;
    }

    return false;
}