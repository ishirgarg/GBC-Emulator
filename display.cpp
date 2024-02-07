#include <display.hpp>
#include "utilBytes.hpp"

void Display::drawScreen() {
    if (backgroundEnabled()) {
        drawBackground();
    }
    if (spriteDisplayEnabled()) {
        drawSprites();
    }
}

Tile Display::getTile(int8_t tileIndex) {
    int8_t tileMemoryAddress = backgroundWindowTileDataSelect() ? 0x8000 : 0x8800;

    Tile tile = {
        .row1 = mergeBytes(memory->readMemory(tileMemoryAddress + 2 * tileIndex + 0), memory->readMemory(tileMemoryAddress + 2 * tileIndex + 1));
        .row2 = mergeBytes(memory->readMemory(tileMemoryAddress + 2 * tileIndex + 2), memory->readMemory(tileMemoryAddress + 2 * tileIndex + 3));
        .row3 = mergeBytes(memory->readMemory(tileMemoryAddress + 2 * tileIndex + 4), memory->readMemory(tileMemoryAddress + 2 * tileIndex + 4));
        .row4 = mergeBytes(memory->readMemory(tileMemoryAddress + 2 * tileIndex + 6), memory->readMemory(tileMemoryAddress + 2 * tileIndex + 7));
        .row5 = mergeBytes(memory->readMemory(tileMemoryAddress + 2 * tileIndex + 8), memory->readMemory(tileMemoryAddress + 2 * tileIndex + 9));
        .row6 = mergeBytes(memory->readMemory(tileMemoryAddress + 2 * tileIndex + 10), memory->readMemory(tileMemoryAddress + 2 * tileIndex + 11));
        .row7 = mergeBytes(memory->readMemory(tileMemoryAddress + 2 * tileIndex + 12), memory->readMemory(tileMemoryAddress + 2 * tileIndex + 13));
        .row8 = mergeBytes(memory->readMemory(tileMemoryAddress + 2 * tileIndex + 14), memory->readMemory(tileMemoryAddress + 2 * tileIndex + 15));
    }

    return tile;
}

void drawBackground() {
    uint16_t tileDataAddress = backgroundWindowTileDataSelect() ? 0x8000 : 0x8800;
    uint16_t offset = backgroundWindowTileDataSelect() ? 0 : 128;

    uint16_t 
}

void drawSprites() {
    
}
