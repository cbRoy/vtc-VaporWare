#include <stdint.h>
#include "space_res.h"

uint8_t Bitmap_alien1[] = {
        0x1C, 0x5E, 0xFE, 0xB6, 0x37, 0x5F, 0x5F, 0x37,
        0xB6, 0xFE, 0x5E, 0x1C
};

uint8_t Bitmap_alien1_shot[] = {
        0x10, 0x92, 0x44, 0x28, 0x81, 0x42, 0x00, 0x42,
        0x81, 0x28, 0x44, 0x92, 0x10
};
uint8_t Bitmap_alien1_step[] = {
        0x9C, 0x9E, 0x5E, 0x76, 0x37, 0x5F, 0x5F, 0x37,
        0x76, 0x5E, 0x9E, 0x9C
};

uint8_t Bitmap_ship[] = {
        0x78, 0x7C, 0x7C, 0x7C, 0x7C, 0x7F, 0x7F, 0x7F,
        0x7C, 0x7C, 0x7C, 0x7C, 0x78
};

int Bitmap_ship_width  = 13;
int Bitmap_ship_height = 7;

int Bitmap_alien1_width  = 12;
int Bitmap_alien1_height = 8;
int Bitmap_alien1_shot_width  = 13;
int Bitmap_alien1_shot_height = 8;
int Bitmap_alien1_step_width  = 12;
int Bitmap_alien1_step_height = 8;