#include "cpu.hpp"
#include "memory.hpp"
#include "display.hpp"
#include "registerFile.hpp"
#include <fstream>
#include <string>

int main() {
    RegisterFile *registerFile = RegisterFile();
    Memory *memory = new Memory;
    CPU cpu = CPU(); 
    
    std::string romName = "test/cpu.gb";
    ifstream romFile(romName);
    
    const std::string bootRomName = "dmg.gb";
    ifstream bootRomFile(bootRomName);
    
    // Add BootROM to memory
    for (int i = 0; i < 0x100; i++) {
        memory->initializeBootRom(&bootRomFile); 
    } 

    // Add ROM to memory
    uint8_t opcode;
    int32_t address = 0;
    while (opcode = fin.get()) {
        memory->writeMemory(address++, opcode);
    }

    // Main loop
    while (true) {
        uint8_t opcode = memory->readByteAtPC();
        registerFile->incrementPC();
        
        cpu->executeOpcode(opcode);

        
    }

    return 0;
}
