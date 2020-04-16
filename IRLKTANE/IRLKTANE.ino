#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <LedControl.h>
#include <SevenSegmentExtended.h>

#define BOMB_TIMER_MINUTES 7 // total bomb starting time min
#define BOMB_TIMER_SECONDS 0 // total bomb starting time sec

#define PIN_STRIKE_LED_1 50 // strike 1 LED
#define PIN_STRIKE_LED_2 48 // strike 2 LED
#define PIN_STRIKE_LED_3 46  // strike 3 LED

bool defused = false, exploded = false; // is bomb defused or exploded default values false

// on new bomb all modules start with default "is difused" state of False
bool 
buttonModuleDefused = false, 
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
  char alphanumeric[50] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
  for ( int i = 0; i < 7; i++)
    serialCode[i] = alphanumeric[random(0, 35)];
}

// list all modules that should be included

#include "button.h"
#include "buzzer.h"
#include "discharge.h"
//#include "knob.h"
//#include "maze.h"
#include "memory.h"
//#include "morse.h"
//#include "password.h"
//#include "simon.h"
#include "time.h"
//#include "Venting.h"
#include "who.h"

void setup() // this section includes all setups for all modules to define INPUTS/OUTPUTS/DEFAULT VALUES/ ETC
{
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
  dischargeSetup();
//  knobSetup();
//  mazeSetup();
  memorySetup();
//  morseSetup();
//  passwordSetup();
//  defusedSimonSetup();
  timeSetup();
//  ventingSetup();
  whoSetup();

}

void bombExploded() // what should each module do when exploded
{
  exploded = true;
  boomBuzzer();
  buttonModuleBoom();
  dischargeModuleBoom();
//  knobModuleBoom();
//  mazeModuleBoom();
  memoryModuleBoom();
//  morseModuleBoom();
//  passwordModuleBoom();
//  simonModuleBoom();
  timeModuleBoom();
//  ventingModuleBoom();
  whoModuleBoom();
}

void addStrike() // function that adds a strike
{
  nrStrikes++;
  strikeBuzzer();

  if (nrStrikes == 1) digitalWrite(PIN_STRIKE_LED_1, HIGH);
  else if (nrStrikes == 2) digitalWrite(PIN_STRIKE_LED_2, HIGH);
  else if (nrStrikes == 3) // the maximum number of strikes is reached
  {
    digitalWrite(PIN_STRIKE_LED_3, HIGH);
    explodedFromStrikes = true;
    bombExploded();
  }
}

void loop()
{

  timeLoop();
//    buzzerLoop(); //dont need a loop as it only goes off if true?

  if (!defused && !exploded)
  {
    buttonLoop();
    dischargeLoop();
//    knobLoop();
//    mazeLoop();
    memoryLoop();
//    morseLoop();
//    passwordLoop();
//    simonLoop();
//    VentingLoop();
    whoLoop();
  }

  if (!exploded && (
   buttonModuleDefused && 
//   mazeModuleDefused &&
   memoryModuleDefused && 
//   morseModuleDefused &&
//   passwordModuleDefused &&
//   simonModuleDefused && 
   whoModuleDefused))
  {
    victoryBuzzer();
    defused = true;
  }
}
