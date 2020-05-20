// On the Subject of Discharge
/*
  KNOWN ISSUES:
  MODULE FULLY FUNCTIONS FROM A BUTTON, LED, AND INCREASING TIME STANDPOINT,
    HOWEVER THE LEVER IS JUST A BUTTON, NO LEVER INSTALLED AND THAT LOGIC HAS NOT BEEN VERIFIED.
  PINS FOR LED AND BUTTON/LEVER WILL CONFLICT ONCE MERGED WITH MAIN BRANCH. USING 2ND MEGA2560 FOR TESTING THIS MODULE
  NEEDS ITS OWN TIMER / DISPLAY CURRENTLY USING SAME DISPLAY AS OVERALL TIME TO REDUCE EFFORT IN WRITING CODE
*/
#define PIN_DISCHARGE_LED_1 2,0,7 // turns on when button pressed
#define PIN_DISCHARGE_BUTTON_1 29 // discharge button
#define DISCHARGE_DEFAULT_TIME 45 // default time per rotation
    
unsigned long dischargeSeconds = 0;
byte dischargeSec = DISCHARGE_DEFAULT_TIME + 1;

byte dischargeButtonState = 0; // current state of the button
byte lastDischargeButtonState = 0; // previous state of the button
byte addOrSubtractTime = 0; //0=subtract,1=add

void dischargeSetup() 
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  pinMode(PIN_DISCHARGE_BUTTON_1, INPUT);
};

void dischargeDisplayTime() // function that displays the time on the clock
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  if (addOrSubtractTime==0) {
    if (dischargeSeconds < millis()) {
      dischargeSeconds += 1000;
      dischargeSec--;
    }
  } else if ((addOrSubtractTime==1) && (dischargeSec<DISCHARGE_DEFAULT_TIME)) {
    if (dischargeSeconds < millis()) {
      dischargeSeconds += 200;
      dischargeSec++;
    }
  } else if ((addOrSubtractTime==1) && (dischargeSec>=DISCHARGE_DEFAULT_TIME)) {
    if (dischargeSeconds < millis()) {
      dischargeSeconds += 1000;
      dischargeSec=DISCHARGE_DEFAULT_TIME;
    }
  }
  if (dischargeSec == -1) {
    addStrike(); // if the time hits zero the bomb will go off
  }
  needyDigitDisplay(1, 2, 3, dischargeSec);
}

void dischargeLoop()
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }

  if (!defused && !exploded) {
    dischargeDisplayTime();

    dischargeButtonState = digitalRead(PIN_DISCHARGE_BUTTON_1);
    // compare the dischargeButtonState to its previous state
    if (dischargeButtonState != lastDischargeButtonState) {
      if (dischargeButtonState == HIGH) {
        if (DEBUG_LEVEL >= 1) {
          Serial.println(F("dischargeButtonOn"));
        }
        addOrSubtractTime=1; //this is what adds the time back
        lc.setLed(PIN_DISCHARGE_LED_1,true);
      } else {
        if (DEBUG_LEVEL >= 1) {
          Serial.println(F("dischargeButtonOff"));
        }
        addOrSubtractTime=0; //reduces time when not pressed
        lc.setLed(PIN_DISCHARGE_LED_1,false);
      }
    }
    // save the current state as the last state, for next time through the loop
    lastDischargeButtonState = dischargeButtonState;
  }
}

void dischargeBombDefused()
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  lc.setDigit(1,3,' ',false);
  lc.setDigit(1,2,' ',false); 
  lc.setLed(PIN_DISCHARGE_LED_1,false);
}

void dischargeModuleBoom()
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  // when the bomb explodes the LED and so should countdown timer above module
  if (explodedFromStrikes) {
    //NEED TO PRINT TO LC.... TO SAY DISPLAY TIME REMAINING //RIGHT NOW IT SHOWS '  '
    lc.setDigit(1,3,' ',false);
    lc.setDigit(1,2,' ',false); 
  }
  else {
    //NEED TO PRINT TO LC.... TO SAY DISPLAY NOTHING
    lc.setDigit(1,3,' ',false);
    lc.setDigit(1,2,' ',false); 
  }
  lc.setLed(PIN_DISCHARGE_LED_1,false);
}
