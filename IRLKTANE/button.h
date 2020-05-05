// On the Subject of Buttons
/*
  KNOWN ISSUES:
*/

#define PIN_BUTTON_LED_GREEN 22 // module complete LED
#define BUTTON_BTN_PIN 23
#define LEFT_LED_RED_PIN 13
#define LEFT_LED_GREEN_PIN 7
#define LEFT_LED_BLUE_PIN 11
#define RIGHT_LED_RED_PIN 6
#define RIGHT_LED_GREEN_PIN 5
#define RIGHT_LED_BLUE_PIN 4

LiquidCrystal lcdButton(39, 38, 40, 41, 42, 43);

int buttonBtnState = LOW;
int lastBtnState = LOW;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelayButton = 50;
int lengthOfPress = 0;
int leftLedColor;
int rightLedColor;
int btnWordGen;
bool buttonColorSet = false;
bool buttonStripColorSet = false;

void setColor (int redPin, int redValue, int greenPin, int greenValue, int bluePin, int blueValue)
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
  setColor(LEFT_LED_RED_PIN, 0, LEFT_LED_GREEN_PIN, 0, LEFT_LED_BLUE_PIN, 0);
  setColor(RIGHT_LED_RED_PIN, 0, RIGHT_LED_GREEN_PIN, 0, RIGHT_LED_BLUE_PIN, 0);
  buttonModuleDefused = true;
  defusedModuleBuzzer();
  digitalWrite(PIN_BUTTON_LED_GREEN, HIGH);
  lcdButton.clear();
  lcdButton.setCursor(1, 0);
  lcdButton.print("MODULE DEFUSED");
  lcdButton.setCursor(0, 1);
  lcdButton.print("Serial: ");
  lcdButton.print(serialCode);
}

void buttonModuleBoom()
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  setColor(LEFT_LED_RED_PIN, 0, LEFT_LED_GREEN_PIN, 0, LEFT_LED_BLUE_PIN, 0);
  setColor(RIGHT_LED_RED_PIN, 0, RIGHT_LED_GREEN_PIN, 0, RIGHT_LED_BLUE_PIN, 0);
  lcdButton.clear();
  lcdButton.setCursor(6, 0);
  lcdButton.print("BOMB");
  lcdButton.setCursor(4, 1);
  lcdButton.print("EXPLODED");
  digitalWrite(PIN_BUTTON_LED_GREEN, LOW);
}

bool checkClock(int value)
{
  int timer = sec;
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
        lcdButton.print("HOLD");
      }
      break;
    case 2:
      { //DETONATE
        lcdButton.setCursor(4, 0);
        lcdButton.print("DETONATE");
      }
      break;
    case 3:
      { //ABORT
        lcdButton.setCursor(5, 0);
        lcdButton.print("ABORT");
      }
      break;
  }
}

void buttonSetup()
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  pinMode(LEFT_LED_RED_PIN, OUTPUT);
  pinMode(LEFT_LED_GREEN_PIN, OUTPUT);
  pinMode(LEFT_LED_BLUE_PIN, OUTPUT);
  pinMode(RIGHT_LED_RED_PIN, OUTPUT);
  pinMode(RIGHT_LED_GREEN_PIN, OUTPUT);
  pinMode(RIGHT_LED_BLUE_PIN, OUTPUT);
  pinMode(BUTTON_BTN_PIN, INPUT);
  pinMode(PIN_BUTTON_LED_GREEN, OUTPUT);

  //generating a seed to use in order to generate random numbers
  randomSeed(analogRead(0));

  //generating the colors for the left led and the right led
  leftLedColor = random(1, 5);
  rightLedColor = random(1, 5);
  btnWordGen = random(1, 4);

  if (DEBUG_LEVEL >= 1) {
    Serial.println("Left led color: ");
    Serial.println(leftLedColor);
    Serial.println("Right led color: ");
    Serial.println(rightLedColor);
    Serial.println("Word: ");
    Serial.println(btnWordGen);
  }
}

void setButtonStripColor()
{
  switch (rightLedColor) // right color is the light strip color for press and hold
    {
      case 1: {
          setColor(RIGHT_LED_RED_PIN, 0, RIGHT_LED_GREEN_PIN, 0, RIGHT_LED_BLUE_PIN, 255);
        }
        break;
      case 2: {
          setColor(RIGHT_LED_RED_PIN, 255, RIGHT_LED_GREEN_PIN, 0, RIGHT_LED_BLUE_PIN, 0);
        }
        break;
      case 3: {
          setColor(RIGHT_LED_RED_PIN, 255, RIGHT_LED_GREEN_PIN, 255, RIGHT_LED_BLUE_PIN, 0);
        }
        break;
      case 4: {
          setColor(RIGHT_LED_RED_PIN, 255, RIGHT_LED_GREEN_PIN, 255, RIGHT_LED_BLUE_PIN, 255);
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
    lcdButton.print("Serial: ");
    lcdButton.print(serialCode);

    switch (leftLedColor) // left color is the button color
    {
      case 1: {
          setColor(LEFT_LED_RED_PIN, 0, LEFT_LED_GREEN_PIN, 0, LEFT_LED_BLUE_PIN, 255);
        }
        break;
      case 2: {
          setColor(LEFT_LED_RED_PIN, 255, LEFT_LED_GREEN_PIN, 0, LEFT_LED_BLUE_PIN, 0);
        }
        break;
      case 3: {
          setColor(LEFT_LED_RED_PIN, 255, LEFT_LED_GREEN_PIN, 255, LEFT_LED_BLUE_PIN, 0);
        }
        break;
      case 4: {
          setColor(LEFT_LED_RED_PIN, 255, LEFT_LED_GREEN_PIN, 255, LEFT_LED_BLUE_PIN, 255);
        }
        break;
    }
    buttonColorSet = true;
  }
  
    buttonBtnState = digitalRead(BUTTON_BTN_PIN);

    // compare the buttonBtnState to its previous state
    if (buttonBtnState != lastBtnState) {
      // if the state has changed, increment the counter
      if (buttonBtnState == HIGH) {
        // if the current state is HIGH then the button went from off to on:
        if (DEBUG_LEVEL >= 1) {
          Serial.println("buttonBtnOn");
        }
        lastDebounceTime = millis();
      } else {
        // if the current state is LOW then the button went from on to off:
        if (DEBUG_LEVEL >= 1) {
          Serial.println("buttonBtnOff");
        }
        if (millis()-lastDebounceTime<debounceDelayButton) {
        lengthOfPress=1; // 1=short, 2=long
        buttonCheckResult();
        } else {
        lengthOfPress=2; // 1=short, 2=long
        buttonCheckResult();
        }
        setColor(RIGHT_LED_RED_PIN, 0, RIGHT_LED_GREEN_PIN, 0, RIGHT_LED_BLUE_PIN, 0);
      }
    }
    // save the current state as the last state, for next time through the loop
    lastBtnState = buttonBtnState;
    if ((buttonBtnState == HIGH) && (millis()-lastDebounceTime>debounceDelayButton)) {
      setButtonStripColor(); // if longer than delay light up strip
    }
  
}
