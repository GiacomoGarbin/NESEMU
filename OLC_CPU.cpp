#include "CPU.h"

#include "Bus.h"

void UNIMPLEMENTED(bool enable = false)
{
	if (!enable)
	{
		__asm("nop");
	}
}

CPU::CPU()
	: mA(0x00), mX(0x00), mY(0x00), mSP(0x00), mPC(0x0000), mSR(0x00), mFetch(0x00), mAddrABS(0x0000), mAddrREL(0x0000), mOpcode(0x00), mCycles(0x00), mBus(nullptr)
{
	mLookup =
		{
			{"BRK", &CPU::BRK, &CPU::IMM, 7},
			{"ORA", &CPU::ORA, &CPU::IZX, 6},
			{"???", &CPU::XXX, &CPU::IMP, 2},
			{"???", &CPU::XXX, &CPU::IMP, 8},
			{"???", &CPU::NOP, &CPU::IMP, 3},
			{"ORA", &CPU::ORA, &CPU::ZP0, 3},
			{"ASL", &CPU::ASL, &CPU::ZP0, 5},
			{"???", &CPU::XXX, &CPU::IMP, 5},
			{"PHP", &CPU::PHP, &CPU::IMP, 3},
			{"ORA", &CPU::ORA, &CPU::IMM, 2},
			{"ASL", &CPU::ASL, &CPU::IMP, 2},
			{"???", &CPU::XXX, &CPU::IMP, 2},
			{"???", &CPU::NOP, &CPU::IMP, 4},
			{"ORA", &CPU::ORA, &CPU::ABS, 4},
			{"ASL", &CPU::ASL, &CPU::ABS, 6},
			{"???", &CPU::XXX, &CPU::IMP, 6},
			{"BPL", &CPU::BPL, &CPU::REL, 2},
			{"ORA", &CPU::ORA, &CPU::IZY, 5},
			{"???", &CPU::XXX, &CPU::IMP, 2},
			{"???", &CPU::XXX, &CPU::IMP, 8},
			{"???", &CPU::NOP, &CPU::IMP, 4},
			{"ORA", &CPU::ORA, &CPU::ZPX, 4},
			{"ASL", &CPU::ASL, &CPU::ZPX, 6},
			{"???", &CPU::XXX, &CPU::IMP, 6},
			{"CLC", &CPU::CLC, &CPU::IMP, 2},
			{"ORA", &CPU::ORA, &CPU::ABY, 4},
			{"???", &CPU::NOP, &CPU::IMP, 2},
			{"???", &CPU::XXX, &CPU::IMP, 7},
			{"???", &CPU::NOP, &CPU::IMP, 4},
			{"ORA", &CPU::ORA, &CPU::ABX, 4},
			{"ASL", &CPU::ASL, &CPU::ABX, 7},
			{"???", &CPU::XXX, &CPU::IMP, 7},
			{"JSR", &CPU::JSR, &CPU::ABS, 6},
			{"AND", &CPU::AND, &CPU::IZX, 6},
			{"???", &CPU::XXX, &CPU::IMP, 2},
			{"???", &CPU::XXX, &CPU::IMP, 8},
			{"BIT", &CPU::BIT, &CPU::ZP0, 3},
			{"AND", &CPU::AND, &CPU::ZP0, 3},
			{"ROL", &CPU::ROL, &CPU::ZP0, 5},
			{"???", &CPU::XXX, &CPU::IMP, 5},
			{"PLP", &CPU::PLP, &CPU::IMP, 4},
			{"AND", &CPU::AND, &CPU::IMM, 2},
			{"ROL", &CPU::ROL, &CPU::IMP, 2},
			{"???", &CPU::XXX, &CPU::IMP, 2},
			{"BIT", &CPU::BIT, &CPU::ABS, 4},
			{"AND", &CPU::AND, &CPU::ABS, 4},
			{"ROL", &CPU::ROL, &CPU::ABS, 6},
			{"???", &CPU::XXX, &CPU::IMP, 6},
			{"BMI", &CPU::BMI, &CPU::REL, 2},
			{"AND", &CPU::AND, &CPU::IZY, 5},
			{"???", &CPU::XXX, &CPU::IMP, 2},
			{"???", &CPU::XXX, &CPU::IMP, 8},
			{"???", &CPU::NOP, &CPU::IMP, 4},
			{"AND", &CPU::AND, &CPU::ZPX, 4},
			{"ROL", &CPU::ROL, &CPU::ZPX, 6},
			{"???", &CPU::XXX, &CPU::IMP, 6},
			{"SEC", &CPU::SEC, &CPU::IMP, 2},
			{"AND", &CPU::AND, &CPU::ABY, 4},
			{"???", &CPU::NOP, &CPU::IMP, 2},
			{"???", &CPU::XXX, &CPU::IMP, 7},
			{"???", &CPU::NOP, &CPU::IMP, 4},
			{"AND", &CPU::AND, &CPU::ABX, 4},
			{"ROL", &CPU::ROL, &CPU::ABX, 7},
			{"???", &CPU::XXX, &CPU::IMP, 7},
			{"RTI", &CPU::RTI, &CPU::IMP, 6},
			{"EOR", &CPU::EOR, &CPU::IZX, 6},
			{"???", &CPU::XXX, &CPU::IMP, 2},
			{"???", &CPU::XXX, &CPU::IMP, 8},
			{"???", &CPU::NOP, &CPU::IMP, 3},
			{"EOR", &CPU::EOR, &CPU::ZP0, 3},
			{"LSR", &CPU::LSR, &CPU::ZP0, 5},
			{"???", &CPU::XXX, &CPU::IMP, 5},
			{"PHA", &CPU::PHA, &CPU::IMP, 3},
			{"EOR", &CPU::EOR, &CPU::IMM, 2},
			{"LSR", &CPU::LSR, &CPU::IMP, 2},
			{"???", &CPU::XXX, &CPU::IMP, 2},
			{"JMP", &CPU::JMP, &CPU::ABS, 3},
			{"EOR", &CPU::EOR, &CPU::ABS, 4},
			{"LSR", &CPU::LSR, &CPU::ABS, 6},
			{"???", &CPU::XXX, &CPU::IMP, 6},
			{"BVC", &CPU::BVC, &CPU::REL, 2},
			{"EOR", &CPU::EOR, &CPU::IZY, 5},
			{"???", &CPU::XXX, &CPU::IMP, 2},
			{"???", &CPU::XXX, &CPU::IMP, 8},
			{"???", &CPU::NOP, &CPU::IMP, 4},
			{"EOR", &CPU::EOR, &CPU::ZPX, 4},
			{"LSR", &CPU::LSR, &CPU::ZPX, 6},
			{"???", &CPU::XXX, &CPU::IMP, 6},
			{"CLI", &CPU::CLI, &CPU::IMP, 2},
			{"EOR", &CPU::EOR, &CPU::ABY, 4},
			{"???", &CPU::NOP, &CPU::IMP, 2},
			{"???", &CPU::XXX, &CPU::IMP, 7},
			{"???", &CPU::NOP, &CPU::IMP, 4},
			{"EOR", &CPU::EOR, &CPU::ABX, 4},
			{"LSR", &CPU::LSR, &CPU::ABX, 7},
			{"???", &CPU::XXX, &CPU::IMP, 7},
			{"RTS", &CPU::RTS, &CPU::IMP, 6},
			{"ADC", &CPU::ADC, &CPU::IZX, 6},
			{"???", &CPU::XXX, &CPU::IMP, 2},
			{"???", &CPU::XXX, &CPU::IMP, 8},
			{"???", &CPU::NOP, &CPU::IMP, 3},
			{"ADC", &CPU::ADC, &CPU::ZP0, 3},
			{"ROR", &CPU::ROR, &CPU::ZP0, 5},
			{"???", &CPU::XXX, &CPU::IMP, 5},
			{"PLA", &CPU::PLA, &CPU::IMP, 4},
			{"ADC", &CPU::ADC, &CPU::IMM, 2},
			{"ROR", &CPU::ROR, &CPU::IMP, 2},
			{"???", &CPU::XXX, &CPU::IMP, 2},
			{"JMP", &CPU::JMP, &CPU::IND, 5},
			{"ADC", &CPU::ADC, &CPU::ABS, 4},
			{"ROR", &CPU::ROR, &CPU::ABS, 6},
			{"???", &CPU::XXX, &CPU::IMP, 6},
			{"BVS", &CPU::BVS, &CPU::REL, 2},
			{"ADC", &CPU::ADC, &CPU::IZY, 5},
			{"???", &CPU::XXX, &CPU::IMP, 2},
			{"???", &CPU::XXX, &CPU::IMP, 8},
			{"???", &CPU::NOP, &CPU::IMP, 4},
			{"ADC", &CPU::ADC, &CPU::ZPX, 4},
			{"ROR", &CPU::ROR, &CPU::ZPX, 6},
			{"???", &CPU::XXX, &CPU::IMP, 6},
			{"SEI", &CPU::SEI, &CPU::IMP, 2},
			{"ADC", &CPU::ADC, &CPU::ABY, 4},
			{"???", &CPU::NOP, &CPU::IMP, 2},
			{"???", &CPU::XXX, &CPU::IMP, 7},
			{"???", &CPU::NOP, &CPU::IMP, 4},
			{"ADC", &CPU::ADC, &CPU::ABX, 4},
			{"ROR", &CPU::ROR, &CPU::ABX, 7},
			{"???", &CPU::XXX, &CPU::IMP, 7},
			{"???", &CPU::NOP, &CPU::IMP, 2},
			{"STA", &CPU::STA, &CPU::IZX, 6},
			{"???", &CPU::NOP, &CPU::IMP, 2},
			{"???", &CPU::XXX, &CPU::IMP, 6},
			{"STY", &CPU::STY, &CPU::ZP0, 3},
			{"STA", &CPU::STA, &CPU::ZP0, 3},
			{"STX", &CPU::STX, &CPU::ZP0, 3},
			{"???", &CPU::XXX, &CPU::IMP, 3},
			{"DEY", &CPU::DEY, &CPU::IMP, 2},
			{"???", &CPU::NOP, &CPU::IMP, 2},
			{"TXA", &CPU::TXA, &CPU::IMP, 2},
			{"???", &CPU::XXX, &CPU::IMP, 2},
			{"STY", &CPU::STY, &CPU::ABS, 4},
			{"STA", &CPU::STA, &CPU::ABS, 4},
			{"STX", &CPU::STX, &CPU::ABS, 4},
			{"???", &CPU::XXX, &CPU::IMP, 4},
			{"BCC", &CPU::BCC, &CPU::REL, 2},
			{"STA", &CPU::STA, &CPU::IZY, 6},
			{"???", &CPU::XXX, &CPU::IMP, 2},
			{"???", &CPU::XXX, &CPU::IMP, 6},
			{"STY", &CPU::STY, &CPU::ZPX, 4},
			{"STA", &CPU::STA, &CPU::ZPX, 4},
			{"STX", &CPU::STX, &CPU::ZPY, 4},
			{"???", &CPU::XXX, &CPU::IMP, 4},
			{"TYA", &CPU::TYA, &CPU::IMP, 2},
			{"STA", &CPU::STA, &CPU::ABY, 5},
			{"TXS", &CPU::TXS, &CPU::IMP, 2},
			{"???", &CPU::XXX, &CPU::IMP, 5},
			{"???", &CPU::NOP, &CPU::IMP, 5},
			{"STA", &CPU::STA, &CPU::ABX, 5},
			{"???", &CPU::XXX, &CPU::IMP, 5},
			{"???", &CPU::XXX, &CPU::IMP, 5},
			{"LDY", &CPU::LDY, &CPU::IMM, 2},
			{"LDA", &CPU::LDA, &CPU::IZX, 6},
			{"LDX", &CPU::LDX, &CPU::IMM, 2},
			{"???", &CPU::XXX, &CPU::IMP, 6},
			{"LDY", &CPU::LDY, &CPU::ZP0, 3},
			{"LDA", &CPU::LDA, &CPU::ZP0, 3},
			{"LDX", &CPU::LDX, &CPU::ZP0, 3},
			{"???", &CPU::XXX, &CPU::IMP, 3},
			{"TAY", &CPU::TAY, &CPU::IMP, 2},
			{"LDA", &CPU::LDA, &CPU::IMM, 2},
			{"TAX", &CPU::TAX, &CPU::IMP, 2},
			{"???", &CPU::XXX, &CPU::IMP, 2},
			{"LDY", &CPU::LDY, &CPU::ABS, 4},
			{"LDA", &CPU::LDA, &CPU::ABS, 4},
			{"LDX", &CPU::LDX, &CPU::ABS, 4},
			{"???", &CPU::XXX, &CPU::IMP, 4},
			{"BCS", &CPU::BCS, &CPU::REL, 2},
			{"LDA", &CPU::LDA, &CPU::IZY, 5},
			{"???", &CPU::XXX, &CPU::IMP, 2},
			{"???", &CPU::XXX, &CPU::IMP, 5},
			{"LDY", &CPU::LDY, &CPU::ZPX, 4},
			{"LDA", &CPU::LDA, &CPU::ZPX, 4},
			{"LDX", &CPU::LDX, &CPU::ZPY, 4},
			{"???", &CPU::XXX, &CPU::IMP, 4},
			{"CLV", &CPU::CLV, &CPU::IMP, 2},
			{"LDA", &CPU::LDA, &CPU::ABY, 4},
			{"TSX", &CPU::TSX, &CPU::IMP, 2},
			{"???", &CPU::XXX, &CPU::IMP, 4},
			{"LDY", &CPU::LDY, &CPU::ABX, 4},
			{"LDA", &CPU::LDA, &CPU::ABX, 4},
			{"LDX", &CPU::LDX, &CPU::ABY, 4},
			{"???", &CPU::XXX, &CPU::IMP, 4},
			{"CPY", &CPU::CPY, &CPU::IMM, 2},
			{"CMP", &CPU::CMP, &CPU::IZX, 6},
			{"???", &CPU::NOP, &CPU::IMP, 2},
			{"???", &CPU::XXX, &CPU::IMP, 8},
			{"CPY", &CPU::CPY, &CPU::ZP0, 3},
			{"CMP", &CPU::CMP, &CPU::ZP0, 3},
			{"DEC", &CPU::DEC, &CPU::ZP0, 5},
			{"???", &CPU::XXX, &CPU::IMP, 5},
			{"INY", &CPU::INY, &CPU::IMP, 2},
			{"CMP", &CPU::CMP, &CPU::IMM, 2},
			{"DEX", &CPU::DEX, &CPU::IMP, 2},
			{"???", &CPU::XXX, &CPU::IMP, 2},
			{"CPY", &CPU::CPY, &CPU::ABS, 4},
			{"CMP", &CPU::CMP, &CPU::ABS, 4},
			{"DEC", &CPU::DEC, &CPU::ABS, 6},
			{"???", &CPU::XXX, &CPU::IMP, 6},
			{"BNE", &CPU::BNE, &CPU::REL, 2},
			{"CMP", &CPU::CMP, &CPU::IZY, 5},
			{"???", &CPU::XXX, &CPU::IMP, 2},
			{"???", &CPU::XXX, &CPU::IMP, 8},
			{"???", &CPU::NOP, &CPU::IMP, 4},
			{"CMP", &CPU::CMP, &CPU::ZPX, 4},
			{"DEC", &CPU::DEC, &CPU::ZPX, 6},
			{"???", &CPU::XXX, &CPU::IMP, 6},
			{"CLD", &CPU::CLD, &CPU::IMP, 2},
			{"CMP", &CPU::CMP, &CPU::ABY, 4},
			{"NOP", &CPU::NOP, &CPU::IMP, 2},
			{"???", &CPU::XXX, &CPU::IMP, 7},
			{"???", &CPU::NOP, &CPU::IMP, 4},
			{"CMP", &CPU::CMP, &CPU::ABX, 4},
			{"DEC", &CPU::DEC, &CPU::ABX, 7},
			{"???", &CPU::XXX, &CPU::IMP, 7},
			{"CPX", &CPU::CPX, &CPU::IMM, 2},
			{"SBC", &CPU::SBC, &CPU::IZX, 6},
			{"???", &CPU::NOP, &CPU::IMP, 2},
			{"???", &CPU::XXX, &CPU::IMP, 8},
			{"CPX", &CPU::CPX, &CPU::ZP0, 3},
			{"SBC", &CPU::SBC, &CPU::ZP0, 3},
			{"INC", &CPU::INC, &CPU::ZP0, 5},
			{"???", &CPU::XXX, &CPU::IMP, 5},
			{"INX", &CPU::INX, &CPU::IMP, 2},
			{"SBC", &CPU::SBC, &CPU::IMM, 2},
			{"NOP", &CPU::NOP, &CPU::IMP, 2},
			{"???", &CPU::SBC, &CPU::IMP, 2},
			{"CPX", &CPU::CPX, &CPU::ABS, 4},
			{"SBC", &CPU::SBC, &CPU::ABS, 4},
			{"INC", &CPU::INC, &CPU::ABS, 6},
			{"???", &CPU::XXX, &CPU::IMP, 6},
			{"BEQ", &CPU::BEQ, &CPU::REL, 2},
			{"SBC", &CPU::SBC, &CPU::IZY, 5},
			{"???", &CPU::XXX, &CPU::IMP, 2},
			{"???", &CPU::XXX, &CPU::IMP, 8},
			{"???", &CPU::NOP, &CPU::IMP, 4},
			{"SBC", &CPU::SBC, &CPU::ZPX, 4},
			{"INC", &CPU::INC, &CPU::ZPX, 6},
			{"???", &CPU::XXX, &CPU::IMP, 6},
			{"SED", &CPU::SED, &CPU::IMP, 2},
			{"SBC", &CPU::SBC, &CPU::ABY, 4},
			{"NOP", &CPU::NOP, &CPU::IMP, 2},
			{"???", &CPU::XXX, &CPU::IMP, 7},
			{"???", &CPU::NOP, &CPU::IMP, 4},
			{"SBC", &CPU::SBC, &CPU::ABX, 4},
			{"INC", &CPU::INC, &CPU::ABX, 7},
			{"???", &CPU::XXX, &CPU::IMP, 7},
		};
}

