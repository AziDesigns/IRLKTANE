// On the Subject of Buttons
/*
  KNOWN ISSUES:
*/
#define PIN_BUTTON_LED_FIN 2,0,1 // module complete LED
#define PIN_BUTTON_BTN 23
#define PIN_BUTTON_LEFT_LED_RED 13
#define PIN_BUTTON_LEFT_LED_GREEN 7
#define PIN_BUTTON_LEFT_LED_BLUE 11
#define PIN_BUTTON_RIGHT_LED_RED 6
#define PIN_BUTTON_RIGHT_LED_GREEN 5
#define PIN_BUTTON_RIGHT_LED_BLUE 4

LiquidCrystal_I2C lcdButton(0x25, 16, 2);

byte buttonBtnState = LOW;
byte lastBtnState = LOW;
unsigned long lastDebounceTime = 0;
byte lengthOfPress = 0;
byte leftLedColor;
byte rightLedColor;
byte btnWordGen;
bool buttonColorSet = false;
bool buttonStripColorSet = false;

void setColor (byte redPin, byte redValue, byte greenPin, byte greenValue, byte bluePin, byte blueValue)
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}

void buttonModuleDefusedPrint()
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  setColor(PIN_BUTTON_LEFT_LED_RED, 0, PIN_BUTTON_LEFT_LED_GREEN, 0, PIN_BUTTON_LEFT_LED_BLUE, 0);
  setColor(PIN_BUTTON_RIGHT_LED_RED, 0, PIN_BUTTON_RIGHT_LED_GREEN, 0, PIN_BUTTON_RIGHT_LED_BLUE, 0);
  buttonModuleDefused = true;
  defusedModuleBuzzer();
  isAnyModuleDefused=true;
  lc.setLed(PIN_BUTTON_LED_FIN,true);
  lcdButton.clear();
  lcdButton.setCursor(1, 0);
  lcdButton.print(F("MODULE DEFUSED"));
  lcdButton.setCursor(0, 1);
  lcdButton.print(F("Serial: "));
  lcdButton.print(serialCode);
}

void buttonModuleBoom()
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  setColor(PIN_BUTTON_LEFT_LED_RED, 0, PIN_BUTTON_LEFT_LED_GREEN, 0, PIN_BUTTON_LEFT_LED_BLUE, 0);
  setColor(PIN_BUTTON_RIGHT_LED_RED, 0, PIN_BUTTON_RIGHT_LED_GREEN, 0, PIN_BUTTON_RIGHT_LED_BLUE, 0);
  lcdButton.clear();
  lcdButton.setCursor(6, 0);
  lcdButton.print(F("BOMB"));
  lcdButton.setCursor(4, 1);
  lcdButton.print(F("EXPLODED"));
  lc.setLed(PIN_BUTTON_LED_FIN,false);
}

bool checkClock(byte value)
{
  byte timer = sec;
  while (timer > 0)
  {
    if (timer % 10 == value)
      return 1;
    timer = timer / 10;
  }

  timer = mins;
  while (timer > 0)
  {
    if (timer % 10 == value)
      return 1;
    timer = timer / 10;
  }

  return 0;
}

void printWord() 
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  switch (btnWordGen)
  {
    case 1:
      { //HOLD
        lcdButton.setCursor(5, 0);
        lcdButton.print(F("HOLD"));
      }
      break;
    case 2:
      { //DETONATE
        lcdButton.setCursor(4, 0);
        lcdButton.print(F("DETONATE"));
      }
      break;
    case 3:
      { //ABORT
        lcdButton.setCursor(5, 0);
        lcdButton.print(F("ABORT"));
      }
      break;
  }
}

