#include <stdint.h>

#ifndef BITMAP_HOT_H
#define BITMAP_HOT_H

uint8_t hot[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00,
	0xFE, 0x03, 0x80, 0xFF, 0x07, 0x00, 0xFF, 0x0F,
	0x00, 0x3E, 0x1E, 0x00, 0x1E, 0x18, 0x00, 0x1C,
	0x18, 0x00, 0x0E, 0x38, 0x00, 0x0E, 0x38, 0x00,
	0x0F, 0x18, 0xCC, 0x07, 0x18, 0xF8, 0x03, 0x1C,
	0xF0, 0x07, 0x0F, 0xC0, 0xFF, 0x07, 0x00, 0xFF,
	0x03, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

int hot_width  = 24;
int hot_height = 24;

#endif