CPU::~CPU()
{
}

void CPU::ConnectBus(Bus *pBus)
{
	mBus = pBus;
}

bool CPU::IsFlagSet(FLAGS flag) const
{
	return GetFlag(flag) != 0x00;
}

bool CPU::IsComplete() const
{
	return (mCycles == 0);
}

CPU::MapASM CPU::Disassemble(uint16_t nStart, uint16_t nStop)
{
	uint32_t addr = nStart;
	uint8_t value = 0x00, lo = 0x00, hi = 0x00;
	std::map<uint16_t, std::string> mapLines;
	uint16_t line_addr = 0;

	// A convenient utility to convert variables into
	// hex strings because "modern C++"'s method with
	// streams is atrocious
	auto hex = [](uint32_t n, uint8_t d)
	{
		std::string s(d, '0');
		for (int i = d - 1; i >= 0; i--, n >>= 4)
			s[i] = "0123456789ABCDEF"[n & 0xF];
		return s;
	};

	// Starting at the specified address we read an instruction
	// byte, which in turn yields information from the mLookup table
	// as to how many additional bytes we need to read and what the
	// addressing mode is. I need this info to assemble human readable
	// syntax, which is different depending upon the addressing mode

	// As the instruction is decoded, a std::string is assembled
	// with the readable output
	while (addr <= (uint32_t)nStop)
	{
		line_addr = addr;

		// Prefix line with instruction address
		std::string sInst = "$" + hex(addr, 4) + ": ";

		// Read instruction, and get its readable name
		uint8_t opcode = mBus->ReadCPU(addr, true);
		addr++;
		sInst += mLookup[opcode].name + " ";

		// Get oprands from desired locations, and form the
		// instruction based upon its addressing mode. These
		// routines mimmick the actual fetch routine of the
		// 6502 in order to get accurate data as part of the
		// instruction
		if (mLookup[opcode].addrmode == &CPU::IMP)
		{
			sInst += " {IMP}";
		}
		else if (mLookup[opcode].addrmode == &CPU::IMM)
		{
			value = mBus->ReadCPU(addr, true);
			addr++;
			sInst += "#$" + hex(value, 2) + " {IMM}";
		}
		else if (mLookup[opcode].addrmode == &CPU::ZP0)
		{
			lo = mBus->ReadCPU(addr, true);
			addr++;
			hi = 0x00;
			sInst += "$" + hex(lo, 2) + " {ZP0}";
		}
		else if (mLookup[opcode].addrmode == &CPU::ZPX)
		{
			lo = mBus->ReadCPU(addr, true);
			addr++;
			hi = 0x00;
			sInst += "$" + hex(lo, 2) + ", X {ZPX}";
		}
		else if (mLookup[opcode].addrmode == &CPU::ZPY)
		{
			lo = mBus->ReadCPU(addr, true);
			addr++;
			hi = 0x00;
			sInst += "$" + hex(lo, 2) + ", Y {ZPY}";
		}
		else if (mLookup[opcode].addrmode == &CPU::IZX)
		{
			lo = mBus->ReadCPU(addr, true);
			addr++;
			hi = 0x00;
			sInst += "($" + hex(lo, 2) + ", X) {IZX}";
		}
		else if (mLookup[opcode].addrmode == &CPU::IZY)
		{
			lo = mBus->ReadCPU(addr, true);
			addr++;
			hi = 0x00;
			sInst += "($" + hex(lo, 2) + "), Y {IZY}";
		}
		else if (mLookup[opcode].addrmode == &CPU::ABS)
		{
			lo = mBus->ReadCPU(addr, true);
			addr++;
			hi = mBus->ReadCPU(addr, true);
			addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + " {ABS}";
		}
		else if (mLookup[opcode].addrmode == &CPU::ABX)
		{
			lo = mBus->ReadCPU(addr, true);
			addr++;
			hi = mBus->ReadCPU(addr, true);
			addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", X {ABX}";
		}
		else if (mLookup[opcode].addrmode == &CPU::ABY)
		{
			lo = mBus->ReadCPU(addr, true);
			addr++;
			hi = mBus->ReadCPU(addr, true);
			addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", Y {ABY}";
		}
		else if (mLookup[opcode].addrmode == &CPU::IND)
		{
			lo = mBus->ReadCPU(addr, true);
			addr++;
			hi = mBus->ReadCPU(addr, true);
			addr++;
			sInst += "($" + hex((uint16_t)(hi << 8) | lo, 4) + ") {IND}";
		}
		else if (mLookup[opcode].addrmode == &CPU::REL)
		{
			value = mBus->ReadCPU(addr, true);
			addr++;
			sInst += "$" + hex(value, 2) + " [$" + hex(addr + (int8_t)value, 4) + "] {REL}";
		}

		// Add the formed string to a std::map, using the instruction's
		// address as the key. This makes it convenient to look for later
		// as the instructions are variable in length, so a straight up
		// incremental index is not sufficient.
		mapLines[line_addr] = sInst;
	}

	return mapLines;
}