void buttonSetup()
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  pinMode(PIN_BUTTON_LEFT_LED_RED, OUTPUT);
  pinMode(PIN_BUTTON_LEFT_LED_GREEN, OUTPUT);
  pinMode(PIN_BUTTON_LEFT_LED_BLUE, OUTPUT);
  pinMode(PIN_BUTTON_RIGHT_LED_RED, OUTPUT);
  pinMode(PIN_BUTTON_RIGHT_LED_GREEN, OUTPUT);
  pinMode(PIN_BUTTON_RIGHT_LED_BLUE, OUTPUT);
  pinMode(PIN_BUTTON_BTN, INPUT);

  //generating the colors for the left led and the right led
  leftLedColor = random(1, 5);
  rightLedColor = random(1, 5);
  btnWordGen = random(1, 4);

  lcdButton.init();
  lcdButton.backlight();
  
  if (DEBUG_LEVEL >= 1) {
    Serial.println(F("Left led color: "));
    Serial.println(leftLedColor);
    Serial.println(F("Right led color: "));
    Serial.println(rightLedColor);
    Serial.println(F("Word: "));
    Serial.println(btnWordGen);
  }
}

void setButtonStripColor()
{
  switch (rightLedColor) // right color is the light strip color for press and hold
    {
      case 1: {
          setColor(PIN_BUTTON_RIGHT_LED_RED, 0, PIN_BUTTON_RIGHT_LED_GREEN, 0, PIN_BUTTON_RIGHT_LED_BLUE, 255);
        }
        break;
      case 2: {
          setColor(PIN_BUTTON_RIGHT_LED_RED, 255, PIN_BUTTON_RIGHT_LED_GREEN, 0, PIN_BUTTON_RIGHT_LED_BLUE, 0);
        }
        break;
      case 3: {
          setColor(PIN_BUTTON_RIGHT_LED_RED, 255, PIN_BUTTON_RIGHT_LED_GREEN, 255, PIN_BUTTON_RIGHT_LED_BLUE, 0);
        }
        break;
      case 4: {
          setColor(PIN_BUTTON_RIGHT_LED_RED, 255, PIN_BUTTON_RIGHT_LED_GREEN, 255, PIN_BUTTON_RIGHT_LED_BLUE, 255);
        }
        break;
    }
}

void buttonCheckResult()
{
  if ((leftLedColor==1) && (btnWordGen==3)) { // if leftbtn is blue and button says abort hold button
    if (lengthOfPress==1) {
      addStrike();
    } else {
      if ((rightLedColor==1) && (checkClock(4))) {
      buttonModuleDefusedPrint();
      } else if ((rightLedColor==2) && (checkClock(1))) {
      buttonModuleDefusedPrint();
      } else if ((rightLedColor==3) && (checkClock(5))) {
      buttonModuleDefusedPrint();
      } else if ((rightLedColor==4) && (checkClock(1))) {
      buttonModuleDefusedPrint();
      } else {
       addStrike(); 
      }
    }
  } else if ((nrBatteries>=2) && (btnWordGen==2)) { // if more than 1 battery and button says detonate press and release
    if (lengthOfPress==1) {
      buttonModuleDefusedPrint();
    } else {
      addStrike();
    }
    
  } else if ((leftLedColor==4) && (indicatorCARLit==true)) { // if leftbtn is white and lit indicator CAR hold button
    if (lengthOfPress==1) {
      addStrike();
    } else {
      if ((rightLedColor==1) && (checkClock(4))) {
      buttonModuleDefusedPrint();
      } else if ((rightLedColor==2) && (checkClock(1))) {
      buttonModuleDefusedPrint();
      } else if ((rightLedColor==3) && (checkClock(5))) {
      buttonModuleDefusedPrint();
      } else if ((rightLedColor==4) && (checkClock(1))) {
      buttonModuleDefusedPrint();
      } else {
       addStrike(); 
      }
    }
    
  } else if ((nrBatteries>=3) && (indicatorFRKLit==true)) { // if more than 2 batteries and lit indicator FRK press and release
    if (lengthOfPress==1) {
      buttonModuleDefusedPrint();
    } else {
      addStrike();
    }
    
  } else if (leftLedColor==3) { // if leftbtn is yellow hold button
    if (lengthOfPress==1) {
      addStrike();
    } else {
      if ((rightLedColor==1) && (checkClock(4))) {
      buttonModuleDefusedPrint();
      } else if ((rightLedColor==2) && (checkClock(1))) {
      buttonModuleDefusedPrint();
      } else if ((rightLedColor==3) && (checkClock(5))) {
      buttonModuleDefusedPrint();
      } else if ((rightLedColor==4) && (checkClock(1))) {
      buttonModuleDefusedPrint();
      } else {
       addStrike(); 
      }
    }
    
  } else if ((leftLedColor==2) && (btnWordGen==1)) { // if leftbtn is red and button says hold press and release
    if (lengthOfPress==1) {
      buttonModuleDefusedPrint();
    } else {
      addStrike();
    }
    
  } else { // else hold button
    if (lengthOfPress==1) {
      addStrike();
    } else {
      if ((rightLedColor==1) && (checkClock(4))) {
      buttonModuleDefusedPrint();
      } else if ((rightLedColor==2) && (checkClock(1))) {
      buttonModuleDefusedPrint();
      } else if ((rightLedColor==3) && (checkClock(5))) {
      buttonModuleDefusedPrint();
      } else if ((rightLedColor==4) && (checkClock(1))) {
      buttonModuleDefusedPrint();
      } else {
       addStrike(); 
      }
    }
    
  }
  
}

