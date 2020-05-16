// On the Subject of Venting Gas
/*
  KNOWN ISSUES:
  MODULE IS WIP AND THERE ARE MANY MANY ISSUES & MISSING LOGIC.
*/
#define PIN_VENTING_BUTTON_1 6 // YES BUTTON // PINOVERLAP
#define PIN_VENTING_BUTTON_2 7 // NO BUTTON // PINOVERLAP
#define PIN_VENTING_LED_GREEN 101 // MODULE COMPLETE GREEN LED

#define VENTING_DEFAULT_TIME 40 // STARTING TIME WHEN MODULE RESETS

unsigned long ventSeconds;
int ventSec;

unsigned long startVentMillisMax = 60000;
unsigned long startVentMillis = 0; //TIME UNTIL NEXT VENT NEEDED
unsigned long ventCurrentMillis = 0; //TIME UNTIL NEXT VENT NEEDED
unsigned long ventSwitchMillis = 0; // DEBOUNCE PREVENTION

unsigned long ventingPreventsExplosionsTime; // if user presses no on vent gas track time since press
bool ventingPreventsExplosionsTF = false;

int ventYesState = 0; // current state of the yes button
int ventNoState = 0; // current state of the no button
int lastVentYesState = 0; // previous state of the yes button
int lastVentNoState = 0; // previous state of the no button

int questionSelect; // 0=vent gas ?, 1=detonate ?

bool ventStartedYN = false;
bool isTimePassed = false;

LiquidCrystal_I2C lcdVent(0x26, 16, 2);

