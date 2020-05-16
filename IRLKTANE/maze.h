//On the Subject of Mazes
/*
  KNOWN ISSUES:
  STARTING AND FINISHING POINTS ARE FIXED CURRENTLY
    - IDEALLY THESE WOULD NOT BE FIXED AND WOULD BE RANDOM EVENTUALLY
  CURRENTLY PROGRAMED FOR RED ONLY LED MATRIX. WAITING ON HARDWARE TO WRITE/ UPDATE FOR RGB MATRIX
/*
  SETUP FOR A MAZE
*/

#define PIN_MAZE_LED_FIN 44
#define PIN_MAZE_UP A5
#define PIN_MAZE_RIGHT A6
#define PIN_MAZE_DOWN A7
#define PIN_MAZE_LEFT A8
#define PIN_MAZE_DATA 92
#define PIN_MAZE_LOAD 91
#define PIN_MAZE_CLOCK 102
#define MAZE_MAX_MODULE_COUNT 1
#define MAZE_BLINK_DELAY 300

int mazeButtonLeftState = 0; // current state of the left button
int mazeButtonRightState = 0; // current state of the right button
int mazeButtonUpState = 0; // current state of the up button
int mazeButtonDownState = 0; // current state of the down button
int lastMazeButtonLeftState = 0; // previous state of the left button
int lastMazeButtonRightState = 0; // previous state of the right button
int lastMazeButtonUpState = 0; // previous state of the up button
int lastMazeButtonDownState = 0; // previous state of the down button

const int MAZE_LEDS_BRIGHTNESS = 8; // Intensity of the led, a number between 1-15

LedControl mazelc = LedControl(PIN_MAZE_DATA, PIN_MAZE_CLOCK, PIN_MAZE_LOAD, MAZE_MAX_MODULE_COUNT);

// these are for movement & play
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
        mazelc.setLed(0, x, y, true);
      }
      else if (MAZE_DISPLAY_LEVELS[mazeCurrentLevel][x][y] == 'f') {
        mazelc.setLed(0, x, y, true);
      }
    }
  }
}

void mazeInitLevel() 
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  mazelc.clearDisplay(0);

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
  mazelc.setLed(0, mazeDisplayCurrentX, mazeDisplayCurrentY, true);
}

void mazeHidePlayer() 
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  mazelc.setLed(0, mazeDisplayCurrentX, mazeDisplayCurrentY, false);
}

void mazeCheckWin() 
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  if (MAZE_LEVELS[mazeCurrentLevel][mazeCurrentX][mazeCurrentY] == 'f') {
    mazelc.clearDisplay(0);
    digitalWrite(PIN_MAZE_LED_FIN, HIGH);
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
  mazelc.shutdown(0, false);
  /* Set the brightness to a medium values */
  mazelc.setIntensity(0, MAZE_LEDS_BRIGHTNESS);
  /* and clear the display */
  mazelc.clearDisplay(0);
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
          Serial.println("mazeLeftOn");
        }
        mazeTryToMove(Left);
      } else {
        if (DEBUG_LEVEL >= 1) {
          Serial.println("mazeLeftOff");
        }
      }
    }
    // save the current state as the last state, for next time through the loop
    lastMazeButtonLeftState = mazeButtonLeftState;

    // compare the mazeButtonRightState to its previous state
    if (mazeButtonRightState != lastMazeButtonRightState) {
      if (mazeButtonRightState == HIGH) {
        if (DEBUG_LEVEL >= 1) {
          Serial.println("mazeRightOn");
        }
        mazeTryToMove(Right);
      } else {
        if (DEBUG_LEVEL >= 1) {
          Serial.println("mazeRightOff");
        }
      }
    }
    // save the current state as the last state, for next time through the loop
    lastMazeButtonRightState = mazeButtonRightState;

    // compare the mazeButtonUpState to its previous state
    if (mazeButtonUpState != lastMazeButtonUpState) {
      if (mazeButtonUpState == HIGH) {
        if (DEBUG_LEVEL >= 1) {
          Serial.println("mazeUpOn");
        }
        mazeTryToMove(Up);
      } else {
        if (DEBUG_LEVEL >= 1) {
          Serial.println("mazeUpOff");
        }
      }
    }
    // save the current state as the last state, for next time through the loop
    lastMazeButtonUpState = mazeButtonUpState;

    // compare the mazeButtonDownState to its previous state
    if (mazeButtonDownState != lastMazeButtonDownState) {
      if (mazeButtonDownState == HIGH) {
        if (DEBUG_LEVEL >= 1) {
          Serial.println("mazeDownOn");
        }
        mazeTryToMove(Down);
      } else {
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
  mazelc.clearDisplay(0);
  digitalWrite(PIN_MAZE_LED_FIN, LOW);
}
