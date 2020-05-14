// On the Subject of Passwords
/*
  KNOWN ISSUES:
  PIN ASSIGMENTS ARE WRONG & NEED UPDATED :)
*/
#define PIN_PASSWORD_LED_FIN 97 // module complete led
#define PIN_PASSWORD_BUTTON_1 34 // letter 1 up // invalid pin max number is 53
#define PIN_PASSWORD_BUTTON_2 33 // letter 2 up // invalid pin max number is 53
#define PIN_PASSWORD_BUTTON_3 32 // letter 3 up // invalid pin max number is 53
#define PIN_PASSWORD_BUTTON_4 31 // letter 4 up // invalid pin max number is 53
#define PIN_PASSWORD_BUTTON_5 30 // letter 5 up // invalid pin max number is 53
#define PIN_PASSWORD_BUTTON_6 26 // letter 1 down // invalid pin max number is 53
#define PIN_PASSWORD_BUTTON_7 25 // letter 2 down
#define PIN_PASSWORD_BUTTON_8 24 // letter 3 down // invalid pin max number is 53
#define PIN_PASSWORD_BUTTON_9 23 // letter 4 down // invalid pin max number is 53
#define PIN_PASSWORD_BUTTON_10 22 // letter 5 down
#define PIN_PASSWORD_BUTTON_SUBMIT 40 // submit button

unsigned long switchMillis = 0;

int passwordPos1UpState = 0; // current state of the 1st pos up button
int passwordPos2UpState = 0; // current state of the 2nd pos up button
int passwordPos3UpState = 0; // current state of the 3rd pos up button
int passwordPos4UpState = 0; // current state of the 4th pos up button
int passwordPos5UpState = 0; // current state of the 5th pos up button

int passwordPos1DownState = 0; // current state of the 1st pos Down button
int passwordPos2DownState = 0; // current state of the 2nd pos Down button
int passwordPos3DownState = 0; // current state of the 3rd pos Down button
int passwordPos4DownState = 0; // current state of the 4th pos Down button
int passwordPos5DownState = 0; // current state of the 5th pos Down button

int passwordSubmitState = 0; // current state of the submit button

int lastPasswordPos1UpState = 0; // previous state of the 1st pos up button
int lastPasswordPos2UpState = 0; // previous state of the 2nd pos up button
int lastPasswordPos3UpState = 0; // previous state of the 3rd pos up button
int lastPasswordPos4UpState = 0; // previous state of the 4th pos up button
int lastPasswordPos5UpState = 0; // previous state of the 5th pos up button

int lastPasswordPos1DownState = 0; // previous state of the 1st pos Down button
int lastPasswordPos2DownState = 0; // previous state of the 2nd pos Down button
int lastPasswordPos3DownState = 0; // previous state of the 3rd pos Down button
int lastPasswordPos4DownState = 0; // previous state of the 4th pos Down button
int lastPasswordPos5DownState = 0; // previous state of the 5th pos Down button

int lastPasswordSubmitState = 0; // previous state of the submit button

char displayVals[5] = {0,0,0,0,0};

char positionVals[5][6] = {
 {0,0,0,0,0,0},
 {0,0,0,0,0,0},
 {0,0,0,0,0,0},
 {0,0,0,0,0,0},
 {0,0,0,0,0,0}
};

int currentPositionPos[] = {0,0,0,0,0};

char validWords[35][5] = {
 {'a','b','o','u','t'},
 {'a','f','t','e','r'},
 {'a','g','a','i','n'},
 {'b','e','l','o','w'},
 {'c','o','u','l','d'},

 {'e','v','e','r','y'},
 {'f','i','r','s','t'},
 {'f','o','u','n','d'},
 {'g','r','e','a','t'},
 {'h','o','u','s','e'},

 {'l','a','r','g','e'},
 {'l','e','a','r','n'},
 {'n','e','v','e','r'},
 {'o','t','h','e','r'},
 {'p','l','a','c','e'},

 {'p','l','a','n','t'},
 {'p','o','i','n','t'},
 {'r','i','g','h','t'},
 {'s','m','a','l','l'},
 {'s','o','u','n','d'},

 {'s','p','e','l','l'},
 {'s','t','i','l','l'},
 {'s','t','u','d','y'},
 {'t','h','e','i','r'},
 {'t','h','e','r','e'},

 {'t','h','e','s','e'},
 {'t','h','i','n','g'},
 {'t','h','i','n','k'},
 {'t','h','r','e','e'},
 {'w','a','t','e','r'},

 {'w','h','e','r','e'},
 {'w','h','i','c','h'},
 {'w','o','r','l','d'},
 {'w','o','u','l','d'},
 {'w','r','i','t','e'}
};

void setInitDisplayLetters(int p) 
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
  for(int i = 0; i < 5; i++)
  {
    Serial.print(displayVals[i]);
  }
  Serial.println("");
}

void setDisplayLetters(int bp, bool pm)
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  int currentLetterPos = currentPositionPos[bp];
  int nextLetterPos;
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
  int positionPopulated = 0;
  while (positionPopulated<5)
  {
    char letter = validWords[randomWord][positionPopulated];
    positionVals[positionPopulated][0] = letter;
    positionPopulated ++;
  }
}

void genRandLetters(int p)
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  int generated=1;
  while (generated<6)
  {
     byte randomValue = random(0, 26);
     char letter = randomValue + 'a';
     positionVals[p][generated] = letter;
     generated ++;
  }
}

void checkSubmission()
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  if ((displayVals[0]==positionVals[0][0]) &&
     (displayVals[1]==positionVals[1][0]) &&
     (displayVals[2]==positionVals[2][0]) &&
     (displayVals[3]==positionVals[3][0]) &&
     (displayVals[4]==positionVals[4][0])) {
    Serial.println("PWmoduledefused");
    passwordModuleDefused = true;
   } else {
      addStrike();
   }
}

void checkSwitches()
{
  if (DEBUG_LEVEL >= 2) {
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
      checkSubmission();
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
  unsigned long currentMillis = millis();
  if (currentMillis - switchMillis > 50) {
    //restart the TIMER
    switchMillis = currentMillis;
    checkSwitches();
  }
}

void passwordSetup() 
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  Serial.begin(9600);
  
  pinMode(PIN_PASSWORD_LED_FIN, OUTPUT);
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
  
  randomSeed(analogRead(0));
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

  if (DEBUG_LEVEL >= 2) {
    Serial.println("Display Letters");
    for(int i = 0; i < 5; i++)
    {
      Serial.print(displayVals[i]);
    }
    Serial.println("");
    Serial.println("Correct Letters");
    for(int i = 0; i < 5; i++)
    {
      Serial.print(positionVals[i][0]);
    }
    Serial.println("");
    Serial.println("POS1 Letters");
    for(int i = 0; i < 6; i++)
    {
      Serial.print(positionVals[0][i]);
    }
    Serial.println("");
    Serial.println("POS2 Letters");
    for(int i = 0; i < 6; i++)
    {
      Serial.print(positionVals[1][i]);
    }
    Serial.println("");
    Serial.println("POS3 Letters");
    for(int i = 0; i < 6; i++)
    {
      Serial.print(positionVals[2][i]);
    }
    Serial.println("");
    Serial.println("POS4 Letters");
    for(int i = 0; i < 6; i++)
    {
      Serial.print(positionVals[3][i]);
    }
    Serial.println("");
    Serial.println("POS5 Letters");
    for(int i = 0; i < 6; i++)
    {
      Serial.print(positionVals[4][i]);
    }
    Serial.println("");
  }
}
