


#include <Arduboy2.h>
#include "Images.h"
Arduboy2 arduboy;

const unsigned char background[] PROGMEM  = {
    0xf1, 0x11, 0x11, 0x11, 0x1f, 0x11, 0x11, 0x11, 
};

enum GameStatus {
  Introduction,
  PlayGame,
  GameOver,
};

enum Stance {
  Left2,
  Left1,
  CenterMiddle,
  Right1,
  Right2,
  Big
};

/*
enum Stance {
  eye_00,
  eye_left_1,
  eye_left_2,
  eye_right_1,
  eye_right_2
};
*/

const byte *eyeball_images[] = { eye_left_2, eye_left_1, eye_00, eye_right_1, eye_right_2, eye_big };
const byte *eyeball_masks[] = { eye_mask, eye_mask, eye_mask, eye_mask, big_eye_mask };

struct Eyeball {
  int x;
  int y;
  Stance stance;
  const byte *image;
  const byte *mask;
};

int randomnumber;
int spriteX = 10;
int spriteY = 10;
int xPos = 0;
int xIncrementer = 0;

Eyeball eyeball = {spriteX, spriteY, Stance::CenterMiddle, eye_00, eye_mask };

GameStatus gameStatus = Introduction;

void setup() {
  //initEEPROM();
  arduboy.boot();
  arduboy.setFrameRate(15);
  arduboy.initRandomSeed();
}

void loop() {
  if (!(arduboy.nextFrame()))
  {
    //randomnumber = 1 + rand() % 100;
    //arduboy.clear();
    //arduboy.setCursor(50, 50);
    //arduboy.print(randomnumber);
    return;
  }
  
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
}

void drawEyeball()
{
    Sprites::drawExternalMask(eyeball.x, eyeball.y - getImageHeight(eyeball.image), eyeball.image, eyeball.mask, 0, 0);//0,0 = frame frame
}

//void drawEyeball()
//{
//  arduboy.fillRect(spriteX, spriteY, 16, 16, BLACK);
//  
//  switch(xPos)
//  {
//    case -2:
//      arduboy.drawBitmap(spriteX, spriteY, eyeLeft2, 16, 16, WHITE);
//      break;
//    case -1:
//      arduboy.drawBitmap(spriteX, spriteY, eyeLeft1, 16, 16, WHITE);
//      break;
//    case 0:
//      arduboy.drawBitmap(spriteX, spriteY, eye00, 16, 16, WHITE);
//      break;
//    case 1:
//      arduboy.drawBitmap(spriteX, spriteY, eyeRight1, 16, 16, WHITE);
//      break;
//    case 2:
//      arduboy.drawBitmap(spriteX, spriteY, eyeRight2, 16, 16, WHITE);
//      break;
//  }
//
//}

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
  //drawEyeball();
  drawEyeball();

  arduboy.display();
    
  if (arduboy.pressed(A_BUTTON)) {
    arduboy.clear();
    gameStatus = GameStatus::PlayGame; 
    eyeball.stance = Stance::CenterMiddle;
  }

  if (arduboy.pressed(B_BUTTON)) {
    arduboy.clear();
    //gameStatus = GameStatus::PlayGame; 
    eyeball.stance = Stance::Big;
    drawBackground();
    arduboy.display();
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

  if (arduboy.pressed(B_BUTTON)) {
    //gameStatus = GameStatus::PlayGame; 
    eyeball.stance = Stance::Big;
  }
}

void initialiseGame()
{
  arduboy.clear();
  eyeball.stance = Stance::CenterMiddle;
  drawBackground();
  
}

void updateEyeball()
{
  //update these eyeball.x, eyeball.y - getImageHeight(eyeball.image), eyeball.image
  eyeball.x = spriteX;
  eyeball.y = spriteY;

  //which image
    switch(xPos)
  {
    case -2:
      //arduboy.drawBitmap(spriteX, spriteY, eyeLeft2, 16, 16, WHITE);
      eyeball.stance = Stance::Left2;

      break;
    case -1:
      //arduboy.drawBitmap(spriteX, spriteY, eyeLeft1, 16, 16, WHITE);
      eyeball.stance = Stance::Left1;
      break;
    case 0:
      //arduboy.drawBitmap(spriteX, spriteY, eye00, 16, 16, WHITE);
      eyeball.stance = Stance::CenterMiddle;
      break;
    case 1:
      //arduboy.drawBitmap(spriteX, spriteY, eyeRight1, 16, 16, WHITE);
      eyeball.stance = Stance::Right1;
      break;
    case 2:
      //arduboy.drawBitmap(spriteX, spriteY, eyeRight2, 16, 16, WHITE);
      eyeball.stance = Stance::Right2;
      break;
  }
      UpdateEyeballImageByStance();
}

void UpdateEyeballImageByStance()
{
  eyeball.image = eyeball_images[eyeball.stance];
  eyeball.mask = eyeball_masks[eyeball.stance];
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
    if(arduboy.pressed(DOWN_BUTTON)) {
      spriteY = spriteY + 1;
  }
  
  updateEyeball();
  
  if (arduboy.pressed(A_BUTTON)) {
    arduboy.clear();
    eyeball.stance = Stance::Big;
    UpdateEyeballImageByStance();
  }
  if (arduboy.pressed(B_BUTTON)) {
    //gameStatus = GameStatus::PlayGame; 
    eyeball.stance = Stance::Big;
    UpdateEyeballImageByStance();
    drawBackground();
  }
  
  arduboy.clear();
  //drawBackground();
  drawEyeball();
  arduboy.display();
}