std::string CPU::ToHex(uint32_t value, uint8_t digits)
{
	std::string hex = std::string(digits, '0');

	for (int i = digits - 1; i >= 0; --i, value >>= 4)
	{
		hex[i] = "0123456789ABCDEF"[value & 0x0000000F];
	}

	return hex;
}

void CPU::Write(uint16_t addr, uint8_t data)
{
	mBus->WriteCPU(addr, data);
}

uint8_t CPU::Read(uint16_t addr)
{
	return mBus->ReadCPU(addr, false);
}

uint8_t CPU::GetFlag(FLAGS flag) const
{
	return ((mSR & flag) > 0) ? 1 : 0;
}

void CPU::SetFlag(FLAGS flag, bool enable)
{
	if (enable)
	{
		mSR |= flag;
	}
	else
	{
		mSR &= ~flag;
	}
}

// Addressing Modes

uint8_t CPU::IMP()
{
	// there is no data as part of the instruction
	// however it can operate upon the accumulator, so we set mFetch to the content of the A register

	mFetch = mA;
	return 0;
}

uint8_t CPU::IMM()
{
	// data is supplied as part of the instruction (the next byte)

	mAddrABS = mPC++;
	return 0;
}

uint8_t CPU::ZP0()
{
	// the byte of data we are interesting in reading for this instruction
	// can be found somewhere in page zero (i.e. the high-byte is zero)

	mAddrABS = Read(mPC);
	mPC++;
	mAddrABS &= 0x00FF;
	return 0;
}

