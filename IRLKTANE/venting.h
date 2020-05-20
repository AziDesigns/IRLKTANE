// On the Subject of Venting Gas
/*
  KNOWN ISSUES:
  MODULE IS WIP AND THERE ARE MANY MANY ISSUES & MISSING LOGIC.
*/
#define PIN_VENTING_BUTTON_1 27 // YES BUTTON // PINOVERLAP
#define PIN_VENTING_BUTTON_2 28 // NO BUTTON // PINOVERLAP

#define VENTING_DEFAULT_TIME 40 // STARTING TIME WHEN MODULE RESETS

unsigned long ventSeconds;
byte ventSec;

unsigned long startVentMillisMax = 0;//60000; //MAX TIME IN MS BEFORE MODULE STARTS IF NO MODULE IS DEFUSED
unsigned long startVentMillis = 0; //TIME UNTIL NEXT VENT NEEDED
unsigned long ventCurrentMillis = 0; //TIME UNTIL NEXT VENT NEEDED
unsigned long ventSwitchMillis = 0; // DEBOUNCE PREVENTION

unsigned long ventingPreventsExplosionsTime; // if user presses no on vent gas track time since press
bool ventingPreventsExplosionsTF = false;

byte ventYesState = 0; // current state of the yes button
byte ventNoState = 0; // current state of the no button
byte lastVentYesState = 0; // previous state of the yes button
byte lastVentNoState = 0; // previous state of the no button

byte questionSelect; // 0=vent gas ?, 1=detonate ?

bool ventStartedYN = false;
bool isTimePassed = false;

LiquidCrystal_I2C lcdVent(0x26, 16, 2);

void ventingPrintQuestion()
{
  if (questionSelect>0) {
    lcdVent.clear();
    lcdVent.setCursor(3, 0); lcdVent.print(F("VENT GAS?"));
    lcdVent.setCursor(6, 1); lcdVent.print(F("Y/N"));
  } else if (questionSelect==0) {
    lcdVent.clear();
    lcdVent.setCursor(3, 0); lcdVent.print(F("DETONATE?"));
    lcdVent.setCursor(6, 1); lcdVent.print(F("Y/N"));
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
      lcdVent.setCursor(0, 0); lcdVent.print(F("VENTING COMPLETE"));
      lcdVent.setCursor(0, 1); lcdVent.print(F("================"));
      lc.setDigit(1,7,' ',false);
      lc.setDigit(1,6,' ',false); 
    } else if (yn==0) {
      // should keep counting down and show new text:
      // and then after a few seconds it will go back to displaying the original question "VENT GAS? Y/N" until time runs out
      lcdVent.clear();
      lcdVent.setCursor(0, 0); lcdVent.print(F("VENTING PREVENTS"));
      lcdVent.setCursor(0, 1); lcdVent.print(F("ABCDEFGHIJKLMNOP"));
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
      Serial.println(F("VentButtonYESpressed"));
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
      Serial.println(F("VentButtonNOpressed"));
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
      Serial.println(F("VentWasNotStarted-StartingNow"));
      }
      ventSec = VENTING_DEFAULT_TIME + 1;
      ventSeconds = millis();
      lcdVent.backlight();
      lcdVent.on();
      lcdVent.clear();
      questionSelect = random(0,10); 
      if (DEBUG_LEVEL >= 2) {
      Serial.println(F("VentingOrDetonate"));
      Serial.println(F("?? > 0 Or = 0 ??"));
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
          needyDigitDisplay(1, 6, 7, ventSec);
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
  lcdVent.setCursor(2, 0); lcdVent.print(F("BOMB DEFUSED"));
  lcdVent.setCursor(0, 1); lcdVent.print(F("================"));
  // when the bomb explodes the venting module should clear the timer and the display
  //clear the COUNTDOWN timer
}

void ventingModuleBoom()
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  lcdVent.clear();
  lcdVent.setCursor(6, 0); lcdVent.print(F("BOMB"));
  lcdVent.setCursor(4, 1); lcdVent.print(F("EXPLODED"));
  // when the bomb explodes the venting module should clear the timer and the display
  //clear the COUNTDOWN timer
}
