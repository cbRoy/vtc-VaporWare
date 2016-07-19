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

#include "levels.h"

// 1111
// 1111
uint8_t brick[] = {0x03,0x03,0x03,0x03};
uint8_t brickHeight = 2;
uint8_t brickWidth = 4;

// 11
// 11
uint8_t ball[] = {0x03,0x03};
uint8_t ballHeight = 2;
uint8_t ballWidth = 2;

// 11111111
// 11111111
uint8_t paddle[] ={0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03};
uint8_t  paddleHeight = 2;
uint8_t  paddleWidth = 8;


struct gameState {
    uint8_t paddlecurX;
    uint8_t paddlecurY;
    uint8_t ballcurX;
    uint8_t ballcurY;
    uint8_t ballDirection;
    uint16_t delay;
    uint8_t level;
    uint32_t score;
    uint8_t quit;

};

struct gameState vapeoutState = {
    .paddlecurX = 2,
    .paddlecurY = 20,
    .ballcurX = 98,
    .ballcurY = 30,
    .ballDirection = 1,
    .delay = 100,
    .level = 1,
    .score = 0,
    .quit = 0
};

int checkPaddleColission(){
    return 0;
}

void paddleMoveLeft(uint8_t status, uint32_t held) {
    if ((status & (BUTTON_PRESS | BUTTON_HELD))) {
        vapeoutState.paddlecurY--;
        if (checkPaddleColission())
            vapeoutState.paddlecurY++;
    }
}

void paddleMoveRight(uint8_t status, uint32_t held) {
    if ((status & (BUTTON_PRESS))) {
        vapeoutState.paddlecurY++;
        if (checkPaddleColission())
            vapeoutState.paddlecurY--;
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

void drawBricks(struct levelDesc *lvl){
  Display_Clear();
  int i, j;
  int x=2, y=3;
  for(i = 0; i < lvl->height; i++){
    x=2;
    for(j = 0; j < lvl->width; j++){
        if(lvl->layout[(i*lvl->width)+j] == 1){
            Display_PutPixels(x,y,brick,brickWidth,brickHeight);
        }
        x+=brickWidth+1;
    }
    y+=brickHeight + 1;
  }
}

void checkColission(struct levelDesc *lvl){
  int i, j;
  int x=2, y=3;
  for(i = 0; i < lvl->height; i++){
    x=2;
    for(j = 0; j < lvl->width; j++){
        if(lvl->layout[(i*lvl->width)+j] == 1){
            if(vapeoutState.ballcurX == x && vapeoutState.ballcurY == y){
                lvl->layout[(i*lvl->width)+j] = 0;
                //reverse ball direction
            }
        }
        x+=brickWidth+1;
    }
    y+=brickHeight + 1;
  }
}

void drawPaddle(){
    Display_PutPixels(vapeoutState.paddlecurX, vapeoutState.paddlecurY, paddle, paddleWidth, paddleHeight);

}

void drawBall(){
    //Display_PutPixels(vapeoutState.ballcurX, vapeoutState.ballcurX, ball, ballWidth, ballHeight);
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
      checkColission(&level1Desc);
      drawPaddle();
      drawBall();
      drawBricks(&level1Desc);
      Display_Update();
    }
gameover:
    returnHandler();
}

static void __attribute__((constructor)) registerVapeoutDrawables(void) {
}
