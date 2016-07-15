#include <stdint.h>

#include "ohm.h"

uint8_t ohm[] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x07, 0x00, 0x00, 0x06, 0x00, 0x00, 0x04, 0xC0,
		0x3F, 0x04, 0xE0, 0xFF, 0x04, 0x30, 0xC0, 0x05,
		0x10, 0x80, 0x07, 0x10, 0x00, 0x03, 0x10, 0x00,
		0x00, 0x10, 0x00, 0x00, 0x10, 0x00, 0x00, 0x10,
		0x00, 0x00, 0x10, 0x00, 0x03, 0x10, 0x80, 0x07,
		0x30, 0xC0, 0x05, 0xE0, 0xFF, 0x04, 0xC0, 0x3F,
		0x04, 0x00, 0x00, 0x04, 0x00, 0x00, 0x06, 0x00,
		0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

};

int ohm_width  = 24;
int ohm_height = 24;

uint8_t shortBIT[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C,
    0x00, 0x80, 0xFF, 0x01, 0xC0, 0x00, 0x03, 0x60,
    0x00, 0x06, 0x30, 0x00, 0x0C, 0x18, 0x00, 0x18,
    0x08, 0x00, 0x10, 0x08, 0x00, 0x10, 0x0C, 0x00,
    0x10, 0x8C, 0x9F, 0x31, 0x8C, 0x9F, 0x31, 0x0C,
    0x00, 0x10, 0x08, 0x00, 0x10, 0x08, 0x00, 0x10,
    0x18, 0x00, 0x18, 0x30, 0x00, 0x0C, 0x60, 0x00,
    0x06, 0xC0, 0x00, 0x03, 0x80, 0xFF, 0x01, 0x00,
    0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

int shortBIT_width  = 24;
int shortBIT_height = 24;
