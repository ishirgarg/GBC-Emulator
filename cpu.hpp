#ifndef CPU_INCLUDED_H
#define CPU_INCLUDED_H

#include <inttypes.h>
#include "registerFile.hpp"
#include "memory.hpp"
#include "display.hpp"
#include "utilBytes.hpp"

class CPU {
private:
    RegisterFile *registerFile;
    Memory *memory;
    Display *display;

    // Check if there is a carry on a 8-bit add
    bool is8BitCarryAdd(int8_t num1, int8_t num2);
    // Check if there is a carry on a 16-bit add
    bool is16BitCarryAdd(int16_t num1, int16_t num2);
    // Check if 3rd bit carries over in 8-bit addition
    bool isHalfCarryAdd(int8_t num1, int8_t num2);
    // Check if the 11th bit carries over in 16-bit addition
    bool isUpperHalfCarryAdd(int16_t num1, int16_t num2);
    // Enable or disable interrupts
    void enableInterrupts(bool);
    // Stall CPU until an interrupt
    void stallUntilInterrupt();
    // Stall CPU until a button is pressed
    void stallUntilButtonPress();
    // Check if there is a borrow on an 8-bit subtraction
    bool is8BitCarrySub(int8_t num1, int8_t num2);
    // Check if there is a borrow from bit 4 on 8-bit subtraction
    bool isHalfCarrySub(int8_t num1, int8_t num2);

public:
    void executeOpcode(uint8_t opcode);
 
};

#endif