uint8_t CPU::ZPX()
{
	// the address supplied with the instruction had the content of the X register added to it
	// useful for iterating regions of memory, such as arrays

	mAddrABS = (Read(mPC) + mX);
	mPC++;
	mAddrABS &= 0x00FF;
	return 0;
}

uint8_t CPU::ZPY()
{
	// the address supplied with the instruction had the content of the Y register added to it
	// useful for iterating regions of memory, such as arrays

	mAddrABS = (Read(mPC) + mY);
	mPC++;
	mAddrABS &= 0x00FF;
	return 0;
}

uint8_t CPU::ABS()
{
	// the data supplied with the instruction consists of the low byte and high byte of the 16-byte address

	uint16_t lo = Read(mPC);
	mPC++;
	uint16_t hi = Read(mPC);
	mPC++;

	mAddrABS = ((hi << 8) | lo);

	return 0;
}

uint8_t CPU::ABX()
{
	// the data supplied with the instruction consists of the low byte and high byte of the 16-byte address
	// the address supplied with the instruction had the content of the X register added to it

	uint16_t lo = Read(mPC);
	mPC++;
	uint16_t hi = Read(mPC);
	mPC++;

	mAddrABS = ((hi << 8) | lo);
	mAddrABS += mX;

	// if after incrementing with the X register, the whole address has changed to a different page,
	// wa may need an additional clock cycle

	if ((mAddrABS & 0xFF00) != (hi << 8))
	{
		return 1;
	}

	return 0;
}