void ventDigitDisplay() 
{
  if (ventSec==45) {
    lc.setDigit(1,7,5,true); 
    lc.setDigit(1,6,4,true); 
  } else if (ventSec==44) {
    lc.setDigit(1,7,4,true); 
    lc.setDigit(1,6,4,true); 
  } else if (ventSec==43) {
    lc.setDigit(1,7,3,true); 
    lc.setDigit(1,6,4,true); 
  } else if (ventSec==42) {
    lc.setDigit(1,7,2,true); 
    lc.setDigit(1,6,4,true); 
  } else if (ventSec==41) {
    lc.setDigit(1,7,1,false); 
    lc.setDigit(1,6,4,true); 
  } else if (ventSec==40) {
    lc.setDigit(1,7,0,false); 
    lc.setDigit(1,6,4,true); 
  } else if (ventSec==39) {
    lc.setDigit(1,7,9,true); 
    lc.setDigit(1,6,3,true); 
  } else if (ventSec==38) {
    lc.setDigit(1,7,8,true); 
    lc.setDigit(1,6,3,true); 
  } else if (ventSec==37) {
    lc.setDigit(1,7,7,false); 
    lc.setDigit(1,6,3,true); 
  } else if (ventSec==36) {
    lc.setDigit(1,7,6,true); 
    lc.setDigit(1,6,3,true); 
  } else if (ventSec==35) {
    lc.setDigit(1,7,5,true); 
    lc.setDigit(1,6,3,true); 
  } else if (ventSec==34) {
    lc.setDigit(1,7,4,true); 
    lc.setDigit(1,6,3,true); 
  } else if (ventSec==33) {
    lc.setDigit(1,7,3,true); 
    lc.setDigit(1,6,3,true); 
  } else if (ventSec==32) {
    lc.setDigit(1,7,2,true); 
    lc.setDigit(1,6,3,true); 
  } else if (ventSec==31) {
    lc.setDigit(1,7,1,false); 
    lc.setDigit(1,6,3,true); 
  } else if (ventSec==30) {
    lc.setDigit(1,7,0,false); 
    lc.setDigit(1,6,3,true); 
  } else if (ventSec==29) {
    lc.setDigit(1,7,9,true); 
    lc.setDigit(1,6,2,true); 
  } else if (ventSec==28) {
    lc.setDigit(1,7,8,true); 
    lc.setDigit(1,6,2,true); 
  } else if (ventSec==27) {
    lc.setDigit(1,7,7,false); 
    lc.setDigit(1,6,2,true); 
  } else if (ventSec==26) {
    lc.setDigit(1,7,6,true); 
    lc.setDigit(1,6,2,true); 
  } else if (ventSec==25) {
    lc.setDigit(1,7,5,true); 
    lc.setDigit(1,6,2,true); 
  } else if (ventSec==24) {
    lc.setDigit(1,7,4,true); 
    lc.setDigit(1,6,2,true); 
  } else if (ventSec==23) {
    lc.setDigit(1,7,3,true); 
    lc.setDigit(1,6,2,true); 
  } else if (ventSec==22) {
    lc.setDigit(1,7,2,true); 
    lc.setDigit(1,6,2,true); 
  } else if (ventSec==21) {
    lc.setDigit(1,7,1,false); 
    lc.setDigit(1,6,2,true); 
  } else if (ventSec==20) {
    lc.setDigit(1,7,0,false); 
    lc.setDigit(1,6,2,true); 
  } else if (ventSec==19) {
    lc.setDigit(1,7,9,true); 
    lc.setDigit(1,6,1,false); 
  } else if (ventSec==18) {
    lc.setDigit(1,7,8,true); 
    lc.setDigit(1,6,1,false); 
  } else if (ventSec==17) {
    lc.setDigit(1,7,7,false); 
    lc.setDigit(1,6,1,false); 
  } else if (ventSec==16) {
    lc.setDigit(1,7,6,true); 
    lc.setDigit(1,6,1,false); 
  } else if (ventSec==15) {
    lc.setDigit(1,7,5,true); 
    lc.setDigit(1,6,1,false); 
  } else if (ventSec==14) {
    lc.setDigit(1,7,4,true); 
    lc.setDigit(1,6,1,false); 
  } else if (ventSec==13) {
    lc.setDigit(1,7,3,true); 
    lc.setDigit(1,6,1,false); 
  } else if (ventSec==12) {
    lc.setDigit(1,7,2,true); 
    lc.setDigit(1,6,1,false); 
  } else if (ventSec==11) {
    lc.setDigit(1,7,1,false); 
    lc.setDigit(1,6,1,false); 
  } else if (ventSec==10) {
    lc.setDigit(1,7,0,false); 
    lc.setDigit(1,6,1,false); 
  } else if (ventSec==9) {
    lc.setDigit(1,7,9,true); 
    lc.setDigit(1,6,0,false); 
  } else if (ventSec==8) {
    lc.setDigit(1,7,8,true); 
    lc.setDigit(1,6,0,false); 
  } else if (ventSec==7) {
    lc.setDigit(1,7,7,false); 
    lc.setDigit(1,6,0,false); 
  } else if (ventSec==6) {
    lc.setDigit(1,7,6,true); 
    lc.setDigit(1,6,0,false); 
  } else if (ventSec==5) {
    lc.setDigit(1,7,5,true); 
    lc.setDigit(1,6,0,false); 
  } else if (ventSec==4) {
    lc.setDigit(1,7,4,true); 
    lc.setDigit(1,6,0,false); 
  } else if (ventSec==3) {
    lc.setDigit(1,7,3,true); 
    lc.setDigit(1,6,0,false); 
  } else if (ventSec==2) {
    lc.setDigit(1,7,2,true); 
    lc.setDigit(1,6,0,false); 
  } else if (ventSec==1) {
    lc.setDigit(1,7,1,false); 
    lc.setDigit(1,6,0,false); 
  } else if (ventSec==0) {
    lc.setDigit(1,7,0,false);
    lc.setDigit(1,6,0,false);
  } else {
    lc.setDigit(1,7,' ',false);
    lc.setDigit(1,6,' ',false); 
    }
}

void ventingPrintQuestion()
{
  if (questionSelect>0) {
    lcdVent.clear();
    lcdVent.setCursor(3, 0); lcdVent.print("VENT GAS?");
    lcdVent.setCursor(6, 1); lcdVent.print("Y/N");
  } else if (questionSelect==0) {
    lcdVent.clear();
    lcdVent.setCursor(3, 0); lcdVent.print("DETONATE?");
    lcdVent.setCursor(6, 1); lcdVent.print("Y/N");
  }
}

