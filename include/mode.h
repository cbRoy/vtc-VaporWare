#ifndef __MODES_H
#define __MODES_H

#include "menu.h"

#define MODE_COUNT 8

enum {
    WATT_CONTROL,
    VOLT_CONTROL,
    TEMP_CONTROL,
    MAX_CONTROL
};

struct vapeMode {
    int8_t index;
    int8_t controlType;
    char name[8];
    int8_t supportedMaterials;
    uint32_t maxSetting;
    void (*init) (void);
    void (*fire) (void);
    void (*increase) (void);
    void (*decrease) (void);
    void (*display) (uint8_t atomizerOn);
    void (*bottomDisplay) (uint8_t atomizerOn);
    struct menuDefinition *vapeModeMenu;
};
extern uint8_t modeCount;

void setVapeMode(int newMode);
void setVapeMaterial(int index);

void (*__init) (void);
void (*__vape) (void);
void (*__up) (void);
void (*__down) (void);

#endif