uint8_t CPU::ABY()
{
	// the data supplied with the instruction consists of the low byte and high byte of the 16-byte address
	// the address supplied with the instruction had the content of the Y register added to it

	uint16_t lo = Read(mPC);
	mPC++;
	uint16_t hi = Read(mPC);
	mPC++;

	mAddrABS = ((hi << 8) | lo);
	mAddrABS += mY;

	// if after incrementing with the Y register, the whole address has changed to a different page,
	// wa may need an additional clock cycle

	if ((mAddrABS & 0xFF00) != (hi << 8))
	{
		return 1;
	}

	return 0;
}

uint8_t CPU::IND()
{
	// the data supplied with the instruction consists of the low byte and high byte of the 16-byte address
	// the address supplied with the instruction is a pointer, we interrogate that location to get the actual address
	// which is where the data we actually want resides

	uint16_t lo = Read(mPC);
	mPC++;
	uint16_t hi = Read(mPC);
	mPC++;

	uint16_t ptr = ((hi << 8) | lo);

	if (lo == 0x00FF)
	{
		// simulate page boundary hardware bug
		mAddrABS = ((Read(ptr & 0xFF00) << 8) | Read(ptr + 0));
	}
	else
	{
		mAddrABS = ((Read(ptr + 1) << 8) | Read(ptr + 0));
	}

	return 0;
}

uint8_t CPU::IZX()
{
	// indirect addressing mode of the zero page with X register offset
	// the supplied address references somewhere in the zero page
	// we offset that 1-byte address by the contents of the X register
	// to read the 2-byte actual address we need for the instruction

	uint16_t zp = Read(mPC);
	mPC++;

	uint16_t lo = Read((uint16_t)(zp + (uint16_t)mX + 0) & 0x00FF);
	uint16_t hi = Read((uint16_t)(zp + (uint16_t)mX + 1) & 0x00FF);

	mAddrABS = ((hi << 8) | lo);

	return 0;
}

uint8_t CPU::IZY()
{
	// we read 1-byte which is an offset in the zero page
	// we read the 2-byte actual address from that location
	// and we offset that address by the contents of the Y register

	uint16_t zp = Read(mPC);
	mPC++;

	uint16_t lo = Read((zp + 0) & 0x00FF);
	uint16_t hi = Read((zp + 1) & 0x00FF);

	mAddrABS = ((hi << 8) | lo);
	mAddrABS += mY;

	// if after incrementing with the Y register, the whole address has changed to a different page,
	// wa may need an additional clock cycle

	if ((mAddrABS & 0xFF00) != (hi << 8))
	{
		return 1;
	}

	return 0;
}

uint8_t CPU::REL()
{
	// relative addressing mode, it only applies to braching instructions
	// branch instructions can't jump further away than 127 memory locations

	mAddrREL = Read(mPC);
	mPC++;

	if (mAddrREL & 0x0080)
	{
		// jumping backwards, adjust the address for binary arithmetic
		mAddrREL |= 0xFF00;
	}

	return 0;
}

// Instructions

uint8_t CPU::Fetch()
{
	// we want to fetch data for all instructions but those that use implied address mode

	if (mLookup[mOpcode].addrmode != &CPU::IMP)
	{
		mFetch = Read(mAddrABS);
	}
	return mFetch;
}

