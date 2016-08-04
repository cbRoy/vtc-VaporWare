#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
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
  .top = 11,
  .topBrickStart = 15,
  .left = 1,
  .right = DISPLAY_WIDTH - 1,
  .bottom = DISPLAY_HEIGHT - 1
};

uint8_t vapeoutDecorations[] = {

    //DRAWLINE, ATTRGROUP, ATTRALIGN, ABSOLUTE, ENDATTRGROUP, 0,0,0,127, /* Left boarder */
    //DRAWLINE, ATTRGROUP, ATTRALIGN, ABSOLUTE, ENDATTRGROUP, 63,0,63,127, /* Right boarder */
    //DRAWLINE, ATTRGROUP, ATTRALIGN, ABSOLUTE, ENDATTRGROUP, 0,127,63,127, /* bottom boarder */
    COLGROUP, ATTRGROUP, ATTRPADDING, 0, 0, 0, 0,ENDATTRGROUP,
    DRAWLINE, ATTRGROUP, ATTRALIGN, ABSOLUTE, ENDATTRGROUP, 0,12,63,12, /* top line */

    VAPEOUTSCORE, ATTRGROUP, ATTRALIGN, ABSOLUTE, ATTRLOCATION, 2, 0, ENDATTRGROUP,
    VAPEOUTLIVES, ATTRGROUP, ATTRLOCATION, 40, 4, ENDATTRGROUP,
    VAPEOUTLEVEL, ATTRGROUP, ATTRLOCATION, 55, 0, ENDATTRGROUP,
    VAPEOUTFIELD, ATTRGROUP, ATTRLOCATION, 0, 10, ENDATTRGROUP,
    ENDCOLGROUP,
    LD
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

uint8_t drawLives(void *priv, uint8_t *args, struct layoutProperties *object){
  uint8_t i;
  uint8_t x = object->X, y = object->Y;
  for(i=0; i < vapeoutState.lives; i++){
    Display_PutPixels(x, y, ball, ballWidth, ballHeight);
    x -= ballWidth + 2;
  }
  return 0;
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

      drawScreen(vapeoutDecorations);
      Display_Update();
    }
gameover:
    returnHandler();
}
uint8_t drawVapeout(){
  drawBricks(&vapeoutState.levels[vapeoutState.currentLevel]);
  drawPaddle();
  drawBall();
  if(vapeoutState.playing){
    moveBall();
    checkBrickCollision(&vapeoutState.levels[vapeoutState.currentLevel]);
  }
  return 0;
}
void vapeoutScoreText(char *text, uint8_t len) {
    sniprintf(text, len, "S:%"PRIu32, vapeoutState.score);
}

struct textPriv vapeoutScore = {
    .font = FONT_SMALL,
    .len = 5,
    .getText = &vapeoutScoreText
};

const struct displayItem _vapeoutScoreText = {
    .label = "voscore",
    .getDimensions = &textGetDimensions,
    .drawAt = &textDraw,
    .priv = &vapeoutScore
};

void vapeoutLevelText(char *text, uint8_t len) {
    sniprintf(text, len, "%d", vapeoutState.currentLevel + 1);
}

struct textPriv vapeoutLevel = {
    .font = FONT_SMALL,
    .len = 2,
    .getText = &vapeoutLevelText
};

const struct displayItem _vapeoutLevelText = {
    .label = "volevel",
    .getDimensions = &textGetDimensions,
    .drawAt = &textDraw,
    .priv = &vapeoutLevel
};


uint8_t vapeoutLivesGetDimensions(void *priv, uint8_t *args, struct layoutProperties *object) {
    object->W = 15;
    object->H = 0;
    return 0;
}

const struct displayItem _vapeoutLives = {
    .label = "volives",
    .getDimensions = &vapeoutLivesGetDimensions,
    .drawAt = &drawLives,
    .args = 0
};

uint8_t vapeoutGetDimensions(void *priv, uint8_t *args, struct layoutProperties *object) {
    object->W = 63;
    object->H = 78;
    return 0;
}

const struct displayItem _vapeoutField = {
  .label = "vofield",
  .getDimensions = &vapeoutGetDimensions,
  .drawAt = &drawVapeout,
  .args = 0
};

static void __attribute__((constructor)) registerVapeoutDrawables(void) {
  drawables[VAPEOUTSCORE] = &_vapeoutScoreText;
  drawables[VAPEOUTLIVES] = &_vapeoutLives;
  drawables[VAPEOUTLEVEL] = &_vapeoutLevelText;
  drawables[VAPEOUTFIELD] = &_vapeoutField;
}
