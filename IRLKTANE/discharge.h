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

#define PIN_DISCHARGE_LED_1  31 // turns on when button pressed
#define PIN_DISCHARGE_BUTTON_1 30 // discharge button
#define DISCHARGE_BUTTON_PRESS_DELAY 50 // time required to prevent button bounce
#define DISCHARGE_DEFAULT_TIME 45 // default time per rotation
    
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
  pinMode(PIN_DISCHARGE_LED_1, OUTPUT);
  pinMode(PIN_DISCHARGE_BUTTON_1, INPUT);
};

// THIS IS REQUIRED DUE TO THE DISPLAY I AM USING BEING PART OF A DAISY CHAIN WITH A ALPHANUMERIC 14 DIGIT DISPLAY AND USING A MAX 7219 THAT CANT HANDLE THAT MANY SEGMENTS. IT USES THE DP AS A PART OF D
void dischargeDigitDisplay() 
{
    if (dischargeSec==45) {
      lc.setDigit(1,3,5,true); 
      lc.setDigit(1,2,4,true); 
    } else if (dischargeSec==44) {
      lc.setDigit(1,3,4,true); 
      lc.setDigit(1,2,4,true); 
    } else if (dischargeSec==43) {
      lc.setDigit(1,3,3,true); 
      lc.setDigit(1,2,4,true); 
    } else if (dischargeSec==42) {
      lc.setDigit(1,3,2,true); 
      lc.setDigit(1,2,4,true); 
    } else if (dischargeSec==41) {
      lc.setDigit(1,3,1,false); 
      lc.setDigit(1,2,4,true); 
    } else if (dischargeSec==40) {
      lc.setDigit(1,3,0,false); 
      lc.setDigit(1,2,4,true); 
    } else if (dischargeSec==39) {
      lc.setDigit(1,3,9,true); 
      lc.setDigit(1,2,3,true); 
    } else if (dischargeSec==38) {
      lc.setDigit(1,3,8,true); 
      lc.setDigit(1,2,3,true); 
    } else if (dischargeSec==37) {
      lc.setDigit(1,3,7,false); 
      lc.setDigit(1,2,3,true); 
    } else if (dischargeSec==36) {
      lc.setDigit(1,3,6,true); 
      lc.setDigit(1,2,3,true); 
    } else if (dischargeSec==35) {
      lc.setDigit(1,3,5,true); 
      lc.setDigit(1,2,3,true); 
    } else if (dischargeSec==34) {
      lc.setDigit(1,3,4,true); 
      lc.setDigit(1,2,3,true); 
    } else if (dischargeSec==33) {
      lc.setDigit(1,3,3,true); 
      lc.setDigit(1,2,3,true); 
    } else if (dischargeSec==32) {
      lc.setDigit(1,3,2,true); 
      lc.setDigit(1,2,3,true); 
    } else if (dischargeSec==31) {
      lc.setDigit(1,3,1,false); 
      lc.setDigit(1,2,3,true); 
    } else if (dischargeSec==30) {
      lc.setDigit(1,3,0,false); 
      lc.setDigit(1,2,3,true); 
    } else if (dischargeSec==29) {
      lc.setDigit(1,3,9,true); 
      lc.setDigit(1,2,2,true); 
    } else if (dischargeSec==28) {
      lc.setDigit(1,3,8,true); 
      lc.setDigit(1,2,2,true); 
    } else if (dischargeSec==27) {
      lc.setDigit(1,3,7,false); 
      lc.setDigit(1,2,2,true); 
    } else if (dischargeSec==26) {
      lc.setDigit(1,3,6,true); 
      lc.setDigit(1,2,2,true); 
    } else if (dischargeSec==25) {
      lc.setDigit(1,3,5,true); 
      lc.setDigit(1,2,2,true); 
    } else if (dischargeSec==24) {
      lc.setDigit(1,3,4,true); 
      lc.setDigit(1,2,2,true); 
    } else if (dischargeSec==23) {
      lc.setDigit(1,3,3,true); 
      lc.setDigit(1,2,2,true); 
    } else if (dischargeSec==22) {
      lc.setDigit(1,3,2,true); 
      lc.setDigit(1,2,2,true); 
    } else if (dischargeSec==21) {
      lc.setDigit(1,3,1,false); 
      lc.setDigit(1,2,2,true); 
    } else if (dischargeSec==20) {
      lc.setDigit(1,3,0,false); 
      lc.setDigit(1,2,2,true); 
    } else if (dischargeSec==19) {
      lc.setDigit(1,3,9,true); 
      lc.setDigit(1,2,1,false); 
    } else if (dischargeSec==18) {
      lc.setDigit(1,3,8,true); 
      lc.setDigit(1,2,1,false); 
    } else if (dischargeSec==17) {
      lc.setDigit(1,3,7,false); 
      lc.setDigit(1,2,1,false); 
    } else if (dischargeSec==16) {
      lc.setDigit(1,3,6,true); 
      lc.setDigit(1,2,1,false); 
    } else if (dischargeSec==15) {
      lc.setDigit(1,3,5,true); 
      lc.setDigit(1,2,1,false); 
    } else if (dischargeSec==14) {
      lc.setDigit(1,3,4,true); 
      lc.setDigit(1,2,1,false); 
    } else if (dischargeSec==13) {
      lc.setDigit(1,3,3,true); 
      lc.setDigit(1,2,1,false); 
    } else if (dischargeSec==12) {
      lc.setDigit(1,3,2,true); 
      lc.setDigit(1,2,1,false); 
    } else if (dischargeSec==11) {
      lc.setDigit(1,3,1,false); 
      lc.setDigit(1,2,1,false); 
    } else if (dischargeSec==10) {
      lc.setDigit(1,3,0,false); 
      lc.setDigit(1,2,1,false); 
    } else if (dischargeSec==9) {
      lc.setDigit(1,3,9,true); 
      lc.setDigit(1,2,0,false); 
    } else if (dischargeSec==8) {
      lc.setDigit(1,3,8,true); 
      lc.setDigit(1,2,0,false); 
    } else if (dischargeSec==7) {
      lc.setDigit(1,3,7,false); 
      lc.setDigit(1,2,0,false); 
    } else if (dischargeSec==6) {
      lc.setDigit(1,3,6,true); 
      lc.setDigit(1,2,0,false); 
    } else if (dischargeSec==5) {
      lc.setDigit(1,3,5,true); 
      lc.setDigit(1,2,0,false); 
    } else if (dischargeSec==4) {
      lc.setDigit(1,3,4,true); 
      lc.setDigit(1,2,0,false); 
    } else if (dischargeSec==3) {
      lc.setDigit(1,3,3,true); 
      lc.setDigit(1,2,0,false); 
    } else if (dischargeSec==2) {
      lc.setDigit(1,3,2,true); 
      lc.setDigit(1,2,0,false); 
    } else if (dischargeSec==1) {
      lc.setDigit(1,3,1,false); 
      lc.setDigit(1,2,0,false); 
    } else if (dischargeSec==0) {
      lc.setDigit(1,3,0,false);
      lc.setDigit(1,2,0,false);
    }
}

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
  dischargeDigitDisplay();
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
  digitalWrite (PIN_DISCHARGE_LED_1, LOW);
}
