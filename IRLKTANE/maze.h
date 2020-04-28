//On the Subject of Mazes
/*
  KNOWN ISSUES:
  I AM OUT OF BUTTONS SO HAD TO REUSE BUTTONS FROM THE MORSE AND BUTTON MODULE. WHEN NEW BUTTONS ARRIVE I WILL FIX AND POINT TO CORRECT PINS
  STARTING AND FINISHING POINTS ARE FIXED CURRENTLY
    - IDEALLY THESE WOULD NOT BE FIXED AND WOULD BE RANDOM EVENTUALLY
  CURRENTLY PROGRAMED FOR RED ONLY LED MATRIX. WAITING ON HARDWARE TO WRITE/ UPDATE FOR RGB
/*
   The Maze module is a module that consists of a 6x6 grid of squares with two of the squares containing green indicator circles,
   one square containing a white square, and one square containing a red triangle as well as four directional buttons around the maze.
   In order to disarm the module, the Defuser must guide the white light to the red triangle without running into any of the walls shown in the manual.
   The manual page consists of nine mazes, one of which will be the maze present on the module.
   The Defuser must communicate the positions of the green circles to the Expert to identify the correct maze,
   and the expert must guide the Defuser's white light to the red triangle to defuse the module.

   We wont be able to make a green circle around the 2 indicators but can light up the grid in green/ red/ and white for this module.
   We also cant get a 6x6 matrix unless we make it ourselves. Alt we use a 8x8 grid and black out the 1 line around all sides when mounting.
*/

/////////////////////////////////
/////////////////////////////////

#include "LedControl.h"

/*
  SETUP FOR A MAZE
*/

#define PIN_MAZE_LED_GREEN 44
#define PIN_MAZE_UP A5
#define PIN_MAZE_RIGHT A6
#define PIN_MAZE_DOWN A7
#define PIN_MAZE_LEFT A8

int mazeButtonLeftState = 0; // current state of the left button
int mazeButtonRightState = 0; // current state of the right button
int mazeButtonUpState = 0; // current state of the up button
int mazeButtonDownState = 0; // current state of the down button
int lastMazeButtonLeftState = 0; // previous state of the left button
int lastMazeButtonRightState = 0; // previous state of the right button
int lastMazeButtonUpState = 0; // previous state of the up button
int lastMazeButtonDownState = 0; // previous state of the down button

#define MAZE_OUTPUT_DATA_PIN 12
#define MAZE_OUTPUT_LOAD_PIN 10
#define MAZE_OUTPUT_CLOCK_PIN 3
#define MAZE_MAX_MODULE_COUNT 1

const int MAZE_LEDS_BRIGHTNESS = 8; // Intensity of the led, a number between 1-15

#define MAZE_DATA_ST 0
#define MAZE_DATA_FIN 1
#define MAZE_DATA_PT1 2
#define MAZE_DATA_PT2 3
#define MAZE_DATA_MZ 4

#define MAZE_BLINK_DELAY 300

LedControl lc = LedControl(MAZE_OUTPUT_DATA_PIN, MAZE_OUTPUT_CLOCK_PIN, MAZE_OUTPUT_LOAD_PIN, MAZE_MAX_MODULE_COUNT);

/**
   GAME
*/
// these are for movement/ play
const int MAZE_LEVELS_ROWS = 13; // number of rows of each level
const int MAZE_LEVELS_COLUMNS = MAZE_LEVELS_ROWS; // number of columns of each level
// these are for matrix display only
const int MAZE_DISPLAY_LEVELS_ROWS = 8; // number of rows of each level
const int MAZE_DISPLAY_LEVELS_COLUMNS = MAZE_DISPLAY_LEVELS_ROWS; // number of columns of each level

