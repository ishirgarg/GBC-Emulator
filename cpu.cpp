#include "cpu.hpp"
#include "memory.hpp"
#include "registerFile.hpp"

bool CPU::is8BitCarryAdd(int8_t num1, int8_t num2) {

}

bool CPU::is16BitCarryAdd(int16_t num1, int16_t num2) {

}

bool CPU::isHalfCarryAdd(int8_t num1, int8_t num2) {

}

void CPU::enableInterrupts(bool enable) {

}

void CPU::stallUntilInterrupt() {

}

void CPU::stallUntilButtonPress() {

}

/*
Executes opcode

Opcode summary:
Normal: (238 instructions)
- 85 8-BIT LOAD instructions 
- 15 16-BIT LOAD instructions 
- 88 8-BIT ALU instructions 
- 13 16-BIT ALU instructions 
- 9 MISC instructions
- 4 ROTATE/SHIFT instructions 
- 16 JMP instructions
- 8 RESTART instructions
- 6 RETURN instructions

CB prefixed: (256 instructions)
- 8 MISC instructions
- 56 ROTATION instructions
- 192 BIT instructions
*/
void CPU::executeOpcode(uint8_t opcode) {
    uint8_t bits76 = opcode >> 6;
    uint8_t bits543 = (opcode >> 3) & 0x111;
    uint8_t bits210 = opcode & 0x111;

    switch (opcode) {
        // 8-BIT LOAD INSTRUCTIONS (85 instructions)
        // LD rd, n : 2 M-cycles : 7 instructions
        case 0x06: case 0x0E: case 0x16: case 0x1E: case 0x26: case 0x2E: case 0x3E:
            int8_t imm = memory->readByteAtPC();
            registerFile->incrementPC();

            registerFile->writeRegister(bits543, imm);
            break;

        // LD r1, r2 : 1 M-cycle : 49 instructions
        case 0x7F: case 0x78: case 0x79: case 0x7A: case 0x7B: case 0x7C: case 0x7D:
        case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x47: case 0x48: case 0x49: case 0x4A: case 0x4B: case 0x4C: case 0x4D: case 0x4F:
        case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x57: case 0x58: case 0x59: case 0x5A: case 0x5B: case 0x5C: case 0x5D: case 0x5F:
        case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x67: case 0x68: case 0x69: case 0x6A: case 0x6B: case 0x6C: case 0x6D: case 0x6F:
            registerFile->writeRegister(bits543, registerFile->readRegister(bits210));
            break;

        // LD r1, (HL) : 2 M-cycles : 7 instructions
        case 0x7E: case 0x46: case 0x4E: case 0x56: case 0x5E: case 0x66: case 0x6E: 
            registerFile->writeRegister(bits543, memory->readMemory(registerFile->readHL()));
            break;

        // LD (HL), r1 : 2 M-cycles : 7 instructions
        case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x77:
            memory->writeMemory(registerFile->readHL(), registerFile->readRegister(bits210));
            break;

        // LD (HL), n : 3 M-cycles : 1 instruction
        case 0x36:
            int8_t imm = memory->readByteAtPC();
            registerFile->incrementPC();

            memory->writeMemory(registerFile->readHL(), imm);
            break;

        // LD A (BC) : 2 M-cycles : 1 instruction
        case 0x0A:
            registerFile->writeRegister(7, registerFile->readBC());
            break;
        
        // LD A (DE) : 2 M-cycles : 1 instruction
        case 0x1A:
            registerFile->writeRegister(7, registerFile->readDE());
            break;

        // LD A (nn) : 4 M-cycles : 1 instruction
        case 0xFA:
            int8_t immLSB = memory->readByteAtPC();
            registerFile->incrementPC();
            int8_t immMSB = memory->readByteAtPC();
            registerFile->incrementPC();
            
            registerFile->writeRegister(7, mergeBytes(immMSB, immLSB));
            break;
        
        // LD (BC), A : 2 M-cycles : 1 instruction
        case 0x02:
            memory->writeMemory(registerFile->readBC(), registerFile->readRegister(7));
            break;

        // LD (DE), A : 2 M-cycles : 1 instruction
        case 0x12:
            memory->writeMemory(registerFile->readDE(), registerFile->readRegister(7));
            break;

        // LD (nn), A : 4 M-cycles : 1 instruction
        case 0xEA:
            int8_t immLSB = memory->readByteAtPC();
            registerFile->incrementPC();
            int8_t immMSB = memory->readByteAtPC();
            registerFile->incrementPC();
            
            memory->writeMemory(mergeBytes(immMSB, immLSB), registerFile->readRegister(7));
            break;

        // LD A, (C) : 2 M-cycles : 1 instruction
        case 0xF2:
            registerFile->writeRegister(7, memory->readMemory(registerFile->readRegister(1)));
            break;

        // LD (C), A : 2 M-cycles : 1 instruction
        case 0xE2:
            memory->writeMemory(registerFile->readRegister(1), registerFile->readRegister(7));
            break;

        // LDD A, (HL) OR LD A, (HLD) OR LD A, (HL-) : 2 M-cycles : 1 instruction
        case 0x3A:
            registerFile->writeRegister(7, memory->readMemory(registerFile->readHL()));
            registerFile->incrementHL(-1);
            break;

        // LD (HLD), A OR LD (HL-), A OR LDD (HL), A : 2 M-cycles : 1 instruction
        case 0x32:
            memory->writeMemory(registerFile->readHL(), registerFile->readRegister(7));
            registerFile->incrementHL(-1);
            break;

        // LDI A, (HL) OR LD A, (HLI) OR LD A, (HL+) : 2 M-cycles : 1 instruction
        case 0x2A:
            registerFile->writeRegister(7, memory->readMemory(registerFile->readHL()));
            registerFile->incrementHL(1);
            break;

        // LD (HLI), A OR LD (HL+), A OR LDI (HL), A : 2 M-cycles : 1 instruction
        case 0x22:
            memory->writeMemory(registerFile->readHL(), registerFile->readRegister(7));
            registerFile->incrementHL(1);
            break;

        // LDH (n), A : 3 M-cycles : 1 instruction
        case 0xE0:
            int8_t imm = memory->readByteAtPC();
            registerFile->incrementPC();

            memory->writeMemory(0xFF00 + (uint16_t) imm, registerFile->readRegister(7));
            break;

        // LDH A, (n) : 3 M-cycles : 1 instruction
        case 0xF0:
            uint8_t imm = memory->readByteAtPC();
            registerFile->incrementPC();

            registerFile->writeRegister(7, memory->readMemory(0xFF00 + (uint16_t) imm));
            break;

        // 16-BIT LOAD INSTRUCTIONS (15 instructions)

        // LD BC, nn : 3 M-cycles : 1 instruction
        case 0x01:
            int8_t immLSB = memory->readByteAtPC();
            registerFile->incrementPC();
            int8_t immMSB = memory->readByteAtPC();
            registerFile->incrementPC();

            registerFile->writeBC(mergeBytes(immMSB, immLSB));
            break;
        
        // LD DE, nn : 3 M-cycles : 1 instruction
        case 0x11:
            int8_t immLSB = memory->readByteAtPC();
            registerFile->incrementPC();
            int8_t immMSB = memory->readByteAtPC();
            registerFile->incrementPC();

            registerFile->writeDE(mergeBytes(immMSB, immLSB));
            break;

        // LD HL, nn : 3 M-cycles : 1 instruction
        case 0x21:
            int8_t immLSB = memory->readByteAtPC();
            registerFile->incrementPC();
            int8_t immMSB = memory->readByteAtPC();
            registerFile->incrementPC();

            registerFile->writeHL(mergeBytes(immMSB, immLSB));
            break;

        // LD SP, nn : 3 M-cycles : 1 instruction
        case 0x31:
            int8_t immLSB = memory->readByteAtPC();
            registerFile->incrementPC();
            int8_t immMSB = memory->readByteAtPC();
            registerFile->incrementPC();

            registerFile->writeSP(mergeBytes(immMSB, immLSB));
            break;

        // LD SP, HL : 2 M-cycles : 1 instruction
        case 0xF9:
            registerFile->writeSP(registerFile->readHL());
            break;

        // LD HL, SP+n OR LDHL SP, n : 3 M-cycles : 1 instruction
        case 0xF8:
            int8_t imm = memory->readByteAtPC();
            registerFile->incrementPC();

            registerFile->setFlagZ(false);
            registerFile->setFlagN(false);
            registerFile->setFlagH(isHalfCarryAdd((int8_t) registerFile->readSP(), imm));
            registerFile->setFlagC(is8BitCarryAdd((int8_t) registerFile->readSP(), imm));

            registerFile->writeHL(registerFile->readSP());
            break;

        // LD (nn), SP : 5 M-cycles : 1 instruction
        case 0x08:
            int8_t immLSB = memory->readByteAtPC();
            registerFile->incrementPC();
            int8_t immMSB = memory->readByteAtPC();
            registerFile->incrementPC();

            memory->writeMemory(mergeBytes(immMSB, immLSB), registerFile->readSP());
            break;

        // PUSH AF : 4 M-cycles : 1 instruction
        case 0xF5:
            memory->pushToStack(registerFile->readRegister(7));
            memory->pushToStack(registerFile->readRegister(6));
            break;

        // PUSH BC : 4 M-cycles : 1 instruction
        case 0xC5:
            memory->pushToStack(registerFile->readRegister(0));
            memory->pushToStack(registerFile->readRegister(1));
            break;

        // PUSH DE : 4 M-cycles : 1 instruction
        case 0xD5:
            memory->pushToStack(registerFile->readRegister(2));
            memory->pushToStack(registerFile->readRegister(3));
            break;

        // PUSH HL : 4 M-cycles : 1 instruction
        case 0xE5:
            memory->pushToStack(registerFile->readRegister(4));
            memory->pushToStack(registerFile->readRegister(5));
            break;

        // POP AF : 3 M-cycles : 1 instruction
        case 0xF1:
            int8_t immMSB = memory->popFromStack();
            int8_t immLSB = memory->popFromStack();
            registerFile->writeAF(mergeBytes(immMSB, immLSB));
            break;

        // POP BC : 3 M-cycles : 1 instruction
        case 0xC1:
            int8_t immMSB = memory->popFromStack();
            int8_t immLSB = memory->popFromStack();
            registerFile->writeBC(mergeBytes(immMSB, immLSB));
            break;

        // POP DE : 3 M-cycles : 1 instruction
        case 0xD1:
            int8_t immMSB = memory->popFromStack();
            int8_t immLSB = memory->popFromStack();
            registerFile->writeDE(mergeBytes(immMSB, immLSB));
            break;

        // POP HL : 3 M-cycles : 1 instruction
        case 0xE1:
            int8_t immMSB = memory->popFromStack();
            int8_t immLSB = memory->popFromStack();
            registerFile->writeHL(mergeBytes(immMSB, immLSB));
            break;

        // 8-BIT ALU INSTRUCTIONS (88 instructions)

        // ADD A, r2 : 1 M-cycle : 7 instructions
        case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x87:
            int8_t reg1 = registerFile->readRegister(7);
            int8_t reg2 = registerFile->readRegister(bits210);

            registerFile->setFlagZ((reg1 + reg2) == 0);
            registerFile->setFlagN(false);
            registerFile->setFlagC(is8BitCarryAdd(reg1, reg2));
            registerFile->setFlagH(isHalfCarryAdd(reg1, reg2));

            registerFile->writeRegister(7, reg1 + reg2);
            break;

        // ADD A, (HL) : 2 M-cycles : 1 instruction
        case 0x86:
            int8_t reg1 = registerFile->readRegister(7);
            int8_t data = memory->readMemory(registerFile->readHL());

            registerFile->setFlagZ((reg1 + data) == 0);
            registerFile->setFlagN(false);
            registerFile->setFlagC(is8BitCarryAdd(reg1, data));
            registerFile->setFlagH(isHalfCarryAdd(reg1, data));

            registerFile->writeRegister(7, reg1 + data);
            break;

        // ADD A, n : 2 M-cycles : 1 instruction
        case 0xC6:
            int8_t reg1 = registerFile->readRegister(7);
            int8_t imm = memory->readByteAtPC();
            registerFile->incrementPC();

            registerFile->setFlagZ((reg1 + imm) == 0);
            registerFile->setFlagN(false);
            registerFile->setFlagC(is8BitCarryAdd(reg1, imm));
            registerFile->setFlagH(isHalfCarryAdd(reg1, imm));

            registerFile->writeRegister(7, reg1 + imm);
            break;

        // ADC A, r2 : 1 M-cycle : 7 instructions
        case 0x8F: case 0x88: case 0x89: case 0x8A: case 0x8B: case 0x8C: case 0x8D:
            int8_t reg1 = registerFile->readRegister(7);
            int8_t reg2Carry = registerFile->readRegister(bits210) + registerFile->readFlagC();

            registerFile->setFlagZ((reg1 + reg2Carry) == 0);
            registerFile->setFlagN(false);
            registerFile->setFlagC(is8BitCarryAdd(reg1, reg2Carry));
            registerFile->setFlagH(isHalfCarryAdd(reg1, reg2Carry));

            registerFile->writeRegister(7, reg1 + reg2Carry);
            break;

        // ADC A, (HL) : 2 M-cycles : 1 instruction
        case 0x8E:
            int8_t reg1 = registerFile->readRegister(7);
            int8_t dataCarry = memory->readMemory(registerFile->readHL()) + registerFile->readFlagC();

            registerFile->setFlagZ((reg1 + dataCarry) == 0);
            registerFile->setFlagN(false);
            registerFile->setFlagC(is8BitCarryAdd(reg1, dataCarry));
            registerFile->setFlagH(isHalfCarryAdd(reg1, dataCarry));

            registerFile->writeRegister(7, reg1 + dataCarry);
            break;

        // ADC A, n : 2 M-cycles : 1 instruction
        case 0xCE:
            int8_t reg1 = registerFile->readRegister(7);
            int8_t immCarry = memory->readByteAtPC() + registerFile->readFlagC();
            registerFile->incrementPC();

            registerFile->setFlagZ((reg1 + immCarry) == 0);
            registerFile->setFlagN(false);
            registerFile->setFlagC(is8BitCarryAdd(reg1, immCarry));
            registerFile->setFlagH(isHalfCarryAdd(reg1, immCarry));

            registerFile->writeRegister(7, reg1 + immCarry);
            break;

        // SUB A, r2 : 1 M-cycle : 7 instructions
        case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x97:
            int8_t reg1 = registerFile->readRegister(7);
            int8_t reg2 = registerFile->readRegister(bits210);

            registerFile->setFlagZ((reg1 - reg2) == 0);
            registerFile->setFlagN(true);
            registerFile->setFlagC(!is8BitCarrySub(reg1, reg2));
            registerFile->setFlagH(!isHalfCarrySub(reg1, reg2));

            registerFile->writeRegister(7, reg1 - reg2);
            break;

        // SUB A, (HL) : 2 M-cycles : 1 instruction
        case 0x96:
            int8_t reg1 = registerFile->readRegister(7);
            int8_t data = memory->readMemory(registerFile->readHL());

            registerFile->setFlagZ((reg1 - data) == 0);
            registerFile->setFlagN(true);
            registerFile->setFlagC(!is8BitCarrySub(reg1, data));
            registerFile->setFlagH(!isHalfCarrySub(reg1, data));

            registerFile->writeRegister(7, reg1 - data);
            break;

        // SUB A, n : 2 M-cycles : 1 instruction
        case 0xD6:
            int8_t reg1 = registerFile->readRegister(7);
            int8_t imm = memory->readByteAtPC();
            registerFile->incrementPC();

            registerFile->setFlagZ((reg1 - imm) == 0);
            registerFile->setFlagN(true);
            registerFile->setFlagC(!is8BitCarrySub(reg1, imm));
            registerFile->setFlagH(!isHalfCarrySub(reg1, imm));

            registerFile->writeRegister(7, reg1 - imm);
            break;

        // SBC A, r2 : 1 M-cycle : 7 instructions
        case 0x98: case 0x99: case 0x9A: case 0x9B: case 0x9C: case 0x9D: case 0x9F:
            int8_t reg1 = registerFile->readRegister(7);
            int8_t reg2Carry = registerFile->readRegister(bits210) + registerFile->readFlagC();

            registerFile->setFlagZ((reg1 - reg2Carry) == 0);
            registerFile->setFlagN(true);
            registerFile->setFlagC(!is8BitCarrySub(reg1, reg2Carry));
            registerFile->setFlagH(!isHalfCarrySub(reg1, reg2Carry));

            registerFile->writeRegister(7, reg1 - reg2Carry);
            break;

        // SBC A, (HL) : 2 M-cycles : 1 instruction
        case 0x9E:
            int8_t reg1 = registerFile->readRegister(7);
            int8_t dataCarry = memory->readMemory(registerFile->readHL()) + registerFile->readFlagC();

            registerFile->setFlagZ((reg1 - dataCarry) == 0);
            registerFile->setFlagN(true);
            registerFile->setFlagC(!is8BitCarrySub(reg1, dataCarry));
            registerFile->setFlagH(!isHalfCarrySub(reg1, dataCarry));

            registerFile->writeRegister(7, reg1 - dataCarry);
            break;

        // SBC A, n : 2 M-cycles : 1 instruction
        case 0xDE:
            int8_t reg1 = registerFile->readRegister(7);
            int8_t immCarry = memory->readByteAtPC() + registerFile->readFlagC();
            registerFile->incrementPC();

            registerFile->setFlagZ((reg1 - immCarry) == 0);
            registerFile->setFlagN(true);
            registerFile->setFlagC(!is8BitCarrySub(reg1, immCarry));
            registerFile->setFlagH(!isHalfCarrySub(reg1, immCarry));

            registerFile->writeRegister(7, reg1 - immCarry);
            break;

        // AND r2 : 1 M-cycle : 7 instruction
        case 0xA0: case 0xA1: case 0xA2: case 0xA3: case 0xA4: case 0xA5: case 0xA7:
            int8_t result = registerFile->readRegister(7) & registerFile->readRegister(bits210);

            registerFile->setFlagZ(result == 0);
            registerFile->setFlagN(false);
            registerFile->setFlagC(false);
            registerFile->setFlagH(true);

            registerFile->writeRegister(7, result);
            break;

        // AND (HL) : 2 M-cycle : 1 instruction
        case 0xA6:
            int8_t result = registerFile->readRegister(7) & memory->readMemory(registerFile->readHL());

            registerFile->setFlagZ(result == 0);
            registerFile->setFlagN(false);
            registerFile->setFlagC(false);
            registerFile->setFlagH(true);

            registerFile->writeRegister(7, result);
            break;

        // AND n : 2 M-cycle : 1 instruction
        case 0xE6:
            int8_t imm = memory->readByteAtPC();
            registerFile->incrementPC();
            int8_t result = registerFile->readRegister(7) & imm;

            registerFile->setFlagZ(result == 0);
            registerFile->setFlagN(false);
            registerFile->setFlagC(false);
            registerFile->setFlagH(true);

            registerFile->writeRegister(7, result);
            break;

        // OR r2 : 1 M-cycle : 7 instruction
        case 0xB0: case 0xB1: case 0xB2: case 0xB3: case 0xB4: case 0xB5: case 0xB7:
            int8_t result = registerFile->readRegister(7) | registerFile->readRegister(bits210);

            registerFile->setFlagZ(result == 0);
            registerFile->setFlagN(false);
            registerFile->setFlagC(false);
            registerFile->setFlagH(false);

            registerFile->writeRegister(7, result);
            break;

        // OR (HL) : 2 M-cycle : 1 instruction
        case 0xB6:
            int8_t result = registerFile->readRegister(7) | memory->readMemory(registerFile->readHL());

            registerFile->setFlagZ(result == 0);
            registerFile->setFlagN(false);
            registerFile->setFlagC(false);
            registerFile->setFlagH(false);

            registerFile->writeRegister(7, result);
            break;

        // OR n : 2 M-cycle : 1 instruction
        case 0xF6:
            int8_t imm = memory->readByteAtPC();
            registerFile->incrementPC();
            int8_t result = registerFile->readRegister(7) | imm;

            registerFile->setFlagZ(result == 0);
            registerFile->setFlagN(false);
            registerFile->setFlagC(false);
            registerFile->setFlagH(false);

            registerFile->writeRegister(7, result);
            break;

        // XOR r2 : 1 M-cycle : 7 instruction
        case 0xAF: case 0xA8: case 0xA9: case 0xAA: case 0xAB: case 0xAC: case 0xAD:
            int8_t result = registerFile->readRegister(7) ^ registerFile->readRegister(bits210);

            registerFile->setFlagZ(result == 0);
            registerFile->setFlagN(false);
            registerFile->setFlagC(false);
            registerFile->setFlagH(false);

            registerFile->writeRegister(7, result);
            break;

        // XOR (HL) : 2 M-cycle : 1 instruction
        case 0xAE:
            int8_t result = registerFile->readRegister(7) ^ memory->readMemory(registerFile->readHL());

            registerFile->setFlagZ(result == 0);
            registerFile->setFlagN(false);
            registerFile->setFlagC(false);
            registerFile->setFlagH(false);

            registerFile->writeRegister(7, result);
            break;

        // XOR n : 2 M-cycle : 1 instruction
        case 0xEE:
            int8_t imm = memory->readByteAtPC();
            registerFile->incrementPC();
            int8_t result = registerFile->readRegister(7) ^ imm;

            registerFile->setFlagZ(result == 0);
            registerFile->setFlagN(false);
            registerFile->setFlagC(false);
            registerFile->setFlagH(false);

            registerFile->writeRegister(7, result);
            break;

        // CP A, r2 : 1 M-cycle : 7 instructions
        case 0xB8: case 0xB9: case 0xBA: case 0xBB: case 0xBC: case 0xBD: case 0xBF:
            int8_t reg1 = registerFile->readRegister(7);
            int8_t reg2 = registerFile->readRegister(bits210);

            registerFile->setFlagZ((reg1 - reg2) == 0);
            registerFile->setFlagN(true);
            registerFile->setFlagC(!is8BitCarrySub(reg1, reg2));
            registerFile->setFlagH(!isHalfCarrySub(reg1, reg2));
            break;

        // CP A, (HL) : 2 M-cycles : 1 instruction
        case 0xBE:
            int8_t reg1 = registerFile->readRegister(7);
            int8_t data = memory->readMemory(registerFile->readHL());

            registerFile->setFlagZ((reg1 - data) == 0);
            registerFile->setFlagN(true);
            registerFile->setFlagC(!is8BitCarrySub(reg1, data));
            registerFile->setFlagH(!isHalfCarrySub(reg1, data));
            break;

        // CP A, n : 2 M-cycles : 1 instruction
        case 0xFE:
            int8_t reg1 = registerFile->readRegister(7);
            int8_t imm = memory->readByteAtPC();
            registerFile->incrementPC();

            registerFile->setFlagZ((reg1 - imm) == 0);
            registerFile->setFlagN(true);
            registerFile->setFlagC(!is8BitCarrySub(reg1, imm));
            registerFile->setFlagH(!isHalfCarrySub(reg1, imm));
            break;

        // INC r1 : 1 M-cycle : 7 instruction 
        case 0x3C: case 0x04: case 0x0C: case 0x14: case 0x1C: case 0x24: case 0x2C:
            int8_t reg = registerFile->readRegister(bits543);
            int8_t result = reg + 1;

            registerFile->setFlagZ(result == 0);
            registerFile->setFlagN(false);
            registerFile->setFlagH(isHalfCarryAdd(reg, 1));

            registerFile->writeRegister(bits543, result);
            break;

        // INC (HL) : 3 M-cycle : 1 instruction 
        case 0x34:
            int8_t data = memory->readMemory(registerFile->readHL());
            int8_t result = data + 1;

            registerFile->setFlagZ(result == 0);
            registerFile->setFlagN(false);
            registerFile->setFlagH(isHalfCarryAdd(data, 1));

            memory->writeMemory(registerFile->readHL(), result);
            break;

        // DEC r1 : 1 M-cycle : 7 instruction 
        case 0x3D: case 0x05: case 0x0D: case 0x15: case 0x1D: case 0x25: case 0x2D:
            int8_t reg = registerFile->readRegister(bits543);
            int8_t result = reg - 1;

            registerFile->setFlagZ(result == 0);
            registerFile->setFlagN(true);
            registerFile->setFlagH(!isHalfCarrySub(reg, 1));

            registerFile->writeRegister(bits543, result);
            break;

        // DEC (HL) : 3 M-cycle : 1 instruction 
        case 0x35:
            int8_t data = memory->readMemory(registerFile->readHL());
            int8_t result = data - 1;

            registerFile->setFlagZ(result == 0);
            registerFile->setFlagN(true);
            registerFile->setFlagH(!isHalfCarrySub(data, 1));

            memory->writeMemory(registerFile->readHL(), result);
            break;

        // 16-BIT ALU INSTRUCTIONS (13 instructions)

        // ADD HL BC : 2 M-cycles : 1 instruction
        case 0x09:
            int16_t reg1 = registerFile->readHL();
            int16_t reg2 = registerFile->readBC();

            registerFile->setFlagN(false);
            registerFile->setFlagC(is16BitCarryAdd(reg1, reg2));
            registerFile->setFlagH(isUpperHalfCarryAdd(reg1, reg2));

            registerFile->writeHL(reg1 + reg2);
            break;

        // ADD HL DE : 2 M-cycles : 1 instruction
        case 0x19:
            int16_t reg1 = registerFile->readHL();
            int16_t reg2 = registerFile->readDE();

            registerFile->setFlagN(false);
            registerFile->setFlagC(is16BitCarryAdd(reg1, reg2));
            registerFile->setFlagH(isUpperHalfCarryAdd(reg1, reg2));

            registerFile->writeHL(reg1 + reg2);
            break;

        // ADD HL HL : 2 M-cycles : 1 instruction
        case 0x29:
            int16_t reg1 = registerFile->readHL();
            int16_t reg2 = registerFile->readHL();

            registerFile->setFlagN(false);
            registerFile->setFlagC(is16BitCarryAdd(reg1, reg2));
            registerFile->setFlagH(isUpperHalfCarryAdd(reg1, reg2));

            registerFile->writeHL(reg1 + reg2);
            break;

        // ADD HL SP : 2 M-cycles : 1 instruction
        case 0x39:
            int16_t reg1 = registerFile->readHL();
            int16_t reg2 = registerFile->readSP();

            registerFile->setFlagN(false);
            registerFile->setFlagC(is16BitCarryAdd(reg1, reg2));
            registerFile->setFlagH(isUpperHalfCarryAdd(reg1, reg2));

            registerFile->writeHL(reg1 + reg2);
            break;

        // ADD SP, n : 4 M-cycles : 1 instruction
        case 0xE8:
            uint16_t sp = registerFile->readSP();
            int8_t imm = memory->readByteAtPC();
            registerFile->incrementPC();

            registerFile->setFlagZ(false);
            registerFile->setFlagN(false);
            registerFile->setFlagH(isHalfCarryAdd((int8_t) registerFile->readSP(), imm));
            registerFile->setFlagC(is8BitCarryAdd((int8_t) registerFile->readSP(), imm));

            registerFile->writeSP(sp + (int16_t) imm);
            break;

        // INC BC : 2 M-cycles : 1 instruction
        case 0x03:
            registerFile->writeBC(registerFile->readBC() + 1);
            break;

        // INC DE : 2 M-cycles : 1 instruction
        case 0x13:
            registerFile->writeDE(registerFile->readDE() + 1);
            break;

        // INC HL : 2 M-cycles : 1 instruction
        case 0x23:
            registerFile->writeHL(registerFile->readHL() + 1);
            break;

        // INC SP : 2 M-cycles : 1 instruction
        case 0x33:
            registerFile->writeSP(registerFile->readSP() + 1);
            break;

        // DEC BC : 2 M-cycles : 1 instruction
        case 0x0B:
            registerFile->writeBC(registerFile->readBC() - 1);
            break;

        // DEC DE : 2 M-cycles : 1 instruction
        case 0x1B:
            registerFile->writeDE(registerFile->readDE() - 1);
            break;

        // DEC HL : 2 M-cycles : 1 instruction
        case 0x2B:
            registerFile->writeHL(registerFile->readHL() - 1);
            break;

        // DEC SP : 2 M-cycles : 1 instruction
        case 0x3B:
            registerFile->writeSP(registerFile->readSP() - 1);
            break;

        // MISC INSTRUCTIONS: NOT 0xCB PREFIXED (9 instructions)

        // DAA : 1 M-cycle : 1 instruction
        case 0x27:
            int8_t result = registerFile->readRegister(7);
            if (!registerFile->readFlagN()) {
                if (registerFile->readFlagC() || result > 0x99) {
                    result += 0x60;
                    registerFile->setFlagC(true);
                }
                if (registerFile->readFlagH()) {
                    result += 0x6;
                }
            }
            else {
                if (registerFile->readFlagC()) {
                    result -= 0x60;
                }
                if (registerFile->readFlagH()) {
                    result -= 0x6;
                }
            }

            registerFile->setFlagZ(result == 0); // the usual z flag
            registerFile->setFlagH(false); // h flag is always cleared

            registerFile->writeRegister(7, result);
            break;

        // CPL : 1 M-cycle : 1 instruction
        case 0x2F:
            int8_t regA = registerFile->readRegister(7);

            registerFile->setFlagN(true);
            registerFile->setFlagH(true);

            registerFile->writeRegister(7, ~regA);
            break;

        // CCF : 1 M-cycle : 1 instruction
        case 0x3F:
            int8_t carryFlag = registerFile->readFlagC();
            registerFile->setFlagC(carryFlag ? false : true);

            registerFile->setFlagN(false);
            registerFile->setFlagH(false);
            break;

        // SCF : 1 M-cycle : 1 instruction
        case 0x37:
            registerFile->setFlagC(true);
            registerFile->setFlagN(false);
            registerFile->setFlagH(false);
            break;

        // NOP : 1 M-cycle : 1 instruction
        case 0x00:
            break;

        // HALT: 1 M-cycle : 1 instruction
        case 0x76:
            stallUntilInterrupt();
            break;

        // STOP: 1 M-cycle : 1 instruction
        case 0x10:
            stallUntilButtonPress();
            display->stallUntilButtonPress();
            break;

        // DI : 1 M-cycle : 1 instruction
        case 0xF3:
            enableInterrupts(false);
            break;

        // EI : 1 M-cycle : 1 instruction
        case 0xFB:
            enableInterrupts(true);
            break;

        // ROTATES AND SHIFT INSTRUCTIONS (4 instructions)

        // RLCA : 1 M-cycle : 1 instruction
        case 0x07:
            int8_t regA = registerFile->readRegister(7);
            int8_t result = (regA << 1) | ((uint8_t) regA >> 7);

            registerFile->setFlagZ(result == 0);
            registerFile->setFlagN(false);
            registerFile->setFlagH(false);
            registerFile->setFlagC((uint8_t) regA >> 7);

            registerFile->writeRegister(7, result);
            break;

        // RLA : 1 M-cycle : 1 instruction
        case 0x17:
            int8_t regA = registerFile->readRegister(7);
            int8_t carryFlag = registerFile->readFlagC();
            int8_t result = (regA << 1) | carryFlag;

            registerFile->setFlagZ(result == 0);
            registerFile->setFlagN(false);
            registerFile->setFlagH(false);
            registerFile->setFlagC((uint8_t) regA >> 7);

            registerFile->writeRegister(7, result);
            break;

        // RRCA : 1 M-cycle : 1 instruction
        case 0x0F:
            int8_t regA = registerFile->readRegister(7);
            int8_t result = ((uint8_t) regA >> 1) | (regA << 7);

            registerFile->setFlagZ(result == 0);
            registerFile->setFlagN(false);
            registerFile->setFlagH(false);
            registerFile->setFlagC(regA & 0x01);

            registerFile->writeRegister(7, result);
            break;

        // RRA : 1 M-cycle : 1 instruction
        case 0x1F:
            int8_t regA = registerFile->readRegister(7);
            int8_t carryFlag = registerFile->readFlagC();
            int8_t result = ((uint8_t) regA >> 1) | (carryFlag << 7);

            registerFile->setFlagZ(result == 0);
            registerFile->setFlagN(false);
            registerFile->setFlagH(false);
            registerFile->setFlagC(regA & 0x01);

            registerFile->writeRegister(7, result);
            break;
            
        // JUMP INSTRUCTIONS (16 instructions)

        // JP nn : 3 M-cycles : 1 instruction
        case 0xC3:
            int8_t immLSB = memory->readByteAtPC();
            registerFile->incrementPC();
            int8_t immMSB = memory->readByteAtPC();
            registerFile->incrementPC();

            registerFile->writePC(mergeBytes(immMSB, immLSB));
            break;
        
        // JP NZ, nn : 3 M-cycles : 1 instruction
        case 0xC2:
            int8_t immLSB = memory->readByteAtPC();
            registerFile->incrementPC();
            int8_t immMSB = memory->readByteAtPC();
            registerFile->incrementPC();

            if (!registerFile->readFlagZ()) {
                registerFile->writePC(mergeBytes(immMSB, immLSB));
            }
            break;

        // JP Z, nn : 3 M-cycles : 1 instruction
        case 0xCA:
            int8_t immLSB = memory->readByteAtPC();
            registerFile->incrementPC();
            int8_t immMSB = memory->readByteAtPC();
            registerFile->incrementPC();

            if (registerFile->readFlagZ()) {
                registerFile->writePC(mergeBytes(immMSB, immLSB));
            }
            break;

        // JP NC, nn : 3 M-cycles : 1 instruction
        case 0xD2:
            int8_t immLSB = memory->readByteAtPC();
            registerFile->incrementPC();
            int8_t immMSB = memory->readByteAtPC();
            registerFile->incrementPC();

            if (!registerFile->readFlagC()) {
                registerFile->writePC(mergeBytes(immMSB, immLSB));
            }
            break;

        // JP C, nn : 3 M-cycles : 1 instruction
        case 0xDA:
            int8_t immLSB = memory->readByteAtPC();
            registerFile->incrementPC();
            int8_t immMSB = memory->readByteAtPC();
            registerFile->incrementPC();

            if (registerFile->readFlagC()) {
                registerFile->writePC(mergeBytes(immMSB, immLSB));
            }
            break;

        // JP (HN) : 4 M-cycles : 1 instruction
        case 0xE9:
            registerFile->writePC(memory->readMemory(registerFile->readHL()));
            break;

        // JR n : 2 M-cycles : 1 instruction
        case 0x18:
            int8_t imm = memory->readByteAtPC();
            registerFile->incrementPC();

            registerFile->incrementPC(imm);

        // JR NZ, n : 2 M-cycles : 1 instruction
        case 0x20:
            int8_t imm = memory->readByteAtPC();
            registerFile->incrementPC();

            if (!registerFile->readFlagZ()) {
                registerFile->incrementPC(imm);
            }
            break;

        // JR Z, n : 2 M-cycles : 1 instruction
        case 0x28:
            int8_t imm = memory->readByteAtPC();
            registerFile->incrementPC();

            if (registerFile->readFlagZ()) {
                registerFile->incrementPC(imm);
            }
            break;

        // JR NC, n : 2 M-cycles : 1 instruction
        case 0x30:
            int8_t imm = memory->readByteAtPC();
            registerFile->incrementPC();

            if (!registerFile->readFlagC()) {
                registerFile->incrementPC(imm);
            }
            break;

        // JR C, n : 2 M-cycles : 1 instruction
        case 0x38:
            int8_t imm = memory->readByteAtPC();
            registerFile->incrementPC();

            if (registerFile->readFlagC()) {
                registerFile->incrementPC(imm);
            }
            break;

        // CALL nn : 3 M-cycles : 1 instruction
        case 0xCD:
            int8_t immLSB = memory->readByteAtPC();
            registerFile->incrementPC();
            int8_t immMSB = memory->readByteAtPC();
            registerFile->incrementPC();

            uint16_t PC = registerFile->readPC();
            memory->pushToStack((int8_t) (PC >> 8));
            memory->pushToStack((int8_t) PC);

            registerFile->writePC(mergeBytes(immMSB, immLSB));
            break;

        // CALL NZ, nn : 3 M-cycles : 1 instruction
        case 0xC4:
            int8_t immLSB = memory->readByteAtPC();
            registerFile->incrementPC();
            int8_t immMSB = memory->readByteAtPC();
            registerFile->incrementPC();

            if (!registerFile->readFlagZ()) {
                uint16_t PC = registerFile->readPC();
                memory->pushToStack((int8_t) (PC >> 8));
                memory->pushToStack((int8_t) PC);

                registerFile->writePC(mergeBytes(immMSB, immLSB));
            }
            break;

        // CALL Z, nn : 3 M-cycles : 1 instruction
        case 0xCC:
            int8_t immLSB = memory->readByteAtPC();
            registerFile->incrementPC();
            int8_t immMSB = memory->readByteAtPC();
            registerFile->incrementPC();

            if (registerFile->readFlagZ()) {
                uint16_t PC = registerFile->readPC();
                memory->pushToStack((int8_t) (PC >> 8));
                memory->pushToStack((int8_t) PC);

                registerFile->writePC(mergeBytes(immMSB, immLSB));
            }
            break;

        // CALL NC, nn : 3 M-cycles : 1 instruction
        case 0xD4:
            int8_t immLSB = memory->readByteAtPC();
            registerFile->incrementPC();
            int8_t immMSB = memory->readByteAtPC();
            registerFile->incrementPC();

            if (!registerFile->readFlagC()) {
                uint16_t PC = registerFile->readPC();
                memory->pushToStack((int8_t) (PC >> 8));
                memory->pushToStack((int8_t) PC);

                registerFile->writePC(mergeBytes(immMSB, immLSB));
            }
            break;

        // CALL C, nn : 3 M-cycles : 1 instruction
        case 0xDC:
            int8_t immLSB = memory->readByteAtPC();
            registerFile->incrementPC();
            int8_t immMSB = memory->readByteAtPC();
            registerFile->incrementPC();

            if (!registerFile->readFlagC()) {
                uint16_t PC = registerFile->readPC();
                memory->pushToStack((int8_t) (PC >> 8));
                memory->pushToStack((int8_t) PC);

                registerFile->writePC(mergeBytes(immMSB, immLSB));
            }
            break;

        // RESTART INSTRUCTIONS (8 INSTRUCTIONs)

        // RST (n) : 8 M-cycles : 8 instructions
        case 0xC7: case 0xCF: case 0xD7: case 0xDF: case 0xE7: case 0xEF: case 0xF7: case 0xFF:
            uint16_t PC = registerFile->readPC();
            memory->pushToStack((int8_t) (PC >> 8));
            memory->pushToStack((int8_t) PC);

            registerFile->writePC((uint16_t) opcode - (uint16_t) 0x00C7);
            break;

        // RETURN INSTRUCTIONS (6 instructions)

        // RET : 2 M-cycles : 1 instruction
        case 0xC9:
            int8_t addrLSB = memory->popFromStack();
            int8_t addrMSB = memory->popFromStack();

            registerFile->writePC(mergeBytes(addrMSB, addrLSB));
            break;

        // RET NZ : 2 M-cycles : 1 instruction
        case 0xC0:
            if (!registerFile->readFlagZ()) {
                int8_t addrLSB = memory->popFromStack();
                int8_t addrMSB = memory->popFromStack();

                registerFile->writePC(mergeBytes(addrMSB, addrLSB));
            }
            break;

        // RET Z : 2 M-cycles : 1 instruction
        case 0xC8:
            if (registerFile->readFlagZ()) {
                int8_t addrLSB = memory->popFromStack();
                int8_t addrMSB = memory->popFromStack();

                registerFile->writePC(mergeBytes(addrMSB, addrLSB));
            }
            break;
            
        // RET NC : 2 M-cycles : 1 instruction
        case 0xD0:
            if (!registerFile->readFlagC()) {
                int8_t addrLSB = memory->popFromStack();
                int8_t addrMSB = memory->popFromStack();

                registerFile->writePC(mergeBytes(addrMSB, addrLSB));
            }
            break;
            
        // RET C : 2 M-cycles : 1 instruction
        case 0xD8:
            if (registerFile->readFlagC()) {
                int8_t addrLSB = memory->popFromStack();
                int8_t addrMSB = memory->popFromStack();

                registerFile->writePC(mergeBytes(addrMSB, addrLSB));
            }
            break;

        // RETI : 2 M-cycle : 1 instruction
        case 0xD9:
            int8_t addrLSB = memory->popFromStack();
            int8_t addrMSB = memory->popFromStack();

            registerFile->writePC(mergeBytes(addrMSB, addrLSB));
            enableInterrupts(true);

        // 0xCB prefixed opcodes
        case 0xCB:
            uint8_t opcode = memory->readByteAtPC();
            registerFile->incrementPC();
            uint8_t bits76 = opcode >> 6;
            uint8_t bits543 = (opcode >> 3) & 0x111;
            uint8_t bits210 = opcode & 0x111;

            switch (opcode) {
                // MISC INSTRUCTIONS (8 instructions)

                // SWAP r1 : 2 M-cycles : 7 instruction
                case 0x37: case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35:
                    int8_t data = registerFile->readRegister(bits210);
                    int8_t result = ((uint8_t) data >> 4) | (data << 4);

                    registerFile->setFlagZ(result == 0);
                    registerFile->setFlagN(false);
                    registerFile->setFlagH(false);
                    registerFile->setFlagC(false);

                    registerFile->writeRegister(bits210, result);
                    break;

                // SWAP (HL) : 4 M-cycles : 1 instruction
                case 0x36:
                    uint16_t addr = registerFile->readHL();
                    int8_t data = memory->readMemory(addr);
                    int8_t result = ((uint8_t) data >> 4) | (data << 4);

                    registerFile->setFlagZ(result == 0);
                    registerFile->setFlagN(false);
                    registerFile->setFlagH(false);
                    registerFile->setFlagC(false);

                    memory->writeMemory(addr, result);
                    break;


                // ROTATION INSTRUCTIONS (56 instructions)

                // RLC r1 : 2 M-cycle : 7 instructions
                case 0x07: case 0x00: case 0x01: case 0x02: case 0x03: case 0x04: case 0x05: 
                    int8_t reg = registerFile->readRegister(bits210);
                    int8_t result = (regA << 1) | ((uint8_t) reg >> 7);

                    registerFile->setFlagZ(result == 0);
                    registerFile->setFlagN(false);
                    registerFile->setFlagH(false);
                    registerFile->setFlagC((uint8_t) reg >> 7);

                    registerFile->writeRegister(bits210, result);
                    break;

                // RLC (HL) : 4 M-cycle : 1 instructions
                case 0x06:
                    uint16_t addr = registerFile->readHL();
                    int8_t data = memory->readMemory(addr);

                    int8_t carryFlag = registerFile->readFlagC();
                    int8_t result = (data << 1) | ((uint8_t) data >> 7);

                    registerFile->setFlagZ(result == 0);
                    registerFile->setFlagN(false);
                    registerFile->setFlagH(false);
                    registerFile->setFlagC((uint8_t) data >> 7);

                    memory->writeMemory(addr, result);
                    break;

                // RL r1 : 2 M-cycle : 7 instruction
                case 0x17: case 0x10: case 0x11: case 0x12: case 0x13: case 0x14: case 0x15: 
                    int8_t reg = registerFile->readRegister(bits210);
                    int8_t carryFlag = registerFile->readFlagC();
                    int8_t result = (reg << 1) | carryFlag;

                    registerFile->setFlagZ(result == 0);
                    registerFile->setFlagN(false);
                    registerFile->setFlagH(false);
                    registerFile->setFlagC((uint8_t) reg >> 7);

                    registerFile->writeRegister(bits210, result);
                    break;

                // RL (HL) : 4 M-cycle : 1 instruction
                case 0x16:
                    uint16_t addr = registerFile->readHL();
                    int8_t data = memory->readMemory(addr);

                    int8_t carryFlag = registerFile->readFlagC();
                    int8_t result = (data << 1) | carryFlag;

                    registerFile->setFlagZ(result == 0);
                    registerFile->setFlagN(false);
                    registerFile->setFlagH(false);
                    registerFile->setFlagC((uint8_t) data >> 7);

                    memory->writeMemory(addr, result);
                    break;

                // RRC r1 : 2 M-cycle : 7 instruction
                case 0x0F: case 0x08: case 0x09: case 0x0A: case 0x0B: case 0x0C: case 0x0D: 
                    int8_t reg = registerFile->readRegister(bits210);
                    int8_t result = ((uint8_t) reg >> 1) | (reg << 7);

                    registerFile->setFlagZ(result == 0);
                    registerFile->setFlagN(false);
                    registerFile->setFlagH(false);
                    registerFile->setFlagC(regA & 0x01);

                    registerFile->writeRegister(bits210, result);
                    break;

                // RRC (HL) : 4 M-cycle : 1 instruction
                case 0x0E:
                    uint16_t addr = registerFile->readHL();
                    int8_t data = memory->readMemory(addr);

                    int8_t result = ((uint8_t) data >> 1) | (data << 7);

                    registerFile->setFlagZ(result == 0);
                    registerFile->setFlagN(false);
                    registerFile->setFlagH(false);
                    registerFile->setFlagC(data & 0x01);

                    memory->writeMemory(addr, result);
                    break;

                // RR r1 : 2 M-cycle : 7 instruction
                case 0x1F: case 0x18: case 0x19: case 0x1A: case 0x1B: case 0x1C: case 0x1D:
                    int8_t reg = registerFile->readRegister(bits210);
                    int8_t carryFlag = registerFile->readFlagC();
                    int8_t result = ((uint8_t) reg >> 1) | (carryFlag << 7);

                    registerFile->setFlagZ(result == 0);
                    registerFile->setFlagN(false);
                    registerFile->setFlagH(false);
                    registerFile->setFlagC(reg & 0x01);

                    registerFile->writeRegister(bits210, result);
                    break;

                // RR (HL) : 4 M-cycle : 1 instruction
                case 0x1E:
                    uint16_t addr = registerFile->readHL();
                    int8_t data = memory->readMemory(addr);

                    int8_t carryFlag = registerFile->readFlagC();
                    int8_t result = ((uint8_t) data >> 1) | (carryFlag << 7);

                    registerFile->setFlagZ(result == 0);
                    registerFile->setFlagN(false);
                    registerFile->setFlagH(false);
                    registerFile->setFlagC(data & 0x01);

                    memory->writeMemory(addr, result);
                    break;

                // SLA r1 : 2 M-cycles : 7 instructions
                case 0x27: case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25:
                    int8_t reg = registerFile->readRegister(bits210);
                    int8_t result = reg << 1;

                    registerFile->setFlagZ(result == 0);
                    registerFile->setFlagN(false);
                    registerFile->setFlagH(false);
                    registerFile->setFlagC((uint8_t) reg >> 7);

                    registerFile->writeRegister(bits210, result);
                    break;

                // SLA (HL) : 4 M-cycles : 1 instruction
                case 0x26:
                    uint16_t addr = registerFile->readHL();
                    int8_t data = memory->readMemory(addr);
                    int8_t result = data << 1;

                    registerFile->setFlagZ(result == 0);
                    registerFile->setFlagN(false);
                    registerFile->setFlagH(false);
                    registerFile->setFlagC((uint8_t) data >> 7);

                    memory->writeMemory(addr, result);
                    break;

                // SRA r1 : 2 M-cycles : 7 instructions
                case 0x2F: case 0x28: case 0x29: case 0x2A: case 0x2B: case 0x2C: case 0x2D:
                    int8_t reg = registerFile->readRegister(bits210);
                    int8_t result = reg >> 1;

                    registerFile->setFlagZ(result == 0);
                    registerFile->setFlagN(false);
                    registerFile->setFlagH(false);
                    registerFile->setFlagC(reg & 0x01);

                    registerFile->writeRegister(bits210, result);
                    break;

                // SRA (HL) : 4 M-cycles : 1 instruction
                case 0x2E:
                    uint16_t addr = registerFile->readHL();
                    int8_t data = memory->readMemory(addr);
                    int8_t result = data >> 1;

                    registerFile->setFlagZ(result == 0);
                    registerFile->setFlagN(false);
                    registerFile->setFlagH(false);
                    registerFile->setFlagC(data & 0x01);

                    memory->writeMemory(addr, result);
                    break;

                // SRL r1 : 2 M-cycles : 7 instructions
                case 0x3F: case 0x38: case 0x39: case 0x3A: case 0x3B: case 0x3C: case 0x3D:
                    int8_t reg = registerFile->readRegister(bits210);
                    int8_t result = (uint8_t) reg >> 1;

                    registerFile->setFlagZ(result == 0);
                    registerFile->setFlagN(false);
                    registerFile->setFlagH(false);
                    registerFile->setFlagC(reg & 0x01);

                    registerFile->writeRegister(bits210, result);
                    break;

                // SRL (HL) : 4 M-cycles : 1 instruction
                case 0x3E:
                    uint16_t addr = registerFile->readHL();
                    int8_t data = memory->readMemory(addr);
                    int8_t result = (uint8_t) data >> 1;

                    registerFile->setFlagZ(result == 0);
                    registerFile->setFlagN(false);
                    registerFile->setFlagH(false);
                    registerFile->setFlagC(data & 0x01);

                    memory->writeMemory(addr, result);
                    break;

                // BIT INSTRUCTIONS (192 instructions)

                // BIT b, r1 : 2 M-cycle : 56 instructions
                case 0x47: case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45:
                case 0x4F: case 0x48: case 0x49: case 0x4A: case 0x4B: case 0x4C: case 0x4D:
                case 0x57: case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55:
                case 0x5F: case 0x58: case 0x59: case 0x5A: case 0x5B: case 0x5C: case 0x5D:
                case 0x67: case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65:
                case 0x6F: case 0x68: case 0x69: case 0x6A: case 0x6B: case 0x6C: case 0x6D:
                case 0x77: case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75:
                case 0x7F: case 0x78: case 0x79: case 0x7A: case 0x7B: case 0x7C: case 0x7D:
                    int8_t data = registerFile->readRegister(bits210);
                    int8_t mask = 1 << bits543;

                    registerFile->setFlagZ(data & mask == 0);
                    registerFile->setFlagN(false);
                    registerFile->setFlagH(true);
                    break;
                // BIT b, (HL) : 3 M-cycles : 8 instructions
                case 0x46: case 0x4E: case 0x56: case 0x5E: case 0x66: case 0x6E: case 0x76: case 0x7E:
                    uint16_t addr = registerFile->readHL();
                    int8_t data = memory->readMemory(addr);
                    int8_t mask = 1 << bits543;

                    registerFile->setFlagZ(data & mask == 0);
                    registerFile->setFlagN(false);
                    registerFile->setFlagH(true);
                    break;

                // SET b, r1 : 2 M-cycles : 56 instructions
                case 0xC7: case 0xC0: case 0xC1: case 0xC2: case 0xC3: case 0xC4: case 0xC5:
                case 0xCF: case 0xC8: case 0xC9: case 0xCA: case 0xCB: case 0xCC: case 0xCD:
                case 0xD7: case 0xD0: case 0xD1: case 0xD2: case 0xD3: case 0xD4: case 0xD5:
                case 0xDF: case 0xD8: case 0xD9: case 0xDA: case 0xDB: case 0xDC: case 0xDD:
                case 0xE7: case 0xE0: case 0xE1: case 0xE2: case 0xE3: case 0xE4: case 0xE5:
                case 0xEF: case 0xE8: case 0xE9: case 0xEA: case 0xEB: case 0xEC: case 0xED:
                case 0xF7: case 0xF0: case 0xF1: case 0xF2: case 0xF3: case 0xF4: case 0xF5:
                case 0xFF: case 0xF8: case 0xF9: case 0xFA: case 0xFB: case 0xFC: case 0xFD:
                    int8_t data = registerFile->readRegister(bits210);
                    int8_t mask = 1 << bits543;

                    registerFile->writeRegister(bits210, data | mask);
                    break;
                // SET b, (HL) : 4 M-cycles : 8 instructions
                case 0xC6: case 0xCE: case 0xD6: case 0xDE: case 0xE6: case 0xEE: case 0xF6: case 0xFE:
                    uint16_t addr = registerFile->readHL();
                    int8_t data = memory->readMemory(addr);
                    int8_t mask = 1 << bits543;

                    memory->writeMemory(addr, data | mask);
                    break;

                // RES b, r1 : 2 M-cycles : 56 instructions
                case 0x87: case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85:
                case 0x8F: case 0x88: case 0x89: case 0x8A: case 0x8B: case 0x8C: case 0x8D:
                case 0x97: case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95:
                case 0x9F: case 0x98: case 0x99: case 0x9A: case 0x9B: case 0x9C: case 0x9D:
                case 0xA7: case 0xA0: case 0xA1: case 0xA2: case 0xA3: case 0xA4: case 0xA5:
                case 0xAF: case 0xA8: case 0xA9: case 0xAA: case 0xAB: case 0xAC: case 0xAD:
                case 0xB7: case 0xB0: case 0xB1: case 0xB2: case 0xB3: case 0xB4: case 0xB5:
                case 0xBF: case 0xB8: case 0xB9: case 0xBA: case 0xBB: case 0xBC: case 0xBD:
                    int8_t data = registerFile->readRegister(bits210);
                    int8_t mask = ~(1 << bits543);

                    registerFile->writeRegister(bits210, data & mask);
                    break;
                // RES b, (HL) : 4 M-cycles : 8 instructions
                case 0x86: case 0x8E: case 0x96: case 0x9E: case 0xA6: case 0xAE: case 0xB6: case 0xBE:
                    uint16_t addr = registerFile->readHL();
                    int8_t data = memory->readMemory(addr);
                    int8_t mask = ~(1 << bits543);

                    memory->writeMemory(addr, data & mask);
                    break;                
            }
            break;

    }
    
}
