// On the Subject of Passwords
/*
  KNOWN ISSUES:
*/
#include "Adafruit_LEDBackpack.h"

#define PIN_PASSWORD_LED_FIN 2,4,3 // module complete led
#define PIN_PASSWORD_BUTTON_1 33 // letter 1 up
#define PIN_PASSWORD_BUTTON_2 34 // letter 2 up
#define PIN_PASSWORD_BUTTON_3 35 // letter 3 up
#define PIN_PASSWORD_BUTTON_4 36 // letter 4 up
#define PIN_PASSWORD_BUTTON_5 37 // letter 5 up
#define PIN_PASSWORD_BUTTON_6 38 // letter 1 down
#define PIN_PASSWORD_BUTTON_7 39 // letter 2 down
#define PIN_PASSWORD_BUTTON_8 40 // letter 3 down
#define PIN_PASSWORD_BUTTON_9 41 // letter 4 down
#define PIN_PASSWORD_BUTTON_10 42 // letter 5 down
#define PIN_PASSWORD_BUTTON_SUBMIT 43 // submit button

Adafruit_AlphaNum4 alpha5 = Adafruit_AlphaNum4();

unsigned long PWswitchMillis = 0;

byte passwordPos1UpState = 0; // current state of the 1st pos up button
byte passwordPos2UpState = 0; // current state of the 2nd pos up button
byte passwordPos3UpState = 0; // current state of the 3rd pos up button
byte passwordPos4UpState = 0; // current state of the 4th pos up button
byte passwordPos5UpState = 0; // current state of the 5th pos up button

byte passwordPos1DownState = 0; // current state of the 1st pos Down button
byte passwordPos2DownState = 0; // current state of the 2nd pos Down button
byte passwordPos3DownState = 0; // current state of the 3rd pos Down button
byte passwordPos4DownState = 0; // current state of the 4th pos Down button
byte passwordPos5DownState = 0; // current state of the 5th pos Down button

byte passwordSubmitState = 0; // current state of the submit button

byte lastPasswordPos1UpState = 0; // previous state of the 1st pos up button
byte lastPasswordPos2UpState = 0; // previous state of the 2nd pos up button
byte lastPasswordPos3UpState = 0; // previous state of the 3rd pos up button
byte lastPasswordPos4UpState = 0; // previous state of the 4th pos up button
byte lastPasswordPos5UpState = 0; // previous state of the 5th pos up button

byte lastPasswordPos1DownState = 0; // previous state of the 1st pos Down button
byte lastPasswordPos2DownState = 0; // previous state of the 2nd pos Down button
byte lastPasswordPos3DownState = 0; // previous state of the 3rd pos Down button
byte lastPasswordPos4DownState = 0; // previous state of the 4th pos Down button
byte lastPasswordPos5DownState = 0; // previous state of the 5th pos Down button

byte lastPasswordSubmitState = 0; // previous state of the submit button

char displayVals[5] = {0,0,0,0,0};

byte randomArray[6]{0,0,0,0,0,0};

char positionVals[5][6] = {
 {0,0,0,0,0,0},
 {0,0,0,0,0,0},
 {0,0,0,0,0,0},
 {0,0,0,0,0,0},
 {0,0,0,0,0,0}
};

byte currentPositionPos[] = {0,0,0,0,0};

char validWords[35][5] = {
  {'A','B','O','U','T'},
  {'A','F','T','E','R'},
  {'A','G','A','I','N'},
  {'B','E','L','O','W'},
  {'C','O','U','L','D'},
  {'E','V','E','R','Y'},
  {'F','I','R','S','T'},
  {'F','O','U','N','D'},
  {'G','R','E','A','T'},
  {'H','O','U','S','E'},
  {'L','A','R','G','E'},
  {'L','E','A','R','N'},
  {'N','E','V','E','R'},
  {'O','T','H','E','R'},
  {'P','L','A','C','E'},
  {'P','L','A','N','T'},
  {'P','O','I','N','T'},
  {'R','I','G','H','T'},
  {'S','M','A','L','L'},
  {'S','O','U','N','D'},
  {'S','P','E','L','L'},
  {'S','T','I','L','L'},
  {'S','T','U','D','Y'},
  {'T','H','E','I','R'},
  {'T','H','E','R','E'},
  {'T','H','E','S','E'},
  {'T','H','I','N','G'},
  {'T','H','I','N','K'},
  {'T','H','R','E','E'},
  {'W','A','T','E','R'},
  {'W','H','E','R','E'},
  {'W','H','I','C','H'},
  {'W','O','R','L','D'},
  {'W','O','U','L','D'},
  {'W','R','I','T','E'}
};

void setInitDisplayLetters(byte p) 
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  byte randomLetter = random(0,6);
  displayVals[p] = positionVals[p][randomLetter];
  currentPositionPos[p] = randomLetter;
}

void updatePasswordDisplay()
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  for(byte i = 0; i < 5; i++)
  {
    Serial.print(displayVals[i]);
    alpha5.writeDigitAscii(i, displayVals[i]);
    alpha5.writeDisplay();
  }
  Serial.println(F(""));
}

