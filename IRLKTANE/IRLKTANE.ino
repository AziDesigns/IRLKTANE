//THIS BOMB USES KTANE BOMB DEFUSAL MANUAL VERSION 1 VERIFICATION CODE #241 ONLY
/*
  KNOWN ISSUES:
  Module lights are staying on after bomb is defused. Only lights that should stay on are module complete green leds
*/
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <LedControl.h>
#include <SevenSegmentExtended.h>
#include "LedControl.h"

#define BOMB_TIMER_MINUTES 7 // total bomb starting time min
#define BOMB_TIMER_SECONDS 0 // total bomb starting time sec

/*  LEVEL 0 = MINIMAL SERIAL PRINT
    LEVEL 1 = PER USER ACTION LOGGING & BOMB VALUES
    LEVEL 2 = PER FUNCTION CALL LOGGING
    LEVEL 3 = SAME AS L3 WITH loop, setup, morseDisplay, displayTime, and mazeOne FUNCTIONS INCLUDED
    MORE LEVELS IN THE FUTURE?
*/
#define DEBUG_LEVEL 2

bool defused = false, exploded = false, victorySong = false; // is bomb defused or exploded default values false

// for testing purposes and potential future (ability to select modules) feature these flags will determine which flags below should be included/ excuded by setting the value to true/false.
bool
buttonModuleIncluded = false,
dischargeModuleIncluded = false,
knobModuleIncluded = false,
mazeModuleIncluded = false,
memoryModuleIncluded = false,
morseModuleIncluded = false,
passwordModuleIncluded = true,
simonModuleIncluded = false,
ventingModuleIncluded = false,
whoModuleIncluded = false;

// on new bomb all modules start with default "is difused" state of False, set to true if you dont want to have to do the modules for testing
bool
buttonModuleDefused = false,
knobModuleStike = false,
mazeModuleDefused = false,
memoryModuleDefused = false,
morseModuleDefused = false,
passwordModuleDefused = false,
simonModuleDefused = false,
whoModuleDefused= false;

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
  Serial.println (serialCode);
}

// list all modules/ header files that should be included
// ** buzzer.h and time.h must be at the top as they are called by other functions

#include "buzzer.h"
#include "time.h"
#include "morse.h" //higher in list so the max7219 chain will be loaded before other modules that need it
#include "indicator.h"
#include "batteries.h"
#include "button.h"
#include "discharge.h"
#include "knob.h"
#include "maze.h"
#include "memory.h"
#include "password.h"
#include "simon.h"
#include "venting.h"
#include "who.h"

void setup() // this section includes all setups for all modules to define INPUTS/OUTPUTS/DEFAULT VALUES/ ETC
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  Serial.begin(9600);
  randomSeed(analogRead(0));

  lcdButton.begin(16, 2);

  generateSerialCode();
  lcdButton.clear();
  lcdButton.setCursor(0, 1);
  lcdButton.print("Serial: ");
  lcdButton.print(serialCode);

  lc.shutdown(1,false); // moved from morse.h so strikes can function without morse active
  lc.setIntensity(1,10);
  lc.clearDisplay(1);
  
  timeSetup();
  buzzerSetup();
  
  if (buttonModuleIncluded==true) {
    buttonSetup();
  } else {
    buttonModuleDefused = true;  
  }
  if (dischargeModuleIncluded==true) {
    dischargeSetup();
  } else {  
  }
  if (knobModuleIncluded==true) {
    knobSetup();
  }
  if (mazeModuleIncluded==true) {
    mazeSetup();
  } else {
    mazeModuleDefused = true; 
  }
  if (memoryModuleIncluded==true) {
    memorySetup();
  } else {
    memoryModuleDefused = true; 
  }
  if (morseModuleIncluded==true) {
    morseSetup();
  } else {
    morseModuleDefused = true; 
  }
  if (passwordModuleIncluded==true) {
    passwordSetup();
  } else {
    passwordModuleDefused = true; 
  }
  if (simonModuleIncluded==true) {
    simonSetup();
  } else {
    simonModuleDefused = true;
  }
  if (ventingModuleIncluded==true) {
    ventingSetup();
  }
  if (whoModuleIncluded==true) {
    whoSetup();
  } else {
    whoModuleDefused= true;
    lcdWho.init();
    lcdWho.off();
  }
}

void bombExploded() // what should each module do when exploded
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  exploded = true;
  
  if (buttonModuleIncluded==true) {
    buttonModuleBoom();
  }
  if (dischargeModuleIncluded==true) {
    dischargeModuleBoom();
  }
  if (knobModuleIncluded==true) {
    knobModuleBoom();
  }
  if (mazeModuleIncluded==true) {
    mazeModuleBoom();
  }
  if (memoryModuleIncluded==true) {
    memoryModuleBoom();
  }
  if (morseModuleIncluded==true) {
    morseModuleBoom();
  }
  if (passwordModuleIncluded==true) {
    passwordModuleBoom();
  }
  if (simonModuleIncluded==true) {
    simonModuleBoom();
  }
  if (ventingModuleIncluded==true) {
    ventingModuleBoom();
  }
  if (whoModuleIncluded==true) {
    whoModuleBoom();
  }
  boomBuzzer();//needs to be last due to delays?
  timeModuleBoom();//needs to be last due to delays?
}

void addStrike() // function that adds a strike
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  nrStrikes++;

  if (nrStrikes == 1) lc.setDigit(1,0,9,true); // use to set strike #1 on
  else if (nrStrikes == 2) lc.setDigit(1,1,9,true); // use to set strike #2 on
  else if (nrStrikes == 3) { // the maximum number of strikes is reached
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
    
      if (buttonModuleIncluded==true) {
        buttonLoop();
      }
      if (dischargeModuleIncluded==true) {
        dischargeLoop();
      }
      if ((knobModuleIncluded==true) && (knobModuleStike==false)) {
        knobLoop();
      }
      if (mazeModuleIncluded==true) {
        mazeLoop();
      }
      if (memoryModuleIncluded==true) {
        memoryLoop();
      }
      if (morseModuleIncluded==true) {
        morseLoop();
      }
      if (passwordModuleIncluded==true) {
        passwordLoop();
      }
      if (simonModuleIncluded==true) {
        simonLoop();
      }
      if (ventingModuleIncluded==true) {
        ventingLoop();
      }
      if (whoModuleIncluded==true) {
        whoLoop();
      }
  }

  if (!exploded && (
        buttonModuleDefused &&
        mazeModuleDefused &&
        memoryModuleDefused &&
        morseModuleDefused &&
        passwordModuleDefused &&
        simonModuleDefused &&
        whoModuleDefused
      ))
  {
    if (victorySong==false) {
    victoryBuzzer();
    defused = true;
    }
  }
}
