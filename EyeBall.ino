


#include <Arduboy2.h>
#include "Images.h"

Arduboy2 arduboy;

const unsigned char background[] PROGMEM  = {
    0xf1, 0x11, 0x11, 0x11, 0x1f, 0x11, 0x11, 0x11, 
};
const unsigned char player[] PROGMEM  = {
0xe0, 0xf8, 0xfc, 0x7e, 0x1e, 0x1f, 0xf, 0xf, 0xf, 0x3f, 0x3f, 0xfe, 0xfe, 0xfc, 0xf8, 0xe0, 0x7, 0x1f, 0x3f, 0x7e, 0x78, 0xf8, 0xf0, 0xf0, 0xf8, 0xf8, 0xfe, 0x7f, 0x7f, 0x3f, 0x1f, 0x7, 
};

const unsigned char eyeLeft2[] PROGMEM  = {
0xe0, 0xf8, 0x3c, 0x1e, 0xe, 0xf, 0xf, 0x1f, 0x3f, 0xff, 0xff, 0xfe, 0xfe, 0xfc, 0xf8, 0xe0, 0x7, 0x1f, 0x3c, 0x78, 0x70, 0xf0, 0xf0, 0xf8, 0xfc, 0xff, 0xff, 0x7f, 0x7f, 0x3f, 0x1f, 0x7, 
};

const unsigned char eyeLeft1[] PROGMEM  = {
0xe0, 0xf8, 0xfc, 0x7e, 0x1e, 0x1f, 0xf, 0xf, 0xf, 0x3f, 0x3f, 0xfe, 0xfe, 0xfc, 0xf8, 0xe0, 0x7, 0x1f, 0x3f, 0x7e, 0x78, 0xf8, 0xf0, 0xf0, 0xf8, 0xf8, 0xfe, 0x7f, 0x7f, 0x3f, 0x1f, 0x7, 
};

const unsigned char eye00[] PROGMEM  = {
0xe0, 0xf8, 0xfc, 0xfe, 0x7e, 0x1f, 0x1f, 0xf, 0xf, 0x3f, 0x3f, 0x7e, 0xfe, 0xfc, 0xf8, 0xe0, 0x7, 0x1f, 0x3f, 0x7f, 0x7e, 0xf8, 0xf8, 0xf0, 0xf0, 0xf8, 0xf8, 0x7e, 0x7f, 0x3f, 0x1f, 0x7, 
};

const unsigned char eyeRight1[] PROGMEM  = {
0xe0, 0xf8, 0xfc, 0xfe, 0xfe, 0xff, 0x7f, 0x1f, 0x1f, 0x3f, 0x3f, 0x1e, 0x1e, 0x7c, 0xf8, 0xe0, 0x7, 0x1f, 0x3f, 0x7f, 0x7f, 0xff, 0xfe, 0xf8, 0xf8, 0xf0, 0xf0, 0x78, 0x78, 0x3e, 0x1f, 0x7, 
};

const unsigned char eyeRight2[] PROGMEM  = {
0xe0, 0xf8, 0xfc, 0xfe, 0xfe, 0xff, 0xff, 0x7f, 0x1f, 0x3f, 0x3f, 0x1e, 0x1e, 0x1c, 0x78, 0xe0, 0x7, 0x1f, 0x3f, 0x7f, 0x7f, 0xff, 0xff, 0xfe, 0xf8, 0xf8, 0xf0, 0x70, 0x78, 0x38, 0x1e, 0x7, 
};

enum GameStatus {
  Introduction,
  PlayGame,
  GameOver,
};

enum Stance {
  CenterMiddle,
  Left1,
  Left2,
  Right1,
  Right2
};

struct Eyeball {
  int x;
  int y;
  Stance stance;
  const byte *image;
  const byte *mask;
};

int randomnumber;
int spriteX = 1;
int spriteY = 1;
int xPos = 0;
int xIncrementer = 0;

Eyeball eyeball = {spriteX, spriteY, Stance::CenterMiddle, eye_00, eye_mask };

