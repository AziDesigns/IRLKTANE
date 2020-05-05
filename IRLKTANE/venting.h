// On the Subject of Venting Gas
/*
  KNOWN ISSUES:
  MODULE IS WIP AND THERE ARE MANY MANY ISSUES & MISSING LOGIC.
*/
#define PIN_VENTING_BUTTON_1 50 // YES BUTTON // PINOVERLAP
#define PIN_VENTING_BUTTON_2 51 // NO BUTTON // PINOVERLAP
#define PIN_VENTING_LED_GREEN 52 // MODULE COMPLETE GREEN LED

#define LCD_VENTING_CONTRAST 40 // LCD CONTRAST
#define VENTING_TIMER_SECONDS 40 // STARTING TIME WHEN MODULE RESETS

unsigned long currentMillis = 0; //TIME UNTIL NEXT VENT NEEDED

void ventingSetup()
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }

}

void ventingLoop()
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  /*
     Module asks one of these two prompts:
    // Vent Gas?
    //   Y/N

    // Detonate?
    //   Y/N

     It then slowly spells out your answer if it was correct.
     If correct it clears the display and timer
     If correct and question was vent y/n it says (Venting Complete) until it resets.
     If correct and question was detonate y/n it leaves the display off until it resets.
     If your answer was wrong to the prompt vent y/n it will (add a strike).
     If your answer was wrong to the prompt detonate y/n it will (detonate the bomb).
     If you run out of time it will (add a strike).

  */
}

void ventingModuleBoom()
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  // when the bomb explodes the venting module should clear the timer and the display
}
