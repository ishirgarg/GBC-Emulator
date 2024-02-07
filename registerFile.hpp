#ifndef REGISTERFILE_INCLUDED_H
#define REGISTERFILE_INCLUDED_H

#include <inttypes.h>

class RegisterFile {
private:
    int8_t A = 0;
    int8_t B = 0;
    int8_t C = 0;
    int8_t D = 0;
    int8_t E = 0;
    int8_t F = 0;
    int8_t H = 0;
    int8_t L = 0;

    uint16_t SP = 0x100;
    uint16_t PC = 0xFFFE;

    // Maps indices to registers
    int8_t *registerMap[8] = {&B, &C, &D, &E, &H, &L, &F, &A};

public:
    // Read register with index registerIndex
    int8_t readRegister(int8_t registerIndex);
    // Write data to register with index registerIndex
    void writeRegister(int8_t registerIndex, int8_t data);
    // Increment program counter by 1
    void incrementPC(int8_t offset=1);

    // Read 16-bit registers
    int16_t readAF();
    int16_t readBC();
    int16_t readDE();
    int16_t readHL();
    // Read stack pointer
    uint16_t readSP();
    // Read program counter
    uint16_t readPC();

    // Write data to 16-bit registers
    void writeAF(int16_t data);
    void writeBC(int16_t data);
    void writeDE(int16_t data);
    void writeHL(int16_t data);
    void writeSP(uint16_t data);
    void writePC(uint16_t data);

    // Increment HL by offset
    void incrementHL(int8_t offset);
    // Increment stack pointer by offset
    void incrementSP(int16_t offset);

    // Set flags
    void setFlagZ(bool value);
    void setFlagN(bool value);
    void setFlagH(bool value);
    void setFlagC(bool value);

    // Read flags
    int8_t readFlagZ();
    int8_t readFlagC();
    int8_t readFlagN();
    int8_t readFlagH();
};

#endif
