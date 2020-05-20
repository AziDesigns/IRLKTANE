//On the Subject of Mazes
#include <Adafruit_NeoMatrix.h>
/*
  KNOWN ISSUES:
  STARTING AND FINISHING POINTS ARE FIXED CURRENTLY
    - IDEALLY THESE WOULD NOT BE FIXED AND WOULD BE RANDOM EVENTUALLY
  CURRENTLY PROGRAMED FOR RED ONLY LED MATRIX. WAITING ON HARDWARE TO WRITE/ UPDATE FOR RGB MATRIX
*/
#define PIN_MAZE_LED_FIN 2,5,2
#define PIN_MAZE_UP A5
#define PIN_MAZE_RIGHT A6
#define PIN_MAZE_DOWN A7
#define PIN_MAZE_LEFT A8
#define PIN_MAZE_DATA 18
#define MAZE_MAX_MODULE_COUNT 1
#define MAZE_BLINK_DELAY 300
#define MAZE_LEDS_BRIGHTNESS 8 // Intensity of the led, a number between 1-15
#define MR 8
#define MC 8

byte mazeButtonLeftState = 0; // current state of the left button
byte mazeButtonRightState = 0; // current state of the right button
byte mazeButtonUpState = 0; // current state of the up button
byte mazeButtonDownState = 0; // current state of the down button
byte lastMazeButtonLeftState = 0; // previous state of the left button
byte lastMazeButtonRightState = 0; // previous state of the right button
byte lastMazeButtonUpState = 0; // previous state of the up button
byte lastMazeButtonDownState = 0; // previous state of the down button

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(MC, MR, PIN_MAZE_DATA, 
  NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE, NEO_GRB + NEO_KHZ800);

// these are for movement & play
const byte MAZE_LEVELS_ROWS = 13; // number of rows of each level
const byte MAZE_LEVELS_COLUMNS = MAZE_LEVELS_ROWS; // number of columns of each level
// these are for matrix display only
const byte MAZE_DISPLAY_LEVELS_ROWS = 8; // number of rows of each level
const byte MAZE_DISPLAY_LEVELS_COLUMNS = MAZE_DISPLAY_LEVELS_ROWS; // number of columns of each level

/*
   'c': green circle position
   's': Start position
   'f': Finish position
   'X': Wall
   ' ': Movement zone
*/