GameStatus gameStatus = Introduction;

const byte *eyeball_images[] = { eye_left_2, eye_left_1, eye_00, eye_right_1, eye_right_2 };
const byte *eyeball_masks[] = { eye_mask };


void setup() {
  //initEEPROM();
  arduboy.boot();
  arduboy.setFrameRate(75);
  arduboy.initRandomSeed();
}

void loop() {
  if (!(arduboy.nextFrame())) return;
  arduboy.pollButtons();

  switch (gameStatus) {

    case GameStatus::Introduction:
      introduction();
      break;

    case GameStatus::PlayGame:
      playGame();
      break;

    case GameStatus::GameOver:
      gameOver();
      break;
      
  }
  //randomnumber = 1 + rand() % 100;
  //arduboy.clear();

  //arduboy.setCursor(50, 50);
  //arduboy.print(randomnumber);

}

void drawEyeball()
{
  arduboy.fillRect(spriteX, spriteY, 16, 16, BLACK);
  switch(xPos)
  {
    case -2:
      arduboy.drawBitmap(spriteX, spriteY, eyeLeft2, 16, 16, WHITE);
      break;
    case -1:
      arduboy.drawBitmap(spriteX, spriteY, eyeLeft1, 16, 16, WHITE);
      break;
    case 0:
      arduboy.drawBitmap(spriteX, spriteY, eye00, 16, 16, WHITE);
      break;
    case 1:
      arduboy.drawBitmap(spriteX, spriteY, eyeRight1, 16, 16, WHITE);
      break;
    case 2:
      arduboy.drawBitmap(spriteX, spriteY, eyeRight2, 16, 16, WHITE);
      break;
  }

}

void drawBackground()
{
    for( int backgroundx = 0; backgroundx < 128; backgroundx = backgroundx + 8 ) {
        //For each row in the column
        for( int backgroundy = 0; backgroundy < 64; backgroundy = backgroundy + 8 ) {
                //Draw a background tile
            arduboy.drawBitmap( backgroundx, backgroundy, background, 8, 8, WHITE );
        }
    }
}

void introduction() {

  //EEPROM.get(EEPROM_SCORE, highScore);
  arduboy.clear();

  initialiseGame();

  arduboy.setCursor(17, 12);
  arduboy.print(F("Press A to begin"));

  //drawBackground();
  drawEyeball();

  arduboy.display();
    
  if (arduboy.pressed(A_BUTTON)) {
    
    gameStatus = GameStatus::PlayGame; 
    eyeball.stance = Stance::CenterMiddle;
  
  }
  
}

void gameOver() {

  arduboy.clear();
  
  arduboy.setCursor(40, 12);
  arduboy.print(F("Game Over"));

  
  arduboy.display();
    
  if (arduboy.justPressed(A_BUTTON)) { 
  
    initialiseGame();

    gameStatus = GameStatus::PlayGame; 
    eyeball.stance = Stance::CenterMiddle;

  }
}

void initialiseGame()
{
  arduboy.clear();
  eyeball.stance = Stance::CenterMiddle;
  drawBackground();
  
}

void playGame()
{
  
  if(arduboy.pressed(LEFT_BUTTON)) {
      
      xIncrementer = xIncrementer - 1;
      
      xPos = xPos - 1;
      if(xPos < -2)
      {
        xPos = -2;
      }
      spriteX = spriteX - 1;
  }
  if(arduboy.pressed(RIGHT_BUTTON)) {
      xIncrementer = xIncrementer + 1;
      
      xPos = xPos + 1;
      if(xPos > 2)
      {
        xPos = 2;
      }

      spriteX = spriteX + 1;
  }
  if(arduboy.pressed(UP_BUTTON)) {
      spriteY = spriteY - 1;
  }
  if(arduboy.pressed(DOWN_BUTTON)) {
      spriteY = spriteY + 1;
  }
  drawBackground();
  drawEyeball();
  
  arduboy.display();
}




