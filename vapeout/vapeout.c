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

// 1111111
// 1111111
uint8_t brick[] = {0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03};
uint8_t brickHeight = 2;
uint8_t brickWidth = 7;

// 11
// 11
uint8_t ball[] = {0x03,0x03};
uint8_t ballHeight = 2;
uint8_t ballWidth = 2;

// 1111111111
// 1111111111
uint8_t paddle[] = {0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03};
uint8_t  paddleHeight = 2;
uint8_t  paddleWidth = 10;


struct gameState {
    uint8_t paddlecurX;
    uint8_t paddlecurY;
    uint8_t ballcurX;
    uint8_t ballcurY;
    uint8_t ballDirection;
    uint8_t ballMagnitude;
    uint8_t ballVelocityX;
    uint8_t ballVelocityY;
    uint16_t delay;
    uint8_t level;
    uint32_t score;
    uint8_t quit;

};

struct gameState vapeoutState = {
    .delay = 100,
    .level = 1,
    .score = 0,
    .quit = 0
};

void initVapeout(){
  vapeoutState.paddlecurX = ((DISPLAY_WIDTH - paddleWidth) / 2);
  vapeoutState.paddlecurY = (DISPLAY_HEIGHT - paddleHeight) - 2;
  vapeoutState.ballcurX = (DISPLAY_WIDTH - ballWidth) / 2;
  vapeoutState.ballcurY = vapeoutState.paddlecurY - ballHeight;
  vapeoutState.ballDirection = (rand()%2) * 2 - 1;
  vapeoutState.ballMagnitude = (drand48()+1)/50;
  vapeoutState.ballVelocityX = vapeoutState.ballDirection * vapeoutState.ballMagnitude;
  vapeoutState.ballVelocityY = 0.5;
}

void paddleMoveLeft(uint8_t status, uint32_t held) {
  int amount = 0;
  if ((status & BUTTON_PRESS) && (held < 300)) {
      amount = 4;
  } else if ((held > 300) && status & BUTTON_HELD) {
      amount = 2;
  }
  if(amount > 0){
    if(vapeoutState.paddlecurX - amount >= 0){
      vapeoutState.paddlecurX -= amount;
    }
  }
}

void paddleMoveRight(uint8_t status, uint32_t held) {
    int amount = 0;
    if ((status & BUTTON_PRESS) && (held < 300)) {
        amount = 4;
    } else if ((held > 300) && status & BUTTON_HELD) {
        amount = 2;
    }
    if (amount > 0) {
        // check bounds
        if ((vapeoutState.paddlecurX + amount) < (DISPLAY_WIDTH - paddleWidth)) {
            vapeoutState.paddlecurX += amount;
        }
    }
}

void vapeoutLaunch(uint8_t status, uint32_t held) {
    if (status & BUTTON_HELD && held > 3000) {
        vapeoutState.quit = 1;
    }
}

struct buttonHandler vapeoutHandler = {
        .name = "mainButtons",
        .flags = LEFT_HOLD_EVENT | RIGHT_HOLD_EVENT,

        .fire_handler = &vapeoutLaunch,
        .fireUpdateInterval = 150,

        .left_handler = &paddleMoveLeft,
        .leftUpdateInterval = 50,

        .right_handler = &paddleMoveRight,
        .rightUpdateInterval = 50,

};

void drawBricks(struct levelDesc *lvl){
  int row, col;
  uint8_t mask;
  int x=1, y=3;
  for(row = 0; row < lvl->height; row++){
    x=1;
    mask = lvl->layout[row];
    for(col = 0; col < lvl->width; col++){
        if(mask & ( 1 << (7 - col))){
            Display_PutPixels(x,y,brick,brickWidth,brickHeight);
        }
        x+=brickWidth+1;
    }
    y+=brickHeight + 1;
  }
}

void checkBallColission(struct levelDesc *lvl){
  int row, col;
  int x=2, y=3;
  uint8_t mask;
  for(row = 0; row < lvl->height; row++){
    x=2;
    mask = lvl->layout[row];
    for(col = 0; col < lvl->width; col++){
        if(mask & ( 1 << (7 - col))){
            if(vapeoutState.ballcurX == x && vapeoutState.ballcurY == y){
                //lvl->layout[(i*lvl->width)+j] = 0;
                // how to change the value to show ball hit?
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
    Display_PutPixels(vapeoutState.ballcurX, vapeoutState.ballcurY, ball, ballWidth, ballHeight);
}

void runVapeout(){
    switchHandler(&vapeoutHandler);
    initVapeout();

    while(1){
      setupScreen();

      if (gv.buttonEvent) {
          handleButtonEvents();
          gv.buttonEvent = 0;
      }

      if(vapeoutState.quit == 1){
        goto gameover;
      }

      //drawScreen();
      checkBallColission(&level1Desc);
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