// these are for MOVEMENT/ PLAY
const char MAZE_LEVELS[][MAZE_LEVELS_ROWS][MAZE_LEVELS_COLUMNS] = {
  // Level 1
  {
    {'X','X','X','X','X','X','X','X','X','X','X','X','X'},
    {'X',' ',' ',' ',' ',' ','X',' ',' ',' ',' ',' ','X'},
    {'X',' ','X','X','X',' ','X',' ','X','X','X','X','X'},
    {'X','c','X','s',' ',' ','X','f',' ',' ',' ',' ','X'},
    {'X',' ','X',' ','X','X','X','X','X','X','X',' ','X'},
    {'X',' ','X',' ',' ',' ','X',' ',' ',' ',' ','c','X'},
    {'X',' ','X','X','X',' ','X',' ','X','X','X',' ','X'},
    {'X',' ','X',' ',' ',' ',' ',' ','X',' ',' ',' ','X'},
    {'X',' ','X','X','X','X','X','X','X','X','X',' ','X'},
    {'X',' ',' ',' ',' ',' ','X',' ',' ',' ','X',' ','X'},
    {'X',' ','X','X','X',' ','X',' ','X','X','X',' ','X'},
    {'X',' ',' ',' ','X',' ',' ',' ','X',' ',' ',' ','X'},
    {'X','X','X','X','X','X','X','X','X','X','X','X','X'},
    },
    // Level 2
    {
    {'X','X','X','X','X','X','X','X','X','X','X','X','X'},
    {'X',' ',' ',' ',' ',' ','X',' ',' ',' ',' ',' ','X'},
    {'X','X','X',' ','X','X','X',' ','X',' ','X','X','X'},
    {'X',' ',' ',' ','X',' ',' ',' ','X','c',' ','f','X'},
    {'X',' ','X','X','X',' ','X','X','X','X','X',' ','X'},
    {'X',' ','X',' ',' ',' ','X',' ',' ',' ',' ',' ','X'},
    {'X',' ','X',' ','X','X','X',' ','X','X','X',' ','X'},
    {'X',' ',' ','c','X',' ',' ',' ','X',' ','X',' ','X'},
    {'X',' ','X','X','X',' ','X','X','X',' ','X',' ','X'},
    {'X',' ','X',' ','X',' ','X',' ',' ',' ','X',' ','X'},
    {'X',' ','X',' ','X',' ','X',' ','X','X','X',' ','X'},
    {'X',' ','X',' ',' ',' ','X',' ',' ',' ',' ','s','X'},
    {'X','X','X','X','X','X','X','X','X','X','X','X','X'},
    },
    // Level 3
    {
    {'X','X','X','X','X','X','X','X','X','X','X','X','X'},
    {'X',' ',' ',' ',' ',' ','X',' ','X',' ',' ',' ','X'},
    {'X',' ','X','X','X',' ','X',' ','X',' ','X',' ','X'},
    {'X',' ','X',' ','X',' ','X',' ',' ',' ','X',' ','X'},
    {'X','X','X',' ','X',' ','X','X','X','X','X',' ','X'},
    {'X',' ',' ',' ','X',' ','X',' ',' ',' ','X',' ','X'},
    {'X',' ','X',' ','X',' ','X',' ','X',' ','X',' ','X'},
    {'X',' ','X',' ','X',' ','X','c','X','s','X','c','X'},
    {'X',' ','X',' ','X',' ','X',' ','X',' ','X',' ','X'},
    {'X',' ','X',' ',' ',' ','X',' ','X',' ','X','f','X'},
    {'X',' ','X','X','X','X','X',' ','X',' ','X',' ','X'},
    {'X',' ',' ',' ',' ',' ',' ',' ','X',' ',' ',' ','X'},
    {'X','X','X','X','X','X','X','X','X','X','X','X','X'},
    },
    // Level 4
    {
    {'X','X','X','X','X','X','X','X','X','X','X','X','X'},
    {'X','c',' ',' ','X',' ',' ',' ',' ','f',' ',' ','X'},
    {'X',' ','X',' ','X','X','X','X','X','X','X',' ','X'},
    {'X',' ','X',' ','X',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X',' ','X',' ','X',' ','X','X','X','X','X',' ','X'},
    {'X',' ','X',' ',' ',' ','X',' ',' ',' ','X',' ','X'},
    {'X',' ','X','X','X','X','X',' ','X','X','X',' ','X'},
    {'X','c','X',' ',' ',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X',' ','X','X','X','X','X','X','X','X','X',' ','X'},
    {'X',' ',' ',' ',' ',' ',' ',' ',' ','s','X',' ','X'},
    {'X',' ','X','X','X','X','X','X','X',' ','X',' ','X'},
    {'X',' ',' ',' ',' ',' ','X',' ',' ',' ','X',' ','X'},
    {'X','X','X','X','X','X','X','X','X','X','X','X','X'},
    },
    // Level 5
    {
    {'X','X','X','X','X','X','X','X','X','X','X','X','X'},
    {'X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X','X','X','X','X','X','X','X','X',' ','X',' ','X'},
    {'X',' ',' ',' ',' ',' ',' ','s',' ',' ','X',' ','X'},
    {'X',' ','X','X','X','X','X',' ','X','X','X','X','X'},
    {'X',' ',' ',' ','X',' ',' ',' ','X','c',' ',' ','X'},
    {'X',' ','X',' ','X','X','X','X','X',' ','X',' ','X'},
    {'X',' ','X',' ',' ',' ',' ',' ','X',' ','X',' ','X'},
    {'X',' ','X','X','X','X','X',' ','X','X','X',' ','X'},
    {'X',' ','X',' ',' ',' ',' ','f',' ',' ','X',' ','X'},
    {'X',' ','X',' ','X','X','X','X','X','X','X',' ','X'},
    {'X',' ','X',' ',' ',' ',' ','c',' ',' ',' ',' ','X'},
    {'X','X','X','X','X','X','X','X','X','X','X','X','X'},
    },
    // Level 6
    {
    {'X','X','X','X','X','X','X','X','X','X','X','X','X'},
    {'X',' ','X',' ',' ',' ','X',' ',' ','c',' ',' ','X'},
    {'X',' ','X',' ','X',' ','X','X','X',' ','X',' ','X'},
    {'X',' ','X',' ','X',' ','X',' ',' ',' ','X',' ','X'},
    {'X',' ','X',' ','X',' ','X',' ','X','X','X',' ','X'},
    {'X',' ',' ',' ','X',' ','X',' ','X',' ',' ',' ','X'},
    {'X',' ','X','X','X','X','X',' ','X',' ','X','X','X'},
    {'X',' ',' ',' ','X',' ',' ',' ','X',' ','X',' ','X'},
    {'X','X','X',' ','X',' ','X',' ','X',' ','X',' ','X'},
    {'X',' ',' ','f','X','c','X',' ','X',' ',' ','s','X'},
    {'X',' ','X','X','X','X','X',' ','X','X','X',' ','X'},
    {'X',' ',' ',' ',' ',' ',' ',' ','X',' ',' ',' ','X'},
    {'X','X','X','X','X','X','X','X','X','X','X','X','X'},
    },
    // Level 7
    {
    {'X','X','X','X','X','X','X','X','X','X','X','X','X'},
    {'X',' ',' ','c',' ',' ',' ',' ','X',' ',' ',' ','X'},
    {'X',' ','X','X','X','X','X',' ','X',' ','X',' ','X'},
    {'X',' ','X',' ',' ',' ','X',' ',' ',' ','X',' ','X'},
    {'X',' ','X',' ','X','X','X','X','X','X','X',' ','X'},
    {'X',' ',' ',' ','X',' ',' ',' ','X','s',' ',' ','X'},
    {'X','X','X','X','X',' ','X','X','X',' ','X','X','X'},
    {'X',' ',' ',' ','X',' ',' ',' ',' ',' ','X',' ','X'},
    {'X',' ','X',' ','X',' ','X','X','X','X','X',' ','X'},
    {'X',' ','X',' ','X',' ',' ',' ',' ',' ','X','f','X'},
    {'X',' ','X','X','X','X','X','X','X',' ','X',' ','X'},
    {'X',' ',' ','c',' ',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X','X','X','X','X','X','X','X','X','X','X','X','X'},
    },
    // Level 8
    {
    {'X','X','X','X','X','X','X','X','X','X','X','X','X'},
    {'X',' ','X','f',' ',' ',' ','c','X',' ',' ',' ','X'},
    {'X',' ','X',' ','X','X','X',' ','X',' ','X',' ','X'},
    {'X',' ',' ',' ',' ',' ','X',' ',' ',' ','X',' ','X'},
    {'X',' ','X','X','X','X','X','X','X','X','X',' ','X'},
    {'X',' ','X',' ',' ',' ',' ',' ',' ',' ','X',' ','X'},
    {'X',' ','X',' ','X','X','X','X','X',' ','X',' ','X'},
    {'X',' ','X',' ',' ','c','X',' ',' ',' ',' ',' ','X'},
    {'X',' ','X','X','X',' ','X','X','X','X','X','X','X'},
    {'X',' ','X',' ','X',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X',' ','X',' ','X','X','X','X','X','X','X','X','X'},
    {'X',' ',' ',' ',' ','s',' ',' ',' ',' ',' ',' ','X'},
    {'X','X','X','X','X','X','X','X','X','X','X','X','X'},
    },
    // Level 9
    {
    {'X','X','X','X','X','X','X','X','X','X','X','X','X'},
    {'X',' ','X',' ',' ',' ',' ','f',' ',' ',' ',' ','X'},
    {'X',' ','X',' ','X','X','X','X','X',' ','X',' ','X'},
    {'X',' ','X',' ','X','c',' ',' ','X',' ','X',' ','X'},
    {'X',' ','X',' ','X',' ','X','X','X',' ','X',' ','X'},
    {'X',' ',' ',' ',' ',' ','X',' ',' ',' ','X',' ','X'},
    {'X',' ','X','X','X','X','X',' ','X','X','X',' ','X'},
    {'X',' ','X',' ','X',' ',' ',' ','X',' ',' ',' ','X'},
    {'X',' ','X',' ','X',' ','X','X','X','X','X',' ','X'},
    {'X','c','X',' ','X',' ','X',' ',' ','s','X',' ','X'},
    {'X',' ','X',' ','X',' ','X',' ',' ',' ','X','X','X'},
    {'X',' ',' ',' ','X',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X','X','X','X','X','X','X','X','X','X','X','X','X'},
    }
};

// these are for DISPLAY ONLY
const char MAZE_DISPLAY_LEVELS[][MAZE_DISPLAY_LEVELS_ROWS][MAZE_DISPLAY_LEVELS_COLUMNS] = {
  // Level 1
  {
    {'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X'},
    {'X', ' ', ' ', ' ', ' ', ' ', ' ', 'X'},
    {'X', 'c', 's', ' ', 'f', ' ', ' ', 'X'},
    {'X', ' ', ' ', ' ', ' ', ' ', 'c', 'X'},
    {'X', ' ', ' ', ' ', ' ', ' ', ' ', 'X'},
    {'X', ' ', ' ', ' ', ' ', ' ', ' ', 'X'},
    {'X', ' ', ' ', ' ', ' ', ' ', ' ', 'X'},
    {'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X'},
  },
  // Level 2
  {
    {'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X'},
    {'X', ' ', ' ', ' ', ' ', ' ', ' ', 'X'},
    {'X', ' ', ' ', ' ', ' ', 'c', 'f', 'X'},
    {'X', ' ', ' ', ' ', ' ', ' ', ' ', 'X'},
    {'X', ' ', 'c', ' ', ' ', ' ', ' ', 'X'},
    {'X', ' ', ' ', ' ', ' ', ' ', ' ', 'X'},
    {'X', ' ', ' ', ' ', ' ', ' ', 's', 'X'},
    {'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X'},
  },
  // Level 3
  {
    {'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X'},
    {'X', ' ', ' ', ' ', ' ', ' ', ' ', 'X'},
    {'X', ' ', ' ', ' ', ' ', ' ', ' ', 'X'},
    {'X', ' ', ' ', ' ', ' ', ' ', ' ', 'X'},
    {'X', ' ', ' ', ' ', 'c', 's', 'c', 'X'},
    {'X', ' ', ' ', ' ', ' ', ' ', 'f', 'X'},
    {'X', ' ', ' ', ' ', ' ', ' ', ' ', 'X'},
    {'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X'},
  },
  // Level 4
  {
    {'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X'},
    {'X', 'c', ' ', ' ', ' ', 'f', ' ', 'X'},
    {'X', ' ', ' ', ' ', ' ', ' ', ' ', 'X'},
    {'X', ' ', ' ', ' ', ' ', ' ', ' ', 'X'},
    {'X', 'c', ' ', ' ', ' ', ' ', ' ', 'X'},
    {'X', ' ', ' ', ' ', ' ', 's', ' ', 'X'},
    {'X', ' ', ' ', ' ', ' ', ' ', ' ', 'X'},
    {'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X'},
  },
  // Level 5
  {
    {'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X'},
    {'X', ' ', ' ', ' ', ' ', ' ', ' ', 'X'},
    {'X', ' ', ' ', ' ', 's', ' ', ' ', 'X'},
    {'X', ' ', ' ', ' ', ' ', 'c', ' ', 'X'},
    {'X', ' ', ' ', ' ', ' ', ' ', ' ', 'X'},
    {'X', ' ', ' ', ' ', 'f', ' ', ' ', 'X'},
    {'X', ' ', ' ', ' ', 'c', ' ', ' ', 'X'},
    {'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X'},
  },
  // Level 6
  {
    {'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X'},
    {'X', ' ', ' ', ' ', ' ', 'c', ' ', 'X'},
    {'X', ' ', ' ', ' ', ' ', ' ', ' ', 'X'},
    {'X', ' ', ' ', ' ', ' ', ' ', ' ', 'X'},
    {'X', ' ', ' ', ' ', ' ', ' ', ' ', 'X'},
    {'X', ' ', 'f', 'c', ' ', ' ', 's', 'X'},
    {'X', ' ', ' ', ' ', ' ', ' ', ' ', 'X'},
    {'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X'},
  },
  // Level 7
  {
    {'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X'},
    {'X', ' ', 'c', ' ', ' ', ' ', ' ', 'X'},
    {'X', ' ', ' ', ' ', ' ', ' ', ' ', 'X'},
    {'X', ' ', ' ', ' ', ' ', 's', ' ', 'X'},
    {'X', ' ', ' ', ' ', ' ', ' ', ' ', 'X'},
    {'X', ' ', ' ', ' ', ' ', ' ', 'f', 'X'},
    {'X', ' ', 'c', ' ', ' ', ' ', ' ', 'X'},
    {'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X'},
  },
  // Level 8
  {
    {'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X'},
    {'X', ' ', 'f', ' ', 'c', ' ', ' ', 'X'},
    {'X', ' ', ' ', ' ', ' ', ' ', ' ', 'X'},
    {'X', ' ', ' ', ' ', ' ', ' ', ' ', 'X'},
    {'X', ' ', ' ', 'c', ' ', ' ', ' ', 'X'},
    {'X', ' ', ' ', ' ', ' ', ' ', ' ', 'X'},
    {'X', ' ', ' ', 's', ' ', ' ', ' ', 'X'},
    {'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X'},
  },
  // Level 9
  {
    {'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X'},
    {'X', ' ', ' ', ' ', 'f', ' ', ' ', 'X'},
    {'X', ' ', ' ', 'c', ' ', ' ', ' ', 'X'},
    {'X', ' ', ' ', ' ', ' ', ' ', ' ', 'X'},
    {'X', ' ', ' ', ' ', ' ', ' ', ' ', 'X'},
    {'X', 'c', ' ', ' ', ' ', 's', ' ', 'X'},
    {'X', ' ', ' ', ' ', ' ', ' ', ' ', 'X'},
    {'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X'},
  }
};
  
/**
   GAME STATE
*/
byte mazeCurrentLevel; // Current level paying // FOR TESTING USE = 0;
byte mazeCurrentX; // Current player X position
byte mazeCurrentY; // Current player Y position
byte mazeStartX; // Current level start X position
byte mazeStartY; // Current level start Y position
byte mazeFinishX; // Current level finish X position
byte mazeFinishY; // Current level finish Y position
byte mazeDisplayCurrentX; // Current player X DISPLAY position
byte mazeDisplayCurrentY; // Current player Y DISPLAY position
byte mazeDisplayStartX; // Current DISPLAY level start X position
byte mazeDisplayStartY; // Current DISPLAY level start Y position
byte mazeDisplayFinishX; // Current level finish X position
byte mazeDisplayFinishY; // Current level finish Y position
enum mazeMove { Left, Right, Up, Down }; // Possible directions to move the player

void mazePrintLevel() // DISPLAYS THE FRIENDLY VERSION NOT THE MOVEMENT/ PLAY VERSION
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  for (byte x = 0; x < MAZE_DISPLAY_LEVELS_ROWS; x++) {
    for (byte y = 0; y < MAZE_DISPLAY_LEVELS_COLUMNS; y++) {
      if (MAZE_DISPLAY_LEVELS[mazeCurrentLevel][x][y] == 'c') {
        matrix.drawPixel(y, x, matrix.Color(0, 30, 0)); //green 
        matrix.show();
      }
      else if (MAZE_DISPLAY_LEVELS[mazeCurrentLevel][x][y] == 'f') {
        matrix.drawPixel(y, x, matrix.Color(30, 0, 0)); //red  
        matrix.show();
      }
    }
  }
}

void clearMatrix() {  
  for (int i = 0; i < MR; i++) {
    for (int j = 0; j < MC; j++) {
      matrix.drawPixel(j, i, matrix.Color(0, 0, 0));      
    }
  }
  matrix.show();
}

void mazeInitLevel() 
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  clearMatrix();

  // sets startX, startY, finishX and finishY for MOVEMENT/ PLAY
  for (byte x = 0; x < MAZE_LEVELS_ROWS; x++) {
    for (byte y = 0; y < MAZE_LEVELS_COLUMNS; y++) {
      if (MAZE_LEVELS[mazeCurrentLevel][x][y] == 's') {
        mazeStartX = x;
        mazeStartY = y;
      } else if (MAZE_LEVELS[mazeCurrentLevel][x][y] == 'f') {
        mazeFinishX = x;
        mazeFinishY = y;
      }
    }
  }
  // sets startX, startY, finishX and finishY for DISPLAY
  for (byte x = 0; x < MAZE_DISPLAY_LEVELS_ROWS; x++) {
    for (byte y = 0; y < MAZE_DISPLAY_LEVELS_COLUMNS; y++) {
      if (MAZE_DISPLAY_LEVELS[mazeCurrentLevel][x][y] == 's') {
        mazeDisplayStartX = x;
        mazeDisplayStartY = y;
      } else if (MAZE_DISPLAY_LEVELS[mazeCurrentLevel][x][y] == 'f') { 
        mazeDisplayFinishX = x;
        mazeDisplayFinishY = y;
      }
    }
  }

  mazeCurrentX = mazeStartX;
  mazeCurrentY = mazeStartY;
  mazeDisplayCurrentX = mazeDisplayStartX;
  mazeDisplayCurrentY = mazeDisplayStartY;

  mazePrintLevel();
}

void mazeShowPlayer() 
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  matrix.drawPixel(mazeDisplayCurrentY, mazeDisplayCurrentX, matrix.Color(30, 30, 30)); //white    
  matrix.show();
}

void mazeHidePlayer() 
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  matrix.drawPixel(mazeDisplayCurrentY, mazeDisplayCurrentX, matrix.Color(0, 0, 0)); //off      
  matrix.show();
}

void mazeCheckWin() 
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  if (MAZE_LEVELS[mazeCurrentLevel][mazeCurrentX][mazeCurrentY] == 'f') {
    clearMatrix();
    lc.setLed(PIN_MAZE_LED_FIN,true);
    mazeModuleDefused = true;
    defusedModuleBuzzer();
    isAnyModuleDefused=true;
  }
}

