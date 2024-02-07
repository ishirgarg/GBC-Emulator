#ifndef MEMORY_HPP_INCLUDED
#define MEMORY_HPP_INCLUDED

#include <inttypes.h>
#include <fstream>
#include "registerFile.hpp"

/*
MEMORY MAP:

*/

class Memory {
private:
    RegisterFile *registerFile;

public:
    // Read byte at program counter
    int8_t readByteAtPC();
    // Read memory byte at given address
    int8_t readMemory(uint16_t addr);
    // Read top byte of stack
    int8_t readFromStack(uint16_t offset);
    // Read and return top byte of stack
    int8_t popFromStack();
    // Push a byte to stack
    void pushToStack(int8_t data);
    // Write data to memory at address addr
    void writeMemory(uint16_t addr, int8_t data);
    // Initialize the boot rom
    void initializeBootRom(std::ifstream);
};

#endif
