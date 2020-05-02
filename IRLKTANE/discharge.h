// On the Subject of Discharge
/*
  KNOWN ISSUES:
  MODULE FULLY FUNCTIONS FROM A BUTTON, LED, AND INCREASING TIME STANDPOINT,
    HOWEVER THE LEVER IS JUST A BUTTON, NO LEVER INSTALLED AND THAT LOGIC HAS NOT BEEN VERIFIED.
  PINS FOR LED AND BUTTON/LEVER WILL CONFLICT ONCE MERGED WITH MAIN BRANCH. USING 2ND MEGA2560 FOR TESTING THIS MODULE
  NEEDS ITS OWN TIMER / DISPLAY CURRENTLY USING SAME DISPLAY AS OVERALL TIME TO REDUCE EFFORT IN WRITING CODE
*/
/*
   This module consists of a knob that can be turned in four directions (up,down,left,right)
   as well as twelve LEDs that may or may not light up upon the module activation.
   When this needy module activates, some of the LEDs on the bottom will light up,
   and the knob will rotate. The Defuser must communicate the formation of the lit LEDs
   to the Expert who will then tell them which position to set the knob to.
*/

#define DISCHARGE_PIN_CLK 14 // discharge countdown clock CLK
#define DISCHARGE_PIN_DIO 15 // discharge countdown clock DIO
#define DISCHARGE_DEFAULT_TIME 45 // default time per rotation
#define PIN_DISCHARGE_LED_1  32 // turns on when button pressed
#define PIN_DISCHARGE_BUTTON_1 30 // discharge button

#define DISCHARGE_BUTTON_PRESS_DELAY 50 // time required to prevent button bounce

SevenSegmentExtended dischargeTimer(PIN_CLK, PIN_DIO);
unsigned long dischargeSeconds = 0;
int dischargeSec = DISCHARGE_DEFAULT_TIME + 1;

int dischargeButtonState = 0; // current state of the button
int lastDischargeButtonState = 0; // previous state of the button
int addOrSubtractTime = 0; //0=subtract,1=add

void dischargeSetup() 
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  dischargeTimer.begin();            // initializes the display
  dischargeTimer.setBacklight(100);  // set the brightness to 100%
  pinMode(PIN_DISCHARGE_LED_1, OUTPUT);
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
    bombExploded(); // if the time hits zero the bomb will go off
  }
  dischargeTimer.printTime(dischargeSec, false);
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
          Serial.println("dischargeButtonOn");
        }
        addOrSubtractTime=1; //this is what adds the time back
        digitalWrite (PIN_DISCHARGE_LED_1, HIGH);
      } else {
        if (DEBUG_LEVEL >= 1) {
          Serial.println("dischargeButtonOff");
        }
        addOrSubtractTime=0; //reduces time when not pressed
        digitalWrite (PIN_DISCHARGE_LED_1, LOW);
      }
    }
    // save the current state as the last state, for next time through the loop
    lastDischargeButtonState = dischargeButtonState;
  }
}

void dischargeModuleBoom()
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  // when the bomb explodes the LED and so should countdown timer above module
  if (explodedFromStrikes) dischargeTimer.printTime(dischargeSec, false);
  else dischargeTimer.print("  ");
  digitalWrite (PIN_DISCHARGE_LED_1, LOW);
}