void buttonLoop()
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  if (buttonColorSet == false) {

    printWord();
    lcdButton.setCursor(0, 1);
    lcdButton.print(F("Serial: "));
    lcdButton.print(serialCode);

    switch (leftLedColor) // left color is the button color
    {
      case 1: {
          setColor(PIN_BUTTON_LEFT_LED_RED, 0, PIN_BUTTON_LEFT_LED_GREEN, 0, PIN_BUTTON_LEFT_LED_BLUE, 255);
        }
        break;
      case 2: {
          setColor(PIN_BUTTON_LEFT_LED_RED, 255, PIN_BUTTON_LEFT_LED_GREEN, 0, PIN_BUTTON_LEFT_LED_BLUE, 0);
        }
        break;
      case 3: {
          setColor(PIN_BUTTON_LEFT_LED_RED, 255, PIN_BUTTON_LEFT_LED_GREEN, 255, PIN_BUTTON_LEFT_LED_BLUE, 0);
        }
        break;
      case 4: {
          setColor(PIN_BUTTON_LEFT_LED_RED, 255, PIN_BUTTON_LEFT_LED_GREEN, 255, PIN_BUTTON_LEFT_LED_BLUE, 255);
        }
        break;
    }
    buttonColorSet = true;
  }
  
    buttonBtnState = digitalRead(PIN_BUTTON_BTN);

    // compare the buttonBtnState to its previous state
    if (buttonBtnState != lastBtnState) {
      // if the state has changed, increment the counter
      if (buttonBtnState == HIGH) {
        // if the current state is HIGH then the button went from off to on:
        if (DEBUG_LEVEL >= 1) {
          Serial.println(F("buttonBtnOn"));
        }
        lastDebounceTime = millis();
      } else {
        // if the current state is LOW then the button went from on to off:
        if (DEBUG_LEVEL >= 1) {
          Serial.println(F("buttonBtnOff"));
        }
        if (millis()-lastDebounceTime<DEBOUNCE_DELAY) {
        lengthOfPress=1; // 1=short, 2=long
        buttonCheckResult();
        } else {
        lengthOfPress=2; // 1=short, 2=long
        buttonCheckResult();
        }
        setColor(PIN_BUTTON_RIGHT_LED_RED, 0, PIN_BUTTON_RIGHT_LED_GREEN, 0, PIN_BUTTON_RIGHT_LED_BLUE, 0);
      }
    }
    // save the current state as the last state, for next time through the loop
    lastBtnState = buttonBtnState;
    if ((buttonBtnState == HIGH) && (millis()-lastDebounceTime>DEBOUNCE_DELAY)) {
      setButtonStripColor(); // if longer than delay light up strip
    }
  
}
