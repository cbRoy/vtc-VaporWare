/*
 * This file is part of eVic SDK.
 *
 * eVic SDK is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * eVic SDK is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with eVic SDK.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) 2016 ReservedField
 * Copyright (C) 2016 kfazz
 */

#include <stdio.h>
#include <string.h>

#include <M451Series.h>
#include <Display.h>
#include <Atomizer.h>
#include <TimerUtils.h>
#include <Battery.h>
#include <Button.h>

#include "font/font_vaporware.h"
#include "globals.h"
#include "helper.h"
#include "display_helper.h"
#include "images/battery.h"
#include "settings.h"
#include "images/hot.h"
#include "images/short.h"
#include "images/ohm.h"
#include "variabletimer.h"

// NOTES:
// Evic VTC mini X-MAX = 116
uint8_t* getBatteryIcon() {
    switch (Atomizer_GetError()) {
    case WEAK_BATT:
	    return batteryalert;
    default:
    	if (Battery_IsPresent()) {
        	if (Battery_IsCharging()) {
        	    return batterycharging;
        	} else {
        		return battery;
        	}
    	} else {
    		return batteryalert;
    	}
    }
}

void updateScreen(struct globals *g) {
    if (s.stealthMode)
        return;

    uint32_t now = uptime;
    uint32_t targetBrightness = s.screenBrightness;

    char buff[9];

    if (g->screenFadeInTime == 0) {
        g->screenFadeInTime = now + s.fadeInTime;
    }

    int chargeScreen = (g->charging && !g->pauseScreenOff && (g->screenOffTime < now));

    if (!g->pauseScreenOff && s.fadeOutTime >= g->screenOffTime - now && g->screenOffTime >= now) {

        // fade out if timing out
        g->currentBrightness = (((g->screenOffTime - now) * 1000 / s.fadeOutTime) * targetBrightness) / 1000;

    } else if (!chargeScreen && g->screenFadeInTime != 0 && now <= g->screenFadeInTime) {

        // fade in
        uint32_t startTime = g->screenFadeInTime - s.fadeInTime;
        g->currentBrightness = (((now - startTime) * 1000 / s.fadeInTime) * targetBrightness) / 1000;

    } else if (chargeScreen) {

        g->currentBrightness = 40;

    }

    bool needBrightness = g->currentBrightness <= targetBrightness;
    if (needBrightness && !chargeScreen) {
        // update animation time left
        g->screenFadeInTime = now +
                (s.fadeInTime - (((g->currentBrightness * 1000 / targetBrightness) * s.fadeInTime) / 1000));

    }

    Display_SetContrast((char *)  g->currentBrightness);

    uint8_t atomizerOn = Atomizer_IsOn();

    if (!atomizerOn) {
	    g->batteryPercent = Battery_VoltageToPercent(
            Battery_IsPresent()? Battery_GetVoltage() : 0);
    }

    if (chargeScreen) {
        Display_Clear();
        // update the battery percent all the time if
        // we are charging
        getPercent(buff, g->batteryPercent);
        uint8_t size = strlen(buff);
    	Display_PutPixels(20, 20, getBatteryIcon(), battery_width, battery_height);

        Display_PutText((DISPLAY_WIDTH/2)-((12*size)/2),
            (DISPLAY_HEIGHT/2)-12, buff, FONT_LARGE);
        Display_Update();
        if (g->screenFadeInTime != 0) {
            g->screenFadeInTime = 0;
        }
        return;
    }

    Display_SetInverted(s.invertDisplay);

    if (s.flipOnVape) {
        if (atomizerOn) {
	        if (!Display_IsFlipped()) {
	            Display_Flip();
	        }
        } else {
	        if (Display_IsFlipped()) {
	            Display_Flip();
	        }
        }
    }

    Display_Clear();

    Display_PutLine(0, 30, 63, 30);

    g->vapeModes[s.mode]->display(atomizerOn);

	// battery
	Display_PutPixels(0, 40, getBatteryIcon(), battery_width, battery_height);

    getPercent(buff, g->batteryPercent);
    Display_PutText(26, 45, buff, FONT_MEDIUM);

    switch (Atomizer_GetError()) {
    case SHORT:
    case OPEN:
		Display_PutPixels(20, 75, shortBIT, shortBIT_width, shortBIT_height);
    	break;
    default:
    	Display_PutPixels(0, 70, ohm, ohm_width, ohm_height);

    	if (atomizerOn) {
            getFloating(buff, g->atomInfo.resistance);
    	} else {
    	getFloating(buff, g->baseRes);
    	}
    	Display_PutText(26, 75, buff, FONT_MEDIUM);
        g->vapeModes[s.mode]->bottomDisplay(atomizerOn);

    }

    Display_Update();
}