void setDisplayLetters(byte bp, bool pm)
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  byte currentLetterPos = currentPositionPos[bp];
  byte nextLetterPos;
  if (pm==true) {
    if (currentLetterPos==5) {
      nextLetterPos = 0;
      currentPositionPos[bp] = 0;
    } else {
      nextLetterPos = currentLetterPos+1;
      currentPositionPos[bp] = currentPositionPos[bp]+1;
    }
  } else if (pm==false) {
    if (currentLetterPos==0) {
      nextLetterPos = 5;
      currentPositionPos[bp] = 5;
    } else {
      nextLetterPos = currentLetterPos-1;
      currentPositionPos[bp] = currentPositionPos[bp]-1;
    }
  }
  displayVals[bp] = positionVals[bp][nextLetterPos];
  updatePasswordDisplay();
}

void genCorrectLetters()
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  byte randomWord = random(0,35);
  byte positionPopulated = 0;
  while (positionPopulated<5)
  {
    char letter = validWords[randomWord][positionPopulated];
    positionVals[positionPopulated][0] = letter;
    positionPopulated ++;
  }
}

void genRandLetters(byte p)
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  
  for(byte i = 1; i<6; i++){
      byte value;
      boolean check = false;
  
      while(check == false){
          value = random(0, 26);
          check = true;
          for(byte j = 1; j<6; j++){
            if(value == randomArray[j]){
              check = false;
            } 
          }
          char valueLetter = value + 'A';
          if (valueLetter == positionVals[p][0]) {
            check = false;
          }
      }
      randomArray[i] = value;
      char letter = value + 'A';
      positionVals[p][i] = letter;
  }
}

void checkPWSubmission()
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  if ((displayVals[0]==positionVals[0][0]) &&
     (displayVals[1]==positionVals[1][0]) &&
     (displayVals[2]==positionVals[2][0]) &&
     (displayVals[3]==positionVals[3][0]) &&
     (displayVals[4]==positionVals[4][0])) {
    Serial.println(F("PWmoduledefused"));
    lc.setLed(PIN_PASSWORD_LED_FIN,true);
    passwordModuleDefused = true;
    defusedModuleBuzzer();
    isAnyModuleDefused=true;
   } else {
      addStrike();
   }
}

void checkPWSwitches()
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  // put your main code here, to run repeatedly:
  passwordPos1UpState = digitalRead(PIN_PASSWORD_BUTTON_1);
  passwordPos2UpState = digitalRead(PIN_PASSWORD_BUTTON_2);
  passwordPos3UpState = digitalRead(PIN_PASSWORD_BUTTON_3);
  passwordPos4UpState = digitalRead(PIN_PASSWORD_BUTTON_4);
  passwordPos5UpState = digitalRead(PIN_PASSWORD_BUTTON_5);
  passwordPos1DownState = digitalRead(PIN_PASSWORD_BUTTON_6);
  passwordPos2DownState = digitalRead(PIN_PASSWORD_BUTTON_7);
  passwordPos3DownState = digitalRead(PIN_PASSWORD_BUTTON_8);
  passwordPos4DownState = digitalRead(PIN_PASSWORD_BUTTON_9);
  passwordPos5DownState = digitalRead(PIN_PASSWORD_BUTTON_10);
  passwordSubmitState = digitalRead(PIN_PASSWORD_BUTTON_SUBMIT);
  
/////////UP BUTTONS////////////
// compare the passwordPos1UpState to its previous state
  if (passwordPos1UpState != lastPasswordPos1UpState) {
    if (passwordPos1UpState == HIGH) {
      setDisplayLetters(0, true);
    }
  }
  // save the current state as the last state, for next time through the loop
  lastPasswordPos1UpState = passwordPos1UpState;

// compare the passwordPos2UpState to its previous state
  if (passwordPos2UpState != lastPasswordPos2UpState) {
    if (passwordPos2UpState == HIGH) {
      setDisplayLetters(1, true);
    }
  }
  // save the current state as the last state, for next time through the loop
  lastPasswordPos2UpState = passwordPos2UpState;

// compare the passwordPos3UpState to its previous state
  if (passwordPos3UpState != lastPasswordPos3UpState) {
    if (passwordPos3UpState == HIGH) {
      setDisplayLetters(2, true);
    }
  }
  // save the current state as the last state, for next time through the loop
  lastPasswordPos3UpState = passwordPos3UpState;
  
// compare the passwordPos4UpState to its previous state
  if (passwordPos4UpState != lastPasswordPos4UpState) {
    if (passwordPos4UpState == HIGH) {
      setDisplayLetters(3, true);
    }
  }
  // save the current state as the last state, for next time through the loop
  lastPasswordPos4UpState = passwordPos4UpState;
  
// compare the passwordPos5UpState to its previous state
  if (passwordPos5UpState != lastPasswordPos5UpState) {
    if (passwordPos5UpState == HIGH) {
      setDisplayLetters(4, true);
    }
  }
  // save the current state as the last state, for next time through the loop
  lastPasswordPos5UpState = passwordPos5UpState;


