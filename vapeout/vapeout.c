#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Display.h"
#include "TimerUtils.h"

#include "button.h"
#include "display.h"
//#include "drawables.h"
#include "variabletimer.h"
#include "debug.h"

// 1111
// 1111
uint8_t brick[] = {0x03,0x03,0x03,0x03};
uint8_t brickHeight = 2;
uint8_t brickWidth = 4;

struct gameState {
    uint8_t paddlecurX;
    uint8_t paddlecurY;
    uint16_t delay;
    uint8_t level;
    uint32_t score;
    uint8_t quit;

};

struct gameState vapeoutState = { 0 };

int checkColission(){
  return 0; //TODO
}
void paddleMoveLeft(uint8_t status, uint32_t held) {
    if ((status & (BUTTON_PRESS | BUTTON_HELD))) {
        vapeoutState.paddlecurX--;
        if (checkColission())
            vapeoutState.paddlecurX++;
    }
}

void paddleMoveRight(uint8_t status, uint32_t held) {
    if ((status & (BUTTON_PRESS))) {
        vapeoutState.paddlecurX++;
        if (checkColission())
            vapeoutState.paddlecurX--;
    }
}

void vapeoutLaunch(uint8_t status, uint32_t held) {
    if (status & BUTTON_HELD && held > 3000) {
        vapeoutState.quit = 1;
    }
}

struct buttonHandler vapeoutHandler = {
        .name = "mainButtons",
        .flags = FIRE_HOLD_EVENT,

        .fire_handler = &vapeoutLaunch,
        .fireUpdateInterval = 150,

        .left_handler = &paddleMoveLeft,
        .leftUpdateInterval = 150,

        .right_handler = &paddleMoveRight,
        .rightUpdateInterval = 150,

};

void drawBricks(){
  Display_Clear();
  int i, j;
  int x=2, y=3;
  for(i=0; i < 2; i++){
    x=2;
    for(j=0;j<10;j++){
        Display_PutPixels(x,y,brick,brickWidth, brickHeight);
        x+=brickWidth+1;
    }
    y+=brickHeight + 1;
  }
}
void runVapeout(){
    switchHandler(&vapeoutHandler);

    while(1){
      //setupScreen();

      if (gv.buttonEvent) {
          handleButtonEvents();
          gv.buttonEvent = 0;
      }

      if(vapeoutState.quit == 1){
        goto gameover;
      }

      //drawScreen();
      drawBricks();
      Display_Update();
    }
gameover:
    returnHandler();
}

static void __attribute__((constructor)) registerVapeoutDrawables(void) {
}
