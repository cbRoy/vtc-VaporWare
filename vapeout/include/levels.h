#ifndef VAPEOUT_LEVELS_H
#define VAPEOUT_LEVELS_H

#include <stdint.h>

#define MAXROWS 5

struct levelDesc {
    uint8_t width;
    uint8_t height;
    uint8_t layout[MAXROWS];
};

struct levelDesc level1Desc = {
     .width = 8,
     .height = 3,
     .layout = { 0xFF, 0xFF, 0xFF }
     // 11111111
     // 11111111
     // 11111111
};

#endif // VAPEOUT_LEVELS_H