void checkVentSubmission(byte yn)
{
  if (questionSelect>0) { //vent gas display
    if (yn==1) {
      //It needs to slowly spells out your answer if it was correct.
      startVentMillisMax = (random(5,30)*1000);
      startVentMillis = millis();  
      isTimePassed = false;
      isAnyModuleDefused = false;
      ventStartedYN = false;
      lcdVent.setCursor(0, 0); lcdVent.print("VENTING COMPLETE");
      lcdVent.setCursor(0, 1); lcdVent.print("================");
      lc.setDigit(1,7,' ',false);
      lc.setDigit(1,6,' ',false); 
    } else if (yn==0) {
      // should keep counting down and show new text:
      // and then after a few seconds it will go back to displaying the original question "VENT GAS? Y/N" until time runs out
      lcdVent.clear();
      lcdVent.setCursor(0, 0); lcdVent.print("VENTING PREVENTS");
      lcdVent.setCursor(3, 1); lcdVent.print("EXPLOSIONS");
      ventingPreventsExplosionsTime = millis();
      ventingPreventsExplosionsTF = true;
    }
  } else if (questionSelect==0) { //detonate display
    if (yn==1) {
      bombExploded();
    } else if (yn==0) {
      //It needs to slowly spells out your answer if it was correct.
      startVentMillisMax = (random(5,30)*1000);
      startVentMillis = millis();  
      isTimePassed = false;
      isAnyModuleDefused = false; 
      ventStartedYN = false;
      lcdVent.clear();
      lcdVent.noBacklight();
      lc.setDigit(1,7,' ',false);
      lc.setDigit(1,6,' ',false); 
    }
  }
}

void ventingSetup()
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  pinMode(PIN_VENTING_BUTTON_1, INPUT);
  pinMode(PIN_VENTING_BUTTON_2, INPUT);
  lcdVent.init();
  lcdVent.noBacklight();
}

void checkVentSwitches()
{
  ventYesState = digitalRead(PIN_VENTING_BUTTON_1);
  ventNoState = digitalRead(PIN_VENTING_BUTTON_2);

// compare the passwordPos1UpState to its previous state
  if (ventYesState != lastVentYesState) {
    if (ventYesState == HIGH) {
      if (DEBUG_LEVEL >= 2) {
      Serial.println("VentButtonYESpressed");
      }
      checkVentSubmission(1);
    }
  }
  // save the current state as the last state, for next time through the loop
  lastVentYesState = ventYesState;

// compare the passwordPos1UpState to its previous state
  if (ventNoState != lastVentNoState) {
    if (ventNoState == HIGH) {
      if (DEBUG_LEVEL >= 2) {
      Serial.println("VentButtonNOpressed");
      }
      checkVentSubmission(0);
    }
  }
  // save the current state as the last state, for next time through the loop
  lastVentNoState = ventNoState;
}

void ventingLoop()
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  
  if (millis() - startVentMillis >= startVentMillisMax) {
    isTimePassed = true;
  }

  if (ventingPreventsExplosionsTF==true) {
    if (millis() - ventingPreventsExplosionsTime >= 3000) {
      ventingPrintQuestion();
      ventingPreventsExplosionsTF = false;
    }
  }
  
  if (isAnyModuleDefused==true || isTimePassed==true) {
    if (ventStartedYN==false) {
      if (DEBUG_LEVEL >= 2) {
      Serial.println("VentWasNotStarted-StartingNow");
      }
      ventSec = VENTING_DEFAULT_TIME + 1;
      ventSeconds = millis();
      lcdVent.backlight();
      lcdVent.on();
      lcdVent.clear();
      questionSelect = random(0,10); 
      if (DEBUG_LEVEL >= 2) {
      Serial.println("VentingOrDetonate");
      Serial.println("?? > 0 Or = 0 ??");
      Serial.println(questionSelect);
      }
      ventingPrintQuestion();
      ventStartedYN = true;
    } else if (ventStartedYN==true) {
        if (millis() - ventSwitchMillis > 50) {
          //restart the TIMER
          ventSwitchMillis = ventCurrentMillis;
          checkVentSwitches();
        }
        if (ventSeconds < millis()) {
          ventSeconds += 1000;
          ventSec--;
          Serial.println(ventSec);
          ventDigitDisplay();
          if (ventSec == -1) {
            addStrike();
            startVentMillisMax = (random(5,30)*1000);
            startVentMillis = millis();   
            isTimePassed = false;  
            isAnyModuleDefused = false; 
            ventStartedYN = false;
            lcdVent.clear();
            lcdVent.noBacklight();
          }
        }
    }
  }
}

void ventingBombDefused()
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  lcdVent.clear();
  lcdVent.setCursor(2, 0); lcdVent.print("BOMB DEFUSED");
  lcdVent.setCursor(0, 1); lcdVent.print("================");
  // when the bomb explodes the venting module should clear the timer and the display
  //clear the COUNTDOWN timer
}

void ventingModuleBoom()
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  lcdVent.clear();
  lcdVent.setCursor(6, 0); lcdVent.print("BOMB");
  lcdVent.setCursor(4, 1); lcdVent.print("EXPLODED");
  // when the bomb explodes the venting module should clear the timer and the display
  //clear the COUNTDOWN timer
}
