#ifndef VAPEOUT_LEVELS_H
#define VAPEOUT_LEVELS_H

#include <stdint.h>

#define MAXROWS 5
#define MAXLEVELS 3

typedef struct levelDesc {
    uint8_t width;
    uint8_t height;
    uint8_t layout[MAXROWS];
} levelDesc;

struct levelDesc level1Desc = {
     .width = 8,
     .height = 3,
     .layout = { 0xFF, 0xFF, 0xFF }
     // 11111111
     // 11111111
     // 11111111
};

struct levelDesc level2Desc = {
  .width = 8,
  .height = 4,
  .layout = { 0xFF, 0x7E, 0x3c, 0x18 }
  // 11111111
  //  111111
  //   1111
  //    11
};

#endif // VAPEOUT_LEVELS_H
