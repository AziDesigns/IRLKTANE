//On the Subject of Mazes
/*
  IDEA BOARD: USE A SINGLE ARRAY OF A 18X18 GRID INSTEAD OF 9 SEPARATE 6X6 GRIDS TO TRACK VALID POSITIONS
  THEN YOU CAN SIMPLY MARK BETWEEN BOARDS AS INVALID SELECTIONS TO ADD A STRIKE
  USE THE PLAYER POSITION TO DETERMINE WHICH GREEN LED'S SHOULD BE LIT UP
  USE THE PLAYER POSITION AND GREEN LED'S TO DETERMINE AN AVAILIBLE SPACE FOR THE RED LED THAT IS NOT OCCUPIED IN THE SAME 6X6

  KNOWN ISSUES:
  MODULE IS WIP AND THERE ARE MANY MANY ISSUES & MISSING LOGIC.
  MAIN ISSUE IS WITH THE ACTUAL DISPLAY OUTPUT.
  - CURRENTLY THE DISPLAY SHOWS WHERE YOU ARE ACTUALLY AT IN THE MAZE WITH THE WALLS
  - WALLS ARE HIDDEN AS THE DISPLAY ONLY IS SET TO SHOW HIGH FOR CURRENT POSITION, FINAL POSITION, AND CIRCLES
  - BELIVED FIX IS TO HAVE A 2ND VERSION OF EACH MAZE ARRAY THAT ONLY STORES MY POSITION, GOAL, CIRCLES, AND NO WALLS.
    - THEN WILL NEED TO UPDATE ADDITIONAL VARIABLES UPON BUTTON PRESSES TO CONTROL WHAT IS DISPLAYED.
  SECOND ISSUE IS I AM OUT OF BUTTONS SO HAD TO REUSE BUTTONS FROM THE MORSE AND BUTTON MODULE. WHEN NEW BUTTONS ARRIVE I WILL FIX AND POINT TO CORRECT PINS
*/
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
#define PIN_MAZE_UP 25
#define PIN_MAZE_RIGHT 26
#define PIN_MAZE_DOWN 23
#define PIN_MAZE_LEFT 24

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
const int MAZE_LEVELS_ROWS = 13; // number of rows of each level
const int MAZE_LEVELS_COLUMNS = MAZE_LEVELS_ROWS; // number of columns of each level

/**
   'c': green circle position
   's': Start position
   'f': Finish position
   'X': Wall
   ' ': Movement zone
*/
const char MAZE_LEVELS[][MAZE_LEVELS_ROWS][MAZE_LEVELS_COLUMNS] = {
  // Level 1
  {
    {'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X'},
    {'X', ' ', ' ', ' ', ' ', ' ', 'X', ' ', ' ', ' ', ' ', ' ', 'X'},
    {'X', ' ', 'X', 'X', 'X', ' ', 'X', ' ', 'X', 'X', 'X', 'X', 'X'},
    {'X', 'c', 'X', 's', ' ', ' ', 'X', 'f', ' ', ' ', ' ', ' ', 'X'},
    {'X', ' ', 'X', ' ', 'X', 'X', 'X', 'X', 'X', 'X', 'X', ' ', 'X'},
    {'X', ' ', 'X', ' ', ' ', ' ', 'X', ' ', ' ', ' ', ' ', 'c', 'X'},
    {'X', ' ', 'X', 'X', 'X', ' ', 'X', ' ', 'X', 'X', 'X', ' ', 'X'},
    {'X', ' ', 'X', ' ', ' ', ' ', ' ', ' ', 'X', ' ', ' ', ' ', 'X'},
    {'X', ' ', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', ' ', 'X'},
    {'X', ' ', ' ', ' ', ' ', ' ', 'X', ' ', ' ', ' ', 'X', ' ', 'X'},
    {'X', ' ', 'X', 'X', 'X', ' ', 'X', ' ', 'X', 'X', 'X', ' ', 'X'},
    {'X', ' ', ' ', ' ', 'X', ' ', ' ', ' ', 'X', ' ', ' ', ' ', 'X'},
    {'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X'},
  }
  // testing with just 1 level for morse
  /*,
    // Level 2
    {
    {'X','X','X','X','X','X','X','X','X','X','X','X','X'},
    {'X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X','X','X','X','X','X','X','X','X','X','X','X','X'},
    },
    // Level 3
    {
    {'X','X','X','X','X','X','X','X','X','X','X','X','X'},
    {'X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X','X','X','X','X','X','X','X','X','X','X','X','X'},
    }
  */
  /*
    // Level Template
    {
    {'X','X','X','X','X','X','X','X','X','X','X','X','X'},
    {'X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','X'},
    {'X','X','X','X','X','X','X','X','X','X','X','X','X'},
    }
  */
};

/**
   GAME STATE
*/
int mazeCurrentLevel = 0; // Current level paying // will become random int between 0-8
int mazeCurrentX; // Current player X position
int mazeCurrentY; // Current player Y position
int mazeStartX; // Current level start X position
int mazeStartY; // Current level start Y position
int mazeFinishX; // Current level finish X position
int mazeFinishY; // Current level finish Y position
enum mazeMove { Left, Right, Up, Down }; // Possible directions to move the player

void mazePrintLevel() 
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  for (int x = 0; x < MAZE_LEVELS_ROWS; x++) {
    for (int y = 0; y < MAZE_LEVELS_COLUMNS; y++) {
      if (MAZE_LEVELS[mazeCurrentLevel][x][y] == 'c') {
        lc.setLed(0, x, y, true);
      }
      else if (MAZE_LEVELS[mazeCurrentLevel][x][y] == 'f') {
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

  // sets startX, startY, finishX and finishY
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

  mazeCurrentX = mazeStartX;
  mazeCurrentY = mazeStartY;

  mazePrintLevel();
}

void mazeShowPlayer() 
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  lc.setLed(0, mazeCurrentX, mazeCurrentY, true);
}

void mazeHidePlayer() 
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  lc.setLed(0, mazeCurrentX, mazeCurrentY, false);
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
    //mazeHidePlayer(); // run every time a player moves instead
    //mazeShowPlayer(); // run this every time the player moves a direction?

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

    //mazeCheckWin(); //moved to check after each move is made.
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