/////////DOWN BUTTONS////////////
// compare the passwordPos1DownState to its previous state
  if (passwordPos1DownState != lastPasswordPos1DownState) {
    if (passwordPos1DownState == HIGH) {
      setDisplayLetters(0, false);
    }
  }
  // save the current state as the last state, for next time through the loop
  lastPasswordPos1DownState = passwordPos1DownState;

// compare the passwordPos2DownState to its previous state
  if (passwordPos2DownState != lastPasswordPos2DownState) {
    if (passwordPos2DownState == HIGH) {
      setDisplayLetters(1, false);
    }
  }
  // save the current state as the last state, for next time through the loop
  lastPasswordPos2DownState = passwordPos2DownState;

// compare the passwordPos3DownState to its previous state
  if (passwordPos3DownState != lastPasswordPos3DownState) {
    if (passwordPos3DownState == HIGH) {
      setDisplayLetters(2, false);
    }
  }
  // save the current state as the last state, for next time through the loop
  lastPasswordPos3DownState = passwordPos3DownState;
  
// compare the passwordPos4DownState to its previous state
  if (passwordPos4DownState != lastPasswordPos4DownState) {
    if (passwordPos4DownState == HIGH) {
      setDisplayLetters(3, false);
    }
  }
  // save the current state as the last state, for next time through the loop
  lastPasswordPos4DownState = passwordPos4DownState;
  
// compare the passwordPos5DownState to its previous state 
  if (passwordPos5DownState != lastPasswordPos5DownState) {
    if (passwordPos5DownState == HIGH) {
      setDisplayLetters(4, false);
    }
  }
  // save the current state as the last state, for next time through the loop
  lastPasswordPos5DownState = passwordPos5DownState; 
  
// compare the passwordPos5DownState to its previous state 
  if (passwordSubmitState != lastPasswordSubmitState) {
    if (passwordSubmitState == HIGH) {
      checkPWSubmission();
    }
  }
  // save the current state as the last state, for next time through the loop
  lastPasswordSubmitState = passwordSubmitState;
}

void passwordModuleBoom()
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
}

void passwordLoop() 
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  unsigned long PWcurrentMillis = millis();
  if (PWcurrentMillis - PWswitchMillis > 50) {
    //restart the TIMER
    PWswitchMillis = PWcurrentMillis;
    checkPWSwitches();
  }
}

void passwordSetup() 
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  
  alpha5.begin(0x70);
  alpha5.clear();
  
  pinMode(PIN_PASSWORD_BUTTON_1, INPUT);
  pinMode(PIN_PASSWORD_BUTTON_2, INPUT);
  pinMode(PIN_PASSWORD_BUTTON_3, INPUT);
  pinMode(PIN_PASSWORD_BUTTON_4, INPUT);
  pinMode(PIN_PASSWORD_BUTTON_5, INPUT);
  pinMode(PIN_PASSWORD_BUTTON_6, INPUT);
  pinMode(PIN_PASSWORD_BUTTON_7, INPUT);
  pinMode(PIN_PASSWORD_BUTTON_8, INPUT);
  pinMode(PIN_PASSWORD_BUTTON_9, INPUT);
  pinMode(PIN_PASSWORD_BUTTON_10, INPUT);
  pinMode(PIN_PASSWORD_BUTTON_SUBMIT, INPUT);
  
  genCorrectLetters();
  genRandLetters(0);
  genRandLetters(1);
  genRandLetters(2);
  genRandLetters(3);
  genRandLetters(4);
  setInitDisplayLetters(0);
  setInitDisplayLetters(1);
  setInitDisplayLetters(2);
  setInitDisplayLetters(3);
  setInitDisplayLetters(4);

  updatePasswordDisplay();
  
  if (DEBUG_LEVEL >= 1) {
    Serial.println(F("Correct Letters"));
    for(byte i = 0; i < 5; i++)
    {
      Serial.print(positionVals[i][0]);
    }
    Serial.println(F(""));
  }
  if (DEBUG_LEVEL >= 2) {
    Serial.println(F("POS1 Letters"));
    for(byte i = 0; i < 6; i++)
    {
      Serial.print(positionVals[0][i]);
    }
    Serial.println(F(""));
    Serial.println(F("POS2 Letters"));
    for(byte i = 0; i < 6; i++)
    {
      Serial.print(positionVals[1][i]);
    }
    Serial.println(F(""));
    Serial.println(F("POS3 Letters"));
    for(byte i = 0; i < 6; i++)
    {
      Serial.print(positionVals[2][i]);
    }
    Serial.println(F(""));
    Serial.println(F("POS4 Letters"));
    for(byte i = 0; i < 6; i++)
    {
      Serial.print(positionVals[3][i]);
    }
    Serial.println(F(""));
    Serial.println(F("POS5 Letters"));
    for(byte i = 0; i < 6; i++)
    {
      Serial.print(positionVals[4][i]);
    }
    Serial.println(F(""));
  }
  if (DEBUG_LEVEL >= 1) {
    Serial.println(F("Display Letters"));
    for(byte i = 0; i < 5; i++)
    {
      Serial.print(displayVals[i]);
    }
    Serial.println(F(""));
  }
}
