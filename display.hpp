#ifndef DISPLAY_HPP_INCLUDED
#define DISPLAY_HPP_INCLUDED

#include <inttypes.h>

#define DISPLAY_X 256
#define DISPLAY_Y 256

#define VISIBLE_DISPLAY_X 160
#define VISIBLE_DISPLAY_Y 144

class Display {
private:
    Memory *memory;

    int8_t backgroundDisplay[DISPLAY_X][DISPLAY_Y];
    int8_t finalDisplay[VISIBLE_DISPLAY_X][VISIBLE_DISPLAY_Y];

    bool displayEnabled();
    bool displayTileMapSelect();
    bool windowEnabled();
    bool backgroundWindowTileDataSelect();
    bool backgroundTileMapSelect();
    bool spriteSize();
    bool spriteDisplayEnabled();
    bool backgroundEnabled();

public:
    void stallUntilButtonPress();

    void writeScrollX(int16_t value);
    void writeScrollY(int16_t value);

    uint8_t readScrollX();
    uint8_t readScrollY();
    uint8_t readWindowX();
    uint8_t readWindowY();
    uint8_t readLCD();

    // Draws the screen
    void drawScreen();
    // Get the tile given tile index
    int16_t getTile(int16_t, uint8_t);
    // Draw the tile with given offset for current scanline
    void drawTile(int8_t);
    // Draw the sprite with the given offset for current scanline
    void drawSprite(int8_t);
    // Draw all tiles
    void drawBackground();
    // Draw all sprites
    void drawSprites();

    
} 

#endif