/**
   Tries to move the player to a given direction.
   The movement is allowed based on MIN_CURRENT and MAX_CURRENT values
   and also if the desired movement is towards an allowed movement placed, i.e. 'X', 's', 'f'
   Returns true if the movement was successful, otherwise false
*/
void mazeTryToMove(mazeMove movement) 
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  byte mazePossibleX = mazeCurrentX;
  byte mazePossibleY = mazeCurrentY;

  if (movement == Left) {
    mazeHidePlayer();
    mazePossibleY--;
    // Checks what would happen if we apply the possible new position
    if (MAZE_LEVELS[mazeCurrentLevel][mazePossibleX][mazePossibleY] != 'X') {
      mazeCurrentY = mazePossibleY - 1;
      mazeDisplayCurrentY = mazeDisplayCurrentY - 1;
    }
    else addStrike();
    mazeShowPlayer();
    mazeCheckWin();
  } else if (movement == Right) {
    mazeHidePlayer();
    mazePossibleY++;
    // Checks what would happen if we apply the possible new position
    if (MAZE_LEVELS[mazeCurrentLevel][mazePossibleX][mazePossibleY] != 'X') {
      mazeCurrentY = mazePossibleY + 1;
      mazeDisplayCurrentY = mazeDisplayCurrentY + 1;
    }
    else addStrike();
    mazeShowPlayer();
    mazeCheckWin();
  } else if (movement == Up) {
    mazeHidePlayer();
    mazePossibleX--;
    // Checks what would happen if we apply the possible new position
    if (MAZE_LEVELS[mazeCurrentLevel][mazePossibleX][mazePossibleY] != 'X') {
      mazeCurrentX = mazePossibleX - 1;
      mazeDisplayCurrentX = mazeDisplayCurrentX - 1;
    }
    else addStrike();
    mazeShowPlayer();
    mazeCheckWin();
  } else if (movement == Down) {
    mazeHidePlayer();
    mazePossibleX++;
    // Checks what would happen if we apply the possible new position
    if (MAZE_LEVELS[mazeCurrentLevel][mazePossibleX][mazePossibleY] != 'X') {
      mazeCurrentX = mazePossibleX + 1;
      mazeDisplayCurrentX = mazeDisplayCurrentX + 1;
    }
    else addStrike();
    mazeShowPlayer();
    mazeCheckWin();
  }
}