/**
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
int mazeCurrentLevel; // Current level paying // FOR TESTING USE = 0;
int mazeCurrentX; // Current player X position
int mazeCurrentY; // Current player Y position
int mazeStartX; // Current level start X position
int mazeStartY; // Current level start Y position
int mazeFinishX; // Current level finish X position
int mazeFinishY; // Current level finish Y position
int mazeDisplayCurrentX; // Current player X DISPLAY position
int mazeDisplayCurrentY; // Current player Y DISPLAY position
int mazeDisplayStartX; // Current DISPLAY level start X position
int mazeDisplayStartY; // Current DISPLAY level start Y position
int mazeDisplayFinishX; // Current level finish X position
int mazeDisplayFinishY; // Current level finish Y position
enum mazeMove { Left, Right, Up, Down }; // Possible directions to move the player

void mazePrintLevel() // DISPLAYS THE FRIENDLY VERSION NOT THE MOVEMENT/ PLAY VERSION
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  for (int x = 0; x < MAZE_DISPLAY_LEVELS_ROWS; x++) {
    for (int y = 0; y < MAZE_DISPLAY_LEVELS_COLUMNS; y++) {
      if (MAZE_DISPLAY_LEVELS[mazeCurrentLevel][x][y] == 'c') {
        lc.setLed(0, x, y, true);
      }
      else if (MAZE_DISPLAY_LEVELS[mazeCurrentLevel][x][y] == 'f') {
        lc.setLed(0, x, y, true);
      }
    }
  }
}

void mazeInitLevel() 
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  lc.clearDisplay(0);

  // sets startX, startY, finishX and finishY for MOVEMENT/ PLAY
  for (int x = 0; x < MAZE_LEVELS_ROWS; x++) {
    for (int y = 0; y < MAZE_LEVELS_COLUMNS; y++) {
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
  for (int x = 0; x < MAZE_DISPLAY_LEVELS_ROWS; x++) {
    for (int y = 0; y < MAZE_DISPLAY_LEVELS_COLUMNS; y++) {
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
  lc.setLed(0, mazeDisplayCurrentX, mazeDisplayCurrentY, true);
}

void mazeHidePlayer() 
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  lc.setLed(0, mazeDisplayCurrentX, mazeDisplayCurrentY, false);
}

void mazeCheckWin() 
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  if (MAZE_LEVELS[mazeCurrentLevel][mazeCurrentX][mazeCurrentY] == 'f') {
    lc.clearDisplay(0);
    digitalWrite(PIN_MAZE_LED_GREEN, HIGH);
    mazeModuleDefused = true;
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
  int mazePossibleX = mazeCurrentX;
  int mazePossibleY = mazeCurrentY;

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
    Serial.println("Maze Level Number 0-8: ");
    Serial.println (mazeCurrentLevel);
  }
  //Setup Led
  /*
    The MAX72XX is in power-saving mode on startup,
    we have to do a wakeup call
  */
  lc.shutdown(0, false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0, MAZE_LEDS_BRIGHTNESS);
  /* and clear the display */
  lc.clearDisplay(0);
  mazeInitLevel();
}

void mazeLoop() 
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }

  mazeShowPlayer();

  if (!mazeModuleDefused) {

    /**
       NOTE: The following movement directions are setted supposing
       the LED Matrix and the gyro are directly connected to a
       breadboard without wires.
       If you have a different setup you may need to modify this:
    */

    // read the pushbutton input pins:
    mazeButtonLeftState = digitalRead(PIN_MAZE_LEFT);
    mazeButtonRightState = digitalRead(PIN_MAZE_RIGHT);
    mazeButtonUpState = digitalRead(PIN_MAZE_UP);
    mazeButtonDownState = digitalRead(PIN_MAZE_DOWN);

    // compare the mazeButtonLeftState to its previous state
    if (mazeButtonLeftState != lastMazeButtonLeftState) {
      // if the state has changed, increment the counter
      if (mazeButtonLeftState == HIGH) {
        // if the current state is HIGH then the button went from off to on:
        if (DEBUG_LEVEL >= 1) {
          Serial.println("mazeLeftOn");
        }
        mazeTryToMove(Left);
      } else {
        // if the current state is LOW then the button went from on to off:
        if (DEBUG_LEVEL >= 1) {
          Serial.println("mazeLeftOff");
        }
      }
    }
    // save the current state as the last state, for next time through the loop
    lastMazeButtonLeftState = mazeButtonLeftState;

    // compare the mazeButtonRightState to its previous state
    if (mazeButtonRightState != lastMazeButtonRightState) {
      // if the state has changed, increment the counter
      if (mazeButtonRightState == HIGH) {
        // if the current state is HIGH then the button went from off to on:
        if (DEBUG_LEVEL >= 1) {
          Serial.println("mazeRightOn");
        }
        mazeTryToMove(Right);
      } else {
        // if the current state is LOW then the button went from on to off:
        if (DEBUG_LEVEL >= 1) {
          Serial.println("mazeRightOff");
        }
      }
    }
    // save the current state as the last state, for next time through the loop
    lastMazeButtonRightState = mazeButtonRightState;

    // compare the mazeButtonUpState to its previous state
    if (mazeButtonUpState != lastMazeButtonUpState) {
      // if the state has changed, increment the counter
      if (mazeButtonUpState == HIGH) {
        // if the current state is HIGH then the button went from off to on:
        if (DEBUG_LEVEL >= 1) {
          Serial.println("mazeUpOn");
        }
        mazeTryToMove(Up);
      } else {
        // if the current state is LOW then the button went from on to off:
        if (DEBUG_LEVEL >= 1) {
          Serial.println("mazeUpOff");
        }
      }
    }
    // save the current state as the last state, for next time through the loop
    lastMazeButtonUpState = mazeButtonUpState;

    // compare the mazeButtonDownState to its previous state
    if (mazeButtonDownState != lastMazeButtonDownState) {
      // if the state has changed, increment the counter
      if (mazeButtonDownState == HIGH) {
        // if the current state is HIGH then the button went from off to on:
        if (DEBUG_LEVEL >= 1) {
          Serial.println("mazeDownOn");
        }
        mazeTryToMove(Down);
      } else {
        // if the current state is LOW then the button went from on to off:
        if (DEBUG_LEVEL >= 1) {
          Serial.println("mazeDownOff");
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
  lc.clearDisplay(0);
  digitalWrite(PIN_MAZE_LED_GREEN, LOW);
}
