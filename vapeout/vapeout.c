#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Display.h"
#include "TimerUtils.h"

#include "button.h"
#include "display.h"
#include "drawables.h"
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
    double ballcurX;
    double ballcurY;
    uint8_t ballDirection;
    double ballMagnitude;
    double ballVelocityX;
    double ballVelocityY;
    uint16_t delay;
    levelDesc levels[MAXLEVELS];
    uint8_t currentLevel;
    uint8_t bricksLeft;
    uint8_t lives;
    uint8_t playing;
    uint32_t score;
    uint8_t quit;

};

struct gameState vapeoutState = { 0 };

struct screen {
  uint8_t top;
  uint8_t topBrickStart;
  uint8_t left;
  uint8_t right;
  uint8_t bottom;
};

struct screen Screen = {
  .top = 10,
  .topBrickStart = 15,
  .left = 1,
  .right = DISPLAY_WIDTH - 1,
  .bottom = DISPLAY_HEIGHT - 1
};

void initLevels(){
  vapeoutState.levels[0] = level1Desc;
  vapeoutState.levels[1] = level2Desc;
  vapeoutState.levels[2] = level3Desc;
}

void resetPaddle(){
  vapeoutState.paddlecurX = ((DISPLAY_WIDTH - paddleWidth) / 2);
  vapeoutState.paddlecurY = (DISPLAY_HEIGHT - paddleHeight) - 2;
}
void resetBall(){
  vapeoutState.ballcurX = vapeoutState.paddlecurX + (paddleWidth / 2) - (ballWidth / 2);
  vapeoutState.ballcurY = vapeoutState.paddlecurY - ballHeight;
  vapeoutState.ballDirection = 1;//(rand()%2) * 2 - 1;
  vapeoutState.ballMagnitude = (drand48()+1)/5;
  vapeoutState.ballVelocityX = 1;//vapeoutState.ballDirection * vapeoutState.ballMagnitude;
  vapeoutState.ballVelocityY = -1;//0.5;
}

void initVapeout(){
  initLevels();

  vapeoutState.currentLevel = 0;
  vapeoutState.bricksLeft = vapeoutState.levels[0].bricks;
  vapeoutState.delay = 100;
  vapeoutState.lives = 3;
  vapeoutState.playing = 0;
  vapeoutState.currentLevel = 0;
  vapeoutState.score = 0;
  vapeoutState.quit = 0;

  resetPaddle();
  resetBall();
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
        if ((vapeoutState.paddlecurX + amount) < (DISPLAY_WIDTH - paddleWidth)) {
            vapeoutState.paddlecurX += amount;
        }
    }
}

void vapeoutLaunch(uint8_t status, uint32_t held) {
    if (status & BUTTON_PRESS && held < 300 && !vapeoutState.playing){
      vapeoutState.playing = 1;
    }
    if (status & BUTTON_HELD && held > 3000) {
        vapeoutState.quit = 1;
    }
}

struct buttonHandler vapeoutHandler = {
        .name = "mainButtons",
        .flags = LEFT_HOLD_EVENT | RIGHT_HOLD_EVENT | FIRE_HOLD_EVENT,

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
  int x=Screen.left, y=Screen.topBrickStart;
  for(row = 0; row < lvl->height; row++){
    x=Screen.left;
    mask = lvl->layout[row];
    for(col = 0; col < lvl->width; col++){
        if(mask & ( 1 << ((lvl->width - 1) - col))){
            Display_PutPixels(x,y,brick,brickWidth,brickHeight);
        }
        x += brickWidth + 1;
    }
    y += brickHeight + 1;
  }
}

void moveBall(){
  vapeoutState.ballcurX += vapeoutState.ballVelocityX;
  vapeoutState.ballcurY += vapeoutState.ballVelocityY;

  if(vapeoutState.ballcurX >= Screen.right ||
     vapeoutState.ballcurX <= Screen.left){
    vapeoutState.ballVelocityX *= -1;
  }
  if((vapeoutState.ballcurX >= vapeoutState.paddlecurX &&
      vapeoutState.ballcurX <= (vapeoutState.paddlecurX + paddleWidth) &&
      vapeoutState.ballcurY >= vapeoutState.paddlecurY &&
      vapeoutState.ballcurY <= (vapeoutState.paddlecurY + paddleHeight)) ||
      vapeoutState.ballcurY <= Screen.top){
        vapeoutState.ballVelocityY *= -1;
  }
  if(vapeoutState.ballcurY > vapeoutState.paddlecurY){
    resetBall();
    vapeoutState.lives--;
    vapeoutState.playing = 0;
  }
}
void checkBrickCollision(struct levelDesc *lvl){
  int row, col;
  int x = Screen.left, y = Screen.topBrickStart;
  uint8_t mask;
  for(row = 0; row < lvl->height; row++){
    x = Screen.left;
    mask = lvl->layout[row];
    for(col = 0; col < lvl->width; col++){
        if(mask & ( 1 << ((lvl->width - 1) - col))){
            if(vapeoutState.ballcurX >= x &&
               vapeoutState.ballcurX <= x + brickWidth &&
               vapeoutState.ballcurY >= y &&
               vapeoutState.ballcurY <= y + brickHeight){
                lvl->layout[row] = mask & ~(1 << ((lvl->width - 1) - col));
                //reverse ball direction
                vapeoutState.score++;
                if(--vapeoutState.bricksLeft != 0){
                  vapeoutState.ballVelocityY *= -1;
                }else{
                  vapeoutState.currentLevel++;
                  vapeoutState.bricksLeft = vapeoutState.levels[vapeoutState.currentLevel].bricks;
                  vapeoutState.playing = 0;
                  resetPaddle();
                  return;
                }
            }
        }
        x += brickWidth + 1;
    }
    y += brickHeight + 1;
  }
}

void drawLives(){
  uint8_t i;
  uint8_t x = 50, y = 5;
  for(i=0; i < vapeoutState.lives; i++){
    Display_PutPixels(x, y, ball, ballWidth, ballHeight);
    x -= ballWidth + 2;
  }
}

void drawPaddle(){
    Display_PutPixels(vapeoutState.paddlecurX, vapeoutState.paddlecurY, paddle, paddleWidth, paddleHeight);
}

void drawBall(){
    if(!vapeoutState.playing) resetBall();
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

      if(vapeoutState.quit == 1 ||
        vapeoutState.currentLevel > (MAXLEVELS-1) ||
        vapeoutState.lives == 0){
        goto gameover;
      }

      //drawScreen();
      char buff[5];
      sniprintf(buff,5, "S:%ld", vapeoutState.score);
      Display_PutText(0,0,buff,FONT_SMALL);
      sniprintf(buff,5, "%d", vapeoutState.currentLevel+1);
      Display_PutText(55,0,buff,FONT_SMALL);
      drawLives();
      Display_PutLine(0, Screen.top, Screen.right, Screen.top);

      drawBricks(&vapeoutState.levels[vapeoutState.currentLevel]);
      drawPaddle();
      drawBall();
      if(vapeoutState.playing){
        moveBall();
        checkBrickCollision(&vapeoutState.levels[vapeoutState.currentLevel]);
      }
      Display_Update();
    }
gameover:
    returnHandler();
}

static void __attribute__((constructor)) registerVapeoutDrawables(void) {
}
