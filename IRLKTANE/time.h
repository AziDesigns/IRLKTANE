// The Overall Bomb Timer/ Clock
/*
  KNOWN ISSUES:
  THERE IS NO LOGIC BUILT TO HANDLE CLOCK MOVING FASTER AFTER STRIKES AND NEEDS TO BE ADDED EVENTUALLY
  NEED LOGIC FOR IF DEFUSED THEN SHOW TIME LEFT AND STOP DECREASING TIME
  NEED LOGIC IN BOOM FOR IF EXPLODED FROM TIMEOUT THEN SHOW 00:00
  NEED LOGIC IN BOOM FOR IF EXPLODED FROM STRIKES THEN SHOW TIME THAT WAS REMAINING
*/

#define PIN_CLK 14 // countdown clock CLK
#define PIN_DIO 15 // countdown clock DIO

SevenSegmentExtended timer(PIN_CLK, PIN_DIO);
unsigned long seconds = 0;
int mins = BOMB_TIMER_MINUTES, sec = BOMB_TIMER_SECONDS + 1;

void timeSetup() 
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  timer.begin();            // initializes the display
  timer.setBacklight(100);  // set the brightness to 100%
};

void displayTime() // function that displays the time on the clock
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  if (seconds < millis()) {
    seconds += 1000;
    sec--;
    //countdownBuzzer(); //NEEDS TURNED BACK ON FOR GAMEPLAY. TURRNED OFF FOR TESTING
    if (sec == -1) {
      mins--;
      if (mins == -1)  bombExploded(); // if the time hits zero, the bomb will go off
      else sec = 59;
    }

    if (mins == -1) timer.printTime(0, 0, true);
    else timer.printTime(mins, sec, true);
  }
}

void timeLoop() 
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }

  if (!defused && !exploded)
    displayTime();

  // need logic for if defused then show time left and stop decreasing

}

void timeModuleBoom() // if the bomb explodes what should the module display
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  // if exploded from time show 00:00
  // if exploded from strikes show time remaining before explosion
  if (explodedFromStrikes) timer.print("    BOMB EXPLODED    ");
  else timer.printTime(mins, sec, true);
}