void mazeSetup() 
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  mazeCurrentLevel = random(9);
  if (DEBUG_LEVEL >= 1) {
    Serial.println(F("Maze Level Number 0-8: "));
    Serial.println (mazeCurrentLevel);
  }
  //Setup LedMatrix
  matrix.begin();
  matrix.show(); // initialize all pixels to 'off'  
  mazeInitLevel();
}

void mazeLoop() 
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }

  mazeShowPlayer();

  if (!mazeModuleDefused) {

    // read the pushbutton input pins:
    mazeButtonLeftState = digitalRead(PIN_MAZE_LEFT);
    mazeButtonRightState = digitalRead(PIN_MAZE_RIGHT);
    mazeButtonUpState = digitalRead(PIN_MAZE_UP);
    mazeButtonDownState = digitalRead(PIN_MAZE_DOWN);

    // compare the mazeButtonLeftState to its previous state
    if (mazeButtonLeftState != lastMazeButtonLeftState) {
      if (mazeButtonLeftState == HIGH) {
        if (DEBUG_LEVEL >= 1) {
          Serial.println(F("mazeLeftOn"));
        }
        mazeTryToMove(Left);
      } else {
        if (DEBUG_LEVEL >= 1) {
          Serial.println(F("mazeLeftOff"));
        }
      }
    }
    // save the current state as the last state, for next time through the loop
    lastMazeButtonLeftState = mazeButtonLeftState;

    // compare the mazeButtonRightState to its previous state
    if (mazeButtonRightState != lastMazeButtonRightState) {
      if (mazeButtonRightState == HIGH) {
        if (DEBUG_LEVEL >= 1) {
          Serial.println(F("mazeRightOn"));
        }
        mazeTryToMove(Right);
      } else {
        if (DEBUG_LEVEL >= 1) {
          Serial.println(F("mazeRightOff"));
        }
      }
    }
    // save the current state as the last state, for next time through the loop
    lastMazeButtonRightState = mazeButtonRightState;

    // compare the mazeButtonUpState to its previous state
    if (mazeButtonUpState != lastMazeButtonUpState) {
      if (mazeButtonUpState == HIGH) {
        if (DEBUG_LEVEL >= 1) {
          Serial.println(F("mazeUpOn"));
        }
        mazeTryToMove(Up);
      } else {
        if (DEBUG_LEVEL >= 1) {
          Serial.println(F("mazeUpOff"));
        }
      }
    }
    // save the current state as the last state, for next time through the loop
    lastMazeButtonUpState = mazeButtonUpState;

    // compare the mazeButtonDownState to its previous state
    if (mazeButtonDownState != lastMazeButtonDownState) {
      if (mazeButtonDownState == HIGH) {
        if (DEBUG_LEVEL >= 1) {
          Serial.println(F("mazeDownOn"));
        }
        mazeTryToMove(Down);
      } else {
        if (DEBUG_LEVEL >= 1) {
          Serial.println(F("mazeDownOff"));
        }
      }
    }
    // save the current state as the last state, for next time through the loop
    lastMazeButtonDownState = mazeButtonDownState;

  }
}

void mazeModuleBoom()
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  clearMatrix();
  lc.setLed(PIN_MAZE_LED_FIN,false);
}
