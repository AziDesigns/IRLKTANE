//THIS BOMB USES KTANE BOMB DEFUSAL MANUAL VERSION 1 VERIFICATION CODE #241 ONLY
/*
  KNOWN ISSUES:
  ADD STRIKE NEEDS TO NOT PLAY STRIKE SOUND UPON EXPLODE.
*/
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <LedControl.h>
#include <SevenSegmentExtended.h>

#define BOMB_TIMER_MINUTES 7 // total bomb starting time min
#define BOMB_TIMER_SECONDS 0 // total bomb starting time sec

#define PIN_STRIKE_LED_1 30 // strike 1 LED
#define PIN_STRIKE_LED_2 31 // strike 2 LED
#define PIN_STRIKE_LED_3 32  // strike 3 LED

/*  LEVEL 0 = MINIMAL SERIAL PRINT
    LEVEL 1 = PER USER ACTION LOGGING & BOMB VALUES
    LEVEL 2 = PER FUNCTION CALL LOGGING
    LEVEL 3 = SAME AS L3 WITH loop, setup, morseDisplay, displayTime, and mazeOne FUNCTIONS INCLUDED
    MORE LEVELS IN THE FUTURE?
*/
#define DEBUG_LEVEL 2

bool defused = false, exploded = false, victorySong = false; // is bomb defused or exploded default values false

// on new bomb all modules start with default "is difused" state of False, set to true if you dont want to have to do the modules for testing
bool
buttonModuleDefused = false,
//mazeModuleDefused = false,
//memoryModuleDefused = false,
morseModuleDefused = false,
//passwordModuleDefused = false,
simonModuleDefused = false; //,
//whoModuleDefused= false;

bool explodedFromStrikes = false; // is bomb exploded from strikes default value false
int nrStrikes = 0; // number of strikes default starting is 0 (could be changed in future to allow only 1 strike, etc)

char serialCode[10];

void generateSerialCode() // function that generates the serial number for the bomb
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  char alphanumeric[50] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
  for ( int i = 0; i < 7; i++)
    serialCode[i] = alphanumeric[random(0, 35)];
}

// list all modules/ header files that should be included
// ** buzzer.h and time.h must be at the top as they are called by other functions

#include "buzzer.h"
#include "time.h"
#include "indicator.h"
#include "batteries.h"
#include "button.h"
//#include "discharge.h"
//#include "knob.h"
//#include "maze.h"
//#include "memory.h"
#include "morse.h"
//#include "password.h"
#include "simon.h"
//#include "venting.h"
//#include "who.h"

void setup() // this section includes all setups for all modules to define INPUTS/OUTPUTS/DEFAULT VALUES/ ETC
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  Serial.begin(9600);
  randomSeed(analogRead(0));

  pinMode(PIN_STRIKE_LED_1, OUTPUT);
  pinMode(PIN_STRIKE_LED_2, OUTPUT);
  pinMode(PIN_STRIKE_LED_3, OUTPUT);

  digitalWrite(PIN_STRIKE_LED_1, LOW);
  digitalWrite(PIN_STRIKE_LED_2, LOW);
  digitalWrite(PIN_STRIKE_LED_3, LOW);

  generateSerialCode();

  buttonSetup();
  //dischargeSetup();
  //knobSetup();
  //mazeSetup();
  //memorySetup();
  morseSetup();
  //passwordSetup();
  simonSetup();
  timeSetup();
  //ventingSetup();
  //whoSetup();
}

void bombExploded() // what should each module do when exploded
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  exploded = true;
  buttonModuleBoom();
  //dischargeModuleBoom();
  //knobModuleBoom();
  //mazeModuleBoom();
  //memoryModuleBoom();
  morseModuleBoom();
  //passwordModuleBoom();
  simonModuleBoom();
  //ventingModuleBoom();
  //whoModuleBoom();
  boomBuzzer();//needs to be last due to delays?
  timeModuleBoom();//needs to be last due to delays?
}

void addStrike() // function that adds a strike
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  nrStrikes++;

  if (nrStrikes == 1) digitalWrite(PIN_STRIKE_LED_1, HIGH);
  else if (nrStrikes == 2) digitalWrite(PIN_STRIKE_LED_2, HIGH);
  else if (nrStrikes == 3) { // the maximum number of strikes is reached
    digitalWrite(PIN_STRIKE_LED_3, HIGH);
    explodedFromStrikes = true;
    bombExploded();
  }
  strikeBuzzer();
}

void loop()
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }

  timeLoop();

  if (!defused && !exploded) {
    buttonLoop();
    //dischargeLoop();
    //knobLoop();
    //mazeLoop();
    //memoryLoop();
    morseLoop();
    //passwordLoop();
    simonLoop();
    //ventingLoop();
    //whoLoop();
  }

  if (!exploded && (
        buttonModuleDefused &&
        //mazeModuleDefused &&
        //memoryModuleDefused &&
        morseModuleDefused &&
        //passwordModuleDefused &&
        simonModuleDefused //&&
        //whoModuleDefused
      ))
  {
    if (victorySong==false) {
    victoryBuzzer();
    defused = true;
    victorySong=true;
    }
  }
}