uint8_t CPU::ADC()
{
	// Grab the data that we are adding to the accumulator
	Fetch();

	// Add is performed in 16-bit domain for emulation to capture any
	// carry bit, which will exist in bit 8 of the 16-bit word
	uint16_t temp = (uint16_t)mA + (uint16_t)mFetch + (uint16_t)GetFlag(C);

	// The carry flag out exists in the high byte bit 0
	SetFlag(FLAGS::C, temp > 255);

	// The Zero flag is set if the result is 0
	SetFlag(FLAGS::Z, (temp & 0x00FF) == 0);

	// The signed Overflow flag is set based on all that up there! :D
	SetFlag(FLAGS::O, (~((uint16_t)mA ^ (uint16_t)mFetch) & ((uint16_t)mA ^ (uint16_t)temp)) & 0x0080);

	// The negative flag is set to the most significant bit of the result
	SetFlag(FLAGS::N, temp & 0x80);

	// Load the result into the accumulator (it's 8-bit dont forget!)
	mA = temp & 0x00FF;

	// This instruction has the potential to require an additional clock cycle
	return 1;
}

uint8_t CPU::SBC()
{
	Fetch();

	// Operating in 16-bit domain to capture carry out

	// We can invert the bottom 8 bits with bitwise xor
	uint16_t value = ((uint16_t)mFetch) ^ 0x00FF;

	// Notice this is exactly the same as addition from here!
	uint16_t temp = (uint16_t)mA + value + (uint16_t)GetFlag(C);
	SetFlag(FLAGS::C, temp & 0xFF00);
	SetFlag(FLAGS::Z, ((temp & 0x00FF) == 0));
	SetFlag(FLAGS::O, (temp ^ (uint16_t)mA) & (temp ^ value) & 0x0080);
	SetFlag(FLAGS::N, temp & 0x0080);
	mA = temp & 0x00FF;
	return 1;
}

uint8_t CPU::AND()
{
	Fetch();
	mA = mA & mFetch;
	SetFlag(FLAGS::Z, mA == 0x00);
	SetFlag(FLAGS::N, mA & 0x80);
	return 1; // can potentially require an additional cycle if page boundary is crossed
}

uint8_t CPU::ASL()
{
	Fetch();
	uint16_t temp = (uint16_t)mFetch << 1;
	SetFlag(FLAGS::C, (temp & 0xFF00) > 0);
	SetFlag(FLAGS::Z, (temp & 0x00FF) == 0x00);
	SetFlag(FLAGS::N, temp & 0x80);
	if (mLookup[mOpcode].addrmode == &CPU::IMP)
		mA = temp & 0x00FF;
	else
		Write(mAddrABS, temp & 0x00FF);
	return 0;
}

uint8_t CPU::BCC()
{
	return BranchIf(GetFlag(FLAGS::C) == 0x00);
}

uint8_t CPU::BCS()
{
	return BranchIf(GetFlag(FLAGS::C) != 0x00);
}

uint8_t CPU::BEQ()
{
	return BranchIf(GetFlag(FLAGS::Z) != 0x00);
}

uint8_t CPU::BIT()
{
	Fetch();
	uint16_t temp = mA & mFetch;
	SetFlag(FLAGS::Z, (temp & 0x00FF) == 0x0000);
	SetFlag(FLAGS::N, mFetch & (1 << 7));
	SetFlag(FLAGS::O, mFetch & (1 << 6));
	return 0;
}

uint8_t CPU::BMI()
{
	return BranchIf(GetFlag(FLAGS::N) != 0x00);
}

uint8_t CPU::BNE()
{
	return BranchIf(GetFlag(FLAGS::Z) == 0x00);
}

uint8_t CPU::BPL()
{
	return BranchIf(GetFlag(FLAGS::N) == 0x00);
}

uint8_t CPU::BRK()
{
	mPC++;

	SetFlag(FLAGS::I, 1);
	Write(0x0100 + mSP, (mPC >> 8) & 0x00FF);
	mSP--;
	Write(0x0100 + mSP, mPC & 0x00FF);
	mSP--;

	SetFlag(FLAGS::B, 1);
	Write(0x0100 + mSP, mSR);
	mSP--;
	SetFlag(FLAGS::B, 0);

	mPC = (uint16_t)Read(0xFFFE) | ((uint16_t)Read(0xFFFF) << 8);
	return 0;
}

uint8_t CPU::BVC()
{
	return BranchIf(GetFlag(FLAGS::O) == 0x00);
}

uint8_t CPU::BVS()
{
	return BranchIf(GetFlag(FLAGS::O) != 0x00);
}

uint8_t CPU::CLC()
{
	SetFlag(FLAGS::C, false);
	return 0;
}

uint8_t CPU::CLD()
{
	SetFlag(FLAGS::D, false);
	return 0;
}

uint8_t CPU::CLI()
{
	SetFlag(FLAGS::I, false);
	return 0;
}

uint8_t CPU::CLV()
{
	SetFlag(FLAGS::O, false);
	return 0;
}

uint8_t CPU::CMP()
{
	Fetch();
	uint16_t temp = (uint16_t)mA - (uint16_t)mFetch;
	SetFlag(FLAGS::C, mA >= mFetch);
	SetFlag(FLAGS::Z, (temp & 0x00FF) == 0x0000);
	SetFlag(FLAGS::N, temp & 0x0080);
	return 1;
}

uint8_t CPU::CPX()
{
	Fetch();
	uint16_t temp = (uint16_t)mX - (uint16_t)mFetch;
	SetFlag(FLAGS::C, mX >= mFetch);
	SetFlag(FLAGS::Z, (temp & 0x00FF) == 0x0000);
	SetFlag(FLAGS::N, temp & 0x0080);
	return 0;
}

uint8_t CPU::CPY()
{
	Fetch();
	uint16_t temp = (uint16_t)mY - (uint16_t)mFetch;
	SetFlag(FLAGS::C, mY >= mFetch);
	SetFlag(FLAGS::Z, (temp & 0x00FF) == 0x0000);
	SetFlag(FLAGS::N, temp & 0x0080);
	return 0;
}

uint8_t CPU::DEC()
{
	Fetch();
	uint16_t temp = mFetch - 1;
	Write(mAddrABS, temp & 0x00FF);
	SetFlag(FLAGS::Z, (temp & 0x00FF) == 0x0000);
	SetFlag(FLAGS::N, temp & 0x0080);
	return 0;
}

uint8_t CPU::DEX()
{
	mX--;

	SetFlag(FLAGS::Z, mX == 0x00);
	SetFlag(FLAGS::N, mX & 0x80);

	return 0;
}

uint8_t CPU::DEY()
{
	mY--;

	SetFlag(FLAGS::Z, mY == 0x00);
	SetFlag(FLAGS::N, mY & 0x80);

	return 0;
}

uint8_t CPU::EOR()
{
	Fetch();
	mA = mA ^ mFetch;
	SetFlag(FLAGS::Z, mA == 0x00);
	SetFlag(FLAGS::N, mA & 0x80);
	return 1;
}

uint8_t CPU::INC()
{
	Fetch();
	uint16_t temp = mFetch + 1;
	Write(mAddrABS, temp & 0x00FF);
	SetFlag(FLAGS::Z, (temp & 0x00FF) == 0x0000);
	SetFlag(FLAGS::N, temp & 0x0080);
	return 0;
}

uint8_t CPU::INX()
{
	mX++;
	SetFlag(FLAGS::Z, mX == 0x00);
	SetFlag(FLAGS::N, mX & 0x80);
	return 0;
}

uint8_t CPU::INY()
{
	mY++;
	SetFlag(FLAGS::Z, mY == 0x00);
	SetFlag(FLAGS::N, mY & 0x80);
	return 0;
}

uint8_t CPU::JMP()
{
	mPC = mAddrABS;
	return 0;
}

uint8_t CPU::JSR()
{
	mPC--;

	Write(0x0100 + mSP, (mPC >> 8) & 0x00FF);
	mSP--;
	Write(0x0100 + mSP, mPC & 0x00FF);
	mSP--;

	mPC = mAddrABS;
	return 0;
}

uint8_t CPU::LDA()
{
	Fetch();
	mA = mFetch;

	SetFlag(FLAGS::Z, mA == 0x00);
	SetFlag(FLAGS::N, mA & 0x80);

	return 1;
}

uint8_t CPU::LDX()
{
	Fetch();
	mX = mFetch;

	SetFlag(FLAGS::Z, mX == 0x00);
	SetFlag(FLAGS::N, mX & 0x80);

	return 1;
}

uint8_t CPU::LDY()
{
	Fetch();
	mY = mFetch;

	SetFlag(FLAGS::Z, mY == 0x00);
	SetFlag(FLAGS::N, mY & 0x80);

	return 1;
}

uint8_t CPU::LSR()
{
	Fetch();
	SetFlag(FLAGS::C, mFetch & 0x0001);
	uint16_t temp = mFetch >> 1;
	SetFlag(FLAGS::Z, (temp & 0x00FF) == 0x0000);
	SetFlag(FLAGS::N, temp & 0x0080);
	if (mLookup[mOpcode].addrmode == &CPU::IMP)
		mA = temp & 0x00FF;
	else
		Write(mAddrABS, temp & 0x00FF);
	return 0;
}

uint8_t CPU::NOP()
{
	switch (mOpcode)
	{
	case 0x1C:
	case 0x3C:
	case 0x5C:
	case 0x7C:
	case 0xDC:
	case 0xFC:
		return 1;

	default:
		break;
	}

	return 0;
}

uint8_t CPU::ORA()
{
	Fetch();
	mA = mA | mFetch;
	SetFlag(FLAGS::Z, mA == 0x00);
	SetFlag(FLAGS::N, mA & 0x80);
	return 1;
}

uint8_t CPU::PHA()
{
	Write(0x0100 + mSP, mA);
	mSP--;
	return 0;
}

uint8_t CPU::PHP()
{
	Write(0x0100 + mSP, mSR | B | U);
	SetFlag(FLAGS::B, 0);
	SetFlag(FLAGS::U, 0);
	mSP--;
	return 0;
}

uint8_t CPU::PLA()
{
	mSP++;
	mA = Read(0x0100 + mSP);
	SetFlag(FLAGS::Z, mA == 0x00);
	SetFlag(FLAGS::N, mA & 0x80);
	return 0;
}

uint8_t CPU::PLP()
{
	mSP++;
	mSR = Read(0x0100 + mSP);
	SetFlag(FLAGS::U, 1);
	return 0;
}

uint8_t CPU::ROL()
{
	Fetch();
	uint16_t temp = (uint16_t)(mFetch << 1) | GetFlag(C);
	SetFlag(FLAGS::C, temp & 0xFF00);
	SetFlag(FLAGS::Z, (temp & 0x00FF) == 0x0000);
	SetFlag(FLAGS::N, temp & 0x0080);
	if (mLookup[mOpcode].addrmode == &CPU::IMP)
		mA = temp & 0x00FF;
	else
		Write(mAddrABS, temp & 0x00FF);
	return 0;
}

uint8_t CPU::ROR()
{
	Fetch();
	uint16_t temp = (uint16_t)(GetFlag(C) << 7) | (mFetch >> 1);
	SetFlag(FLAGS::C, mFetch & 0x01);
	SetFlag(FLAGS::Z, (temp & 0x00FF) == 0x00);
	SetFlag(FLAGS::N, temp & 0x0080);
	if (mLookup[mOpcode].addrmode == &CPU::IMP)
		mA = temp & 0x00FF;
	else
		Write(mAddrABS, temp & 0x00FF);
	return 0;
}

