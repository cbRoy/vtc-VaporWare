#include <stdint.h>
#include <stdio.h>
#include "globals.h"
#include "debug.h"
#include "materials.h"
#include "settings.h"

void materialIndexSet(uint32_t materialIndex) {
    if (materialIndex < 0) {
        s.materialIndex = DEFAULTMATERIAL;
    } else if (materialIndex >= vapeMaterialsCount) {
        s.materialIndex = DEFAULTMATERIAL;
    } else if (vapeMaterialList[materialIndex].name[0] == '\0') {
        /* Somehow we got to the sentinal, things are probably about to get bad */
        s.materialIndex = DEFAULTMATERIAL;
    } else {
        s.materialIndex = materialIndex;
    }

    g.atomInfo.tcr = vapeMaterialList[materialIndex].tcr;
    g.baseSettingsChanged = 1;
    g.settingsChanged = 1;
}

void modeSet(uint8_t mode) {
    if (mode < 0) {
        s.mode = DEFAULTMODE;
    } else if (mode >= modeCount) {
        /* This is currently 'not perfect' as vapemodes is a larger 
           array than we use.  Point to cleanup */
        s.mode = DEFAULTMODE;
    } else if (g.vapeModes[mode] == NULL) {
        s.mode = DEFAULTMODE;
    } else if (g.vapeModes[mode]->supportedMaterials &
        vapeMaterialList[s.materialIndex].typeMask) {
        // TODO: Extract this out as its own helper function in modes
        s.mode = DEFAULTMODE;
    } else {
        s.mode = mode;
    }
    setVapeMode(mode);

    g.freqSettingsChanged = 1;
    g.settingsChanged = 1;
}

void screenTimeoutSet(uint16_t screenTimeout) {
    if (screenTimeout < SCREENMINTIMEOUT) {
        s.screenTimeout = SCREENDEFAUTLTIMEOUT;
    } else if (screenTimeout > SCREENMAXTIMEOUT) {
        s.screenTimeout = SCREENDEFAUTLTIMEOUT;
    } else {
        s.screenTimeout = screenTimeout;
    }

    g.baseSettingsChanged = 1;
    g.settingsChanged = 1;
}

void tempScaleTypeIndexSet(uint8_t tempScaleTypeIndex) {
    // tempScales currently live in the settings struct.
    if (tempScaleTypeIndex < 0) {
        s.tempScaleTypeIndex = DEFAULTTEMPSCALE;
    } else if (tempScaleTypeIndex >= tempScaleCount) {
        s.tempScaleTypeIndex = DEFAULTTEMPSCALE;
    } else if (tempScaleType[tempScaleTypeIndex].display[0] == '\0') {
        s.tempScaleTypeIndex = DEFAULTTEMPSCALE;   
    } else {
        s.tempScaleTypeIndex = tempScaleTypeIndex;
    }
    g.baseSettingsChanged = 1;
    g.settingsChanged = 1;
}

void displayTemperatureSet(uint32_t displayTemperature) {
    if (displayTemperature > tempScaleType[s.tempScaleTypeIndex].max) {
        s.displayTemperature = tempScaleType[s.tempScaleTypeIndex].def;
    } else if (displayTemperature < tempScaleType[s.tempScaleTypeIndex].min) {
        s.displayTemperature = tempScaleType[s.tempScaleTypeIndex].def;
    } else {
        s.displayTemperature = displayTemperature;
    }
    g.freqSettingsChanged = 1;
    g.settingsChanged = 1;
    // Todo set target temp based on display?
}

void targetTemperatureSet(uint32_t targetTemperature) {
    // 0 is C and the unit all the low levels use
    if (targetTemperature > tempScaleType[0].max) {
        s.targetTemperature = tempScaleType[0].def;
    } else if (targetTemperature < tempScaleType[0].min) {
        s.targetTemperature = tempScaleType[0].def;
    } else {
        s.targetTemperature = targetTemperature;
    }
    g.freqSettingsChanged = 1;
    g.settingsChanged = 1;
}

void pidPSet(uint32_t pidP) {
    if (pidP > MAXPID) {
        s.pidP = DEFPIDP;
    } else if(pidP < MINPID) {
        s.pidP = DEFPIDP;
    } else {
        s.pidP = pidP;
    }
    g.baseSettingsChanged = 1;
    g.settingsChanged = 1;
}

void pidISet(uint32_t pidI){
    if (pidI > MAXPID) {
        s.pidI = DEFPIDI;
    } else if(pidI < MINPID) {
        s.pidI = DEFPIDI;
    } else {
        s.pidI = pidI;
    }
    g.baseSettingsChanged = 1;
    g.settingsChanged = 1;
}

void pidDSet(uint32_t pidD) {
    if (pidD > MAXPID) {
        s.pidD = DEFPIDD;
    } else if(pidD < MINPID) {
        s.pidD = DEFPIDD;
    } else {
        s.pidD = pidD;
    }
    g.baseSettingsChanged = 1;
    g.settingsChanged = 1;
}

void initWattsSet(int32_t initWatts) {
    if (initWatts < MINWATTS) {
        s.initWatts = DEFWATTS;
    } else if (initWatts > MAXWATTS) {
        s.initWatts = DEFWATTS;
    } else {
        s.initWatts = initWatts;
    }
    g.baseSettingsChanged = 1;
    g.settingsChanged = 1;
}

void pidSwitchSet(int32_t pidSwitch) {
    if (pidSwitch < STEMPMIN) {
        s.pidSwitch = STEMPDEF;
    } else if (pidSwitch > STEMPMAX) {
        s.pidSwitch = STEMPDEF;
    } else {
        s.pidSwitch = pidSwitch;
    }
    g.baseSettingsChanged = 1;
    g.settingsChanged = 1;
}

void invertDisplaySet(uint8_t invertDisplay) {
    if (invertDisplay != 0 &&
        invertDisplay != 1) {
        s.invertDisplay = INVERTDEF;
    } else {
        s.invertDisplay = invertDisplay;
    }
    g.settingsChanged = 1;
}

void flipOnVapeSet(uint8_t flipOnVape) {
    if (flipOnVape !=0  &&
        flipOnVape != 1) {
        s.flipOnVape = FLIPDEF;
    } else {
        s.flipOnVape = flipOnVape;
    }
    g.settingsChanged = 1;
}

void tcrSet(uint16_t tcr) {
    if (tcr < TCRMIN) {
        s.tcr = TCRDEF;
    } else if (tcr > TCRMAX) {
        s.tcr = TCRDEF;
    } else {
        s.tcr = tcr;
    }

    g.atomInfo.tcr = s.tcr;
    g.settingsChanged = 1;
}

void baseTempSet(int16_t baseTemp) {
    if (baseTemp < BTEMPMIN) {
        s.baseTemp = BTEMPDEF;
    } else if (baseTemp > BTEMPMAX) {
        s.baseTemp = BTEMPDEF;
    } else {
        s.baseTemp = baseTemp;
    }

    g.atomInfo.base_temperature = baseTemp;
    g.settingsChanged = 1;
}

void baseResSet(uint16_t baseRes) {
    if (baseRes < BRESMIN) {
        s.baseRes = BRESDEF;
    } else if (baseRes > BRESMAX) {
        s.baseRes = BRESDEF;
    } else {
        s.baseRes = baseRes;
    }

    g.atomInfo.base_resistance = baseRes;
    g.settingsChanged = 1;
}