#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

class Bus;

class CPU
{
public:
    CPU();
    ~CPU();

    enum FLAGS
    {
        C = (1 << 0), // carry bit
        Z = (1 << 1), // zero
        I = (1 << 2), // disable interrupts
        D = (1 << 3), // decimal mode
        B = (1 << 4), // break
        U = (1 << 5), // unused
        O = (1 << 6), // overflow
        N = (1 << 7), // negative
    };

    // registers

    uint8_t mA;   // A register
    uint8_t mX;   // X register
    uint8_t mY;   // Y register
    uint8_t mSP;  // stack pointer
    uint16_t mPC; // program counter
    uint8_t mSR;  // status register

    void ConnectBus(Bus *pBus);

    bool IsFlagSet(FLAGS flag) const;
    bool IsComplete() const;

    using MapASM = std::map<uint16_t, std::string>;
    MapASM Disassemble(uint16_t begin, uint16_t end);

    static std::string ToHex(uint32_t value, uint8_t digits);

    // addressing modes (12)

    uint8_t ABS(); // absolute address
    uint8_t ABX(); // absolute address, X register offset
    uint8_t ABY(); // absolute address, Y register offset
    uint8_t IMM(); // immediate data source
    uint8_t IMP(); // implied data source
    uint8_t IND(); // indirect (aka pointer)
    uint8_t IZX(); // indirect zero page, X register offset
    uint8_t IZY(); // indirect zero page, Y register offset
    uint8_t REL(); // relative
    uint8_t ZP0(); // zero page
    uint8_t ZPX(); // zero page, X register offset
    uint8_t ZPY(); // zero page, Y register offset

    // opcodes (56)

    uint8_t ADC(); // addition with carry bit
    uint8_t AND(); // bitwise logic AND between the accumulator register and the fetched data
    uint8_t ASL(); // arithmetic shift left: C <- [76543210] <- 0
    uint8_t BCC(); // branch if the carry bit is clear
    uint8_t BCS(); // branch if the carry bit is set
    uint8_t BEQ(); // branch if equal
    uint8_t BIT(); // bit test: A & memory
    uint8_t BMI(); // branch if minus (i.e. negative)
    uint8_t BNE(); // branch if not equal
    uint8_t BPL(); // branch if positive
    uint8_t BRK(); // break (software IRQ)
    uint8_t BVC(); // branch if overflow
    uint8_t BVS(); // branch if not overflow
    uint8_t CLC(); // clear the carry bit
    uint8_t CLD(); // clear the decimal mode bit
    uint8_t CLI(); // clear the interrupt flag
    uint8_t CLV(); // clear the overflow bit
    uint8_t CMP(); // compare the accumulator register
    uint8_t CPX(); // compare the X register
    uint8_t CPY(); // compare the Y register
    uint8_t DEC(); // decrement value at memory location
    uint8_t DEX(); // decrement the X register
    uint8_t DEY(); // decrement the Y register
    uint8_t EOR(); // bitwise XOR: A = A ^ memory
    uint8_t INC(); // increment value at memory location
    uint8_t INX(); // increment the X register
    uint8_t INY(); // increment the Y register
    uint8_t JMP(); // jump to memory location
    uint8_t JSR(); // jump to subroutine
    uint8_t LDA(); // load the accumulator register
    uint8_t LDX(); // load the X register
    uint8_t LDY(); // load the Y register
    uint8_t LSR(); // logical shift right: 0 -> [76543210] -> C
    uint8_t NOP(); // no operation
    uint8_t ORA(); // bitwise OR: A = A | memory
    uint8_t PHA(); // push the accumulator register to the stack
    uint8_t PHP(); // push the status register to the stack
    uint8_t PLA(); // pop a value from the stack and into the accumulator register
    uint8_t PLP(); // pop a value from the stack and into the status register
    uint8_t ROL(); // rotate left: C <- [76543210] <- C
    uint8_t ROR(); // rotate right: C -> [76543210] -> C
    uint8_t RTI(); // return from interrupt
    uint8_t RTS(); // return from subroutine
    uint8_t SBC(); // subtraction with carry bit
    uint8_t SEC(); // set carry flag
    uint8_t SED(); // set decimal mode flag
    uint8_t SEI(); // set interrupt flag
    uint8_t STA(); // store the accumulator register in memory
    uint8_t STX(); // store the X register in memory
    uint8_t STY(); // store the Y register in memory
    uint8_t TAX(); // transfer the accumulator register to the X register
    uint8_t TAY(); // transfer the accumulator register to the Y register
    uint8_t TSX(); // transfer the stack pointer to the X register
    uint8_t TXA(); // transfer the X register to the accumulator register
    uint8_t TXS(); // transfer the X register to the stack pointer
    uint8_t TYA(); // transfer the Y register to the accumulator register

    uint8_t XXX(); // illegal opcode fallback

    // signals

    void Clock();
    void Reset(); // configure the CPU to a known state
    void IRQ();   // interrupt request
    void NMI();   // non-maskable interrupt request

    uint8_t Fetch();
    uint8_t mFetch;

    uint16_t mAddrABS; // absolute address
    uint16_t mAddrREL; // relative address
    uint8_t mOpcode;   // current opcode
    uint8_t mCycles;   // number of cycles left for the duration of the current instruction

private:
    Bus *mBus;

    // methods to access the BUS
    void Write(uint16_t addr, uint8_t data);
    uint8_t Read(uint16_t addr);

    // methods to access the status register
    uint8_t GetFlag(FLAGS flag) const;
    void SetFlag(FLAGS flag, bool enable);

    struct Instruction
    {
        std::string name;
        uint8_t (CPU::*operate)(void) = nullptr;
        uint8_t (CPU::*addrmode)(void) = nullptr;
        uint8_t cycles = 0;
    };

    std::vector<Instruction> mLookup;

    // helpers

    uint8_t BranchIf(const bool cond);
    uint8_t AddValue(const uint16_t value);
    void JumpAddrABS(const uint16_t addr);
    void PushValue(const uint8_t value);
    uint8_t PopValue();
    void CallInterrupt(const uint16_t routine);
};