uint8_t CPU::RTI()
{
	mSP++;
	mSR = Read(0x0100 + mSP);
	mSR &= ~B;
	mSR &= ~U;

	mSP++;
	mPC = (uint16_t)Read(0x0100 + mSP);
	mSP++;
	mPC |= (uint16_t)Read(0x0100 + mSP) << 8;
	return 0;
}

uint8_t CPU::RTS()
{
	mSP++;
	mPC = (uint16_t)Read(0x0100 + mSP);
	mSP++;
	mPC |= (uint16_t)Read(0x0100 + mSP) << 8;

	mPC++;
	return 0;
}

uint8_t CPU::SEC()
{
	SetFlag(FLAGS::C, true);
	return 0;
}

uint8_t CPU::SED()
{
	SetFlag(FLAGS::D, true);
	return 0;
}

uint8_t CPU::SEI()
{
	SetFlag(FLAGS::I, true);
	return 0;
}

uint8_t CPU::STA()
{
	Write(mAddrABS, mA);
	return 0;
}

uint8_t CPU::STX()
{
	Write(mAddrABS, mX);
	return 0;
}

uint8_t CPU::STY()
{
	Write(mAddrABS, mY);
	return 0;
}

uint8_t CPU::TAX()
{
	mX = mA;
	SetFlag(FLAGS::Z, mX == 0x00);
	SetFlag(FLAGS::N, mX & 0x80);
	return 0;
}

uint8_t CPU::TAY()
{
	mY = mA;
	SetFlag(FLAGS::Z, mY == 0x00);
	SetFlag(FLAGS::N, mY & 0x80);
	return 0;
}

uint8_t CPU::TSX()
{
	mX = mSP;
	SetFlag(FLAGS::Z, mX == 0x00);
	SetFlag(FLAGS::N, mX & 0x80);
	return 0;
}

uint8_t CPU::TXA()
{
	mA = mX;
	SetFlag(FLAGS::Z, mA == 0x00);
	SetFlag(FLAGS::N, mA & 0x80);
	return 0;
}

uint8_t CPU::TXS()
{
	mSP = mX;
	return 0;
}

uint8_t CPU::TYA()
{
	mA = mY;
	SetFlag(FLAGS::Z, mA == 0x00);
	SetFlag(FLAGS::N, mA & 0x80);
	return 0;
}

uint8_t CPU::XXX()
{
	// TODO: should we do something?
	return 0;
}

// Signals

void CPU::Clock()
{
	if (mCycles == 0)
	{
		mOpcode = Read(mPC);

		SetFlag(FLAGS::U, true);

		mPC++;

		// get starting number of cycles
		mCycles = mLookup[mOpcode].cycles;

		uint8_t additional_cycle_1 = (this->*mLookup[mOpcode].addrmode)();
		uint8_t additional_cycle_2 = (this->*mLookup[mOpcode].operate)();

		mCycles += (additional_cycle_1 & additional_cycle_2);

		SetFlag(FLAGS::U, true);
	}

	mCycles--;
}

void CPU::Reset()
{
	// Get address to set program counter to
	mAddrABS = 0xFFFC;
	uint16_t lo = Read(mAddrABS + 0);
	uint16_t hi = Read(mAddrABS + 1);

	// Set it
	mPC = (hi << 8) | lo;

	// Reset internal registers
	mA = 0;
	mX = 0;
	mY = 0;
	mSP = 0xFD;
	mSR = 0x00 | U;

	// Clear internal helper variables
	mAddrREL = 0x0000;
	mAddrABS = 0x0000;
	mFetch = 0x00;

	// Reset takes time
	mCycles = 8;
}

void CPU::IRQ()
{
	if (GetFlag(FLAGS::I) == 0x00)
	{
		CallInterrupt(0xFFFE);
		mCycles = 7;
	}
}

void CPU::NMI()
{
	CallInterrupt(0xFFFA);
	mCycles = 8;
}

// Helpers

uint8_t CPU::BranchIf(const bool cond)
{
	if (cond)
	{
		mCycles++;
		mAddrABS = mPC + mAddrREL;

		if ((mAddrABS & 0xFF00) != (mPC & 0xFF00))
		{
			// add a second clock cycle if the branch needs to cross a page boundary
			mCycles++;
		}

		mPC = mAddrABS;
	}
	return 0;
}

uint8_t CPU::AddValue(const uint16_t mask)
{
	Fetch();
	uint16_t value = uint16_t(mFetch) ^ mask;
	uint16_t temp = uint16_t(mA) + value + uint16_t(GetFlag(FLAGS::C));
	SetFlag(FLAGS::C, (temp & 0x0100) != 0x0000);
	SetFlag(FLAGS::Z, (temp & 0x00FF) == 0x0000);
	SetFlag(FLAGS::N, (temp & 0x0080) != 0x0000);
	SetFlag(FLAGS::O, ((~(uint16_t(mA) ^ uint16_t(mFetch)) & (uint16_t(mA) ^ temp)) & 0x0080) != 0x0000);
	mA = uint8_t(temp & 0x00FF);
	return 1;
}

void CPU::JumpAddrABS(const uint16_t addr)
{
	mAddrABS = addr;
	uint16_t lo = Read(mAddrABS + 0);
	uint16_t hi = Read(mAddrABS + 1);
	mPC = (hi << 8) | lo;
}

void CPU::PushValue(const uint8_t value)
{
	Write(0x0100 + uint16_t(mSP), value);
	mSP--;
}

uint8_t CPU::PopValue()
{
	mSP++;
	return Read(0x0100 + uint16_t(mSP));
}

void CPU::CallInterrupt(const uint16_t routine)
{
	PushValue((mPC >> 8) & 0x00FF);
	PushValue(mPC & 0x00FF);

	SetFlag(FLAGS::B, false);
	SetFlag(FLAGS::U, true);
	SetFlag(FLAGS::I, true);
	PushValue(mSR);

	JumpAddrABS(routine);
}