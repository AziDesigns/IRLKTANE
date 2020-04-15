// The Timer

#define PIN_CLK 14 // countdown clock CLK
#define PIN_DIO 15 // countdown clock DIO

SevenSegmentExtended timer(PIN_CLK, PIN_DIO);
unsigned long seconds = 0;
int mins = BOMB_TIMER_MINUTES, sec = BOMB_TIMER_SECONDS + 1;

void timeSetup() {
  timer.begin();            // initializes the display
  timer.setBacklight(100);  // set the brightness to 100%
};

void displayTime() // function that displays the time on the clock
{
  if (seconds < millis())
  {
    seconds += 1000;
    sec--;
    if (sec == -1)
    {
      mins--;
      if (mins == -1)  bombExploded(); // if the time hits zero, the bomb will go off
      else sec = 59;
    }

    if (mins == -1) timer.printTime(0, 0, true);
    else timer.printTime(mins, sec, true);
  }
}

void timeLoop() {

  // if defused then show time left
  // if exploded from time show 00:00
  // if exploded from strikes show time remaining before explosion

  if (exploded)
    bombExploded();
  else if (!defused && !exploded)
    displayTime();

}

void timeModuleBoom() // if the bomb explodes what should the module display
{
  if (explodedFromStrikes) timer.print("    BOMB EXPLODED    ");
  else timer.printTime(mins, sec, true);
}
