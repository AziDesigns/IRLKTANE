// On the Subject of Buttons
/*
  I know there is a lot of repetitive code inside this.
  I will rewrite after getting other modules updated and working.

  KNOWN ISSUES:
  NEED TO PREVENT LIGHTS FROM COMING ON BEFORE ALL MODULES START AND TIMER STARTS
  NEED TO PREVENT "RIGHT LED" FROM SHOWING A COLOR BEFORE THE BUTTON IS DEPRESSED
*/

#define LCD_BUTTON_CONTRAST 40
#define PIN_BUTTON_LED_GREEN 22 // module complete LED
#define BUTTON_BTN_PIN 23
#define LEFT_LED_RED_PIN 13
#define LEFT_LED_GREEN_PIN 7
#define LEFT_LED_BLUE_PIN 11
#define RIGHT_LED_RED_PIN 4
#define RIGHT_LED_GREEN_PIN 5
#define RIGHT_LED_BLUE_PIN 6
#define V0_PIN 13

LiquidCrystal lcdButton(39, 38, 40, 41, 42, 43);

int btnState = LOW;
int lastBtnState = LOW;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelayButton = 50;
int moduleFinished = 0;
int leftLedColor;
int rightLedColor;
int btnWordGen;
bool buttonColorSet = false;

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
  pinMode(V0_PIN, OUTPUT);

  analogWrite(V0_PIN, LCD_BUTTON_CONTRAST);
  lcdButton.begin(16, 2);

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
    buttonColorSet = true;
  }
  if (!buttonModuleDefused) {
    int reading = digitalRead(BUTTON_BTN_PIN);

    if (reading != lastBtnState)
      lastDebounceTime = millis();

    switch (leftLedColor)
    {
      case 1: {// left led is blue
          if (btnWordGen == 3 && seconds > 0 && !moduleFinished) { //Abort written on the screen
            // press and imediately release
            if (millis() - lastDebounceTime > debounceDelayButton) {
              if (reading != btnState) {
                btnState = reading;
                if (btnState == HIGH) {
                  moduleFinished = 1;
                  buttonModuleDefusedPrint();
                  if (DEBUG_LEVEL >= 1) {
                    Serial.println("Finished: Left led is blue + Abort");
                  }
                }
              }
            }
          }
          else if (seconds > 0 && !moduleFinished) //hold button
            switch (rightLedColor)
            {
              case 1:
                { // right led is blue
                  // release when timer has a 4 in any position
                  if (millis() - lastDebounceTime > debounceDelayButton) {
                    if (reading != btnState) {
                      btnState = reading;
                      if (btnState == LOW && checkClock(4)) {
                        moduleFinished = 1;
                        buttonModuleDefusedPrint();
                        if (DEBUG_LEVEL >= 1) {
                          Serial.println("Finished: Left led is blue + Right led is blue, button has been held");
                        }
                      }
                      else if (btnState == LOW && !checkClock(4)) {
                        addStrike();
                      }
                    }
                  }
                }
                break;
              case 2:
                { // right led is red
                  // release when timer has a 3 in any position
                  if (millis() - lastDebounceTime > debounceDelayButton) {
                    if (reading != btnState) {
                      btnState = reading;
                      if (btnState == LOW && checkClock(3)) {
                        moduleFinished = 1;
                        buttonModuleDefusedPrint();
                        if (DEBUG_LEVEL >= 1) {
                          Serial.println("Finished: Left led is blue + Right led is red, button has been held");
                        }
                      }
                      else if (btnState == LOW && !checkClock(3)) {
                        addStrike();
                      }
                    }
                  }
                }
                break;
              case 3:
                { // Right led is green
                  // release when timer has a 5 in any position
                  if (millis() - lastDebounceTime > debounceDelayButton) {
                    if (reading != btnState) {
                      btnState = reading;
                      if (btnState == LOW && checkClock(5)) {
                        moduleFinished = 1;
                        buttonModuleDefusedPrint();
                        if (DEBUG_LEVEL >= 1) {
                          Serial.println("Finished: Left led is blue + Right led is green, button has been held");
                        }
                      }
                      else if (btnState == LOW && !checkClock(5)) {
                        addStrike();
                      }
                    }
                  }
                }
                break;
              case 4:
                { // Right led is white
                  // release when timer has a 1 in any position
                  if (millis() - lastDebounceTime > debounceDelayButton) {
                    if (reading != btnState) {
                      btnState = reading;
                      if (btnState == LOW && checkClock(1)) {
                        moduleFinished = 1;
                        buttonModuleDefusedPrint();
                        if (DEBUG_LEVEL >= 1) {
                          Serial.println("Finished: Left led is blue + Right led is white, button has been held");
                        }
                      }
                      else if (btnState == LOW && !checkClock(1)) {
                        addStrike();
                      }
                    }
                  }
                }
                break;
            }
        }
        break;
      case 2:
        { // Left led is red
          if (btnWordGen == 1 && seconds > 0 && !moduleFinished) { // Hold is written on the screen 
            // press and imediately release
            if (millis() - lastDebounceTime > debounceDelayButton) {
              if (reading != btnState) {
                btnState = reading;
                if (btnState == HIGH) {
                  moduleFinished = 1;
                  buttonModuleDefusedPrint();
                  if (DEBUG_LEVEL >= 1) {
                    Serial.println("Finished: Left led is red + Hold");
                  }
                }
              }
            }
          }
          else if (seconds > 0 && !moduleFinished) //hold button
            switch (rightLedColor)
            {
              case 1:
                { // Right led is blue
                  // release when timer has a 4 in any position
                  if (millis() - lastDebounceTime > debounceDelayButton) {
                    if (reading != btnState) {
                      btnState = reading;
                      if (btnState == LOW && checkClock(4)) {
                        moduleFinished = 1;
                        buttonModuleDefusedPrint();
                        if (DEBUG_LEVEL >= 1) {
                          Serial.println("Finished: Left led is red + Right led is blue, button has been held");
                        }
                      }
                      else if (btnState == LOW && !checkClock(4)) {
                        addStrike();
                      }
                    }
                  }
                }
                break;
              case 2:
                { // Right led is red
                  // release when timer has a 3 in any position
                  if (millis() - lastDebounceTime > debounceDelayButton) {
                    if (reading != btnState) {
                      btnState = reading;
                      if (btnState == LOW && checkClock(3)) {
                        moduleFinished = 1;
                        buttonModuleDefusedPrint();
                        if (DEBUG_LEVEL >= 1) {
                          Serial.println("Finished: Left led is red + Right led is red, button has been held");
                        }
                      }
                      else if (btnState == LOW && !checkClock(3)) {
                        addStrike();
                      }
                    }
                  }
                }
                break;
              case 3:
                { // Right led is green
                  // release when timer has a 5 in any position
                  if (millis() - lastDebounceTime > debounceDelayButton) {
                    if (reading != btnState) {
                      btnState = reading;
                      if (btnState == LOW && checkClock(5)) {
                        moduleFinished = 1;
                        buttonModuleDefusedPrint();
                        if (DEBUG_LEVEL >= 1) {
                          Serial.println("Finished: Left led is red + Right led is green, button has been held");
                        }
                      }
                      else if (btnState == LOW && !checkClock(5)) {
                        addStrike();
                      }
                    }
                  }
                }
                break;
              case 4:
                { // Right Led is white
                  //release when timer has a 1 in any position
                  if (millis() - lastDebounceTime > debounceDelayButton) {
                    if (reading != btnState) {
                      btnState = reading;
                      if (btnState == LOW && checkClock(1)) {
                        moduleFinished = 1;
                        buttonModuleDefusedPrint();
                        if (DEBUG_LEVEL >= 1) {
                          Serial.println("Finished: Left Led is red + Right led is white, button has been held");
                        }
                      }
                      else if (btnState == LOW && !checkClock(1)) {
                        addStrike();
                      }
                    }
                  }
                }
                break;
            }
        }
        break;
      case 3:
        { // Left led is yellow
          if (btnWordGen == 2 && seconds > 0 && !moduleFinished) { // Detonate written on the screen 
            // press and imediately release
            if (millis() - lastDebounceTime > debounceDelayButton) {
              if (reading != btnState) {
                btnState = reading;
                if (btnState == HIGH) {
                  moduleFinished = 1;
                  buttonModuleDefusedPrint();
                  if (DEBUG_LEVEL >= 1) {
                    Serial.println("Finished: Left led is yellow + Detonate");
                  }
                }
              }
            }
          }
          else if (seconds > 0 && !moduleFinished) //hold button
            switch (rightLedColor)
            {
              case 1:
                { // Right led is blue
                  // release when timer has a 4 in any position
                  if (millis() - lastDebounceTime > debounceDelayButton) {
                    if (reading != btnState) {
                      btnState = reading;
                      if (btnState == LOW && checkClock(4)) {
                        moduleFinished = 1;
                        buttonModuleDefusedPrint();
                        if (DEBUG_LEVEL >= 1) {
                          Serial.println("Finished: Left led is yellow + Right led is blue, button has been held");
                        }
                      }
                      else if (btnState == LOW && !checkClock(4)) {
                        addStrike();
                      }
                    }
                  }
                }
                break;
              case 2:
                { // Right led is red
                  // release when timer has a 3 in any position
                  if (millis() - lastDebounceTime > debounceDelayButton) {
                    if (reading != btnState) {
                      btnState = reading;
                      if (btnState == LOW && checkClock(3)) {
                        moduleFinished = 1;
                        buttonModuleDefusedPrint();
                        if (DEBUG_LEVEL >= 1) {
                          Serial.println("Finished: Left led is yellow + Right led is red, button has been held");
                        }
                      }
                      else if (btnState == LOW && !checkClock(3)) {
                        addStrike();
                      }
                    }
                  }
                }
                break;
              case 3:
                { // Right led is green
                  // release when timer has a 5 in any position
                  if (millis() - lastDebounceTime > debounceDelayButton) {
                    if (reading != btnState) {
                      btnState = reading;
                      if (btnState == LOW && checkClock(5)) {
                        moduleFinished = 1;
                        buttonModuleDefusedPrint();
                        if (DEBUG_LEVEL >= 1) {
                          Serial.println("Finished: Left led is yellow + Right led is green, button has been held");
                        }
                      }
                      else if (btnState == LOW && !checkClock(5)) {
                        addStrike();
                      }
                    }
                  }
                }
                break;
              case 4:
                { // Right led is white
                  // release when timer has a 1 in any position
                  if (millis() - lastDebounceTime > debounceDelayButton) {
                    if (reading != btnState) {
                      btnState = reading;
                      if (btnState == LOW && checkClock(1)) {
                        moduleFinished = 1;
                        buttonModuleDefusedPrint();
                        if (DEBUG_LEVEL >= 1) {
                          Serial.println("Finished: Left led is yellow + Right led is white, button has been held");
                        }
                      }
                      else if (btnState == LOW && !checkClock(1)) {
                        addStrike();
                      }
                    }
                  }
                }
                break;
            }
        }
        break;
      case 4:
        { // Left led is white
          if (seconds > 0 && !moduleFinished)
            switch (rightLedColor)
            {
              case 1:
                { // Right led is blue
                  // release when timer has a 4 in any position
                  if (millis() - lastDebounceTime > debounceDelayButton) {
                    if (reading != btnState) {
                      btnState = reading;
                      if (btnState == LOW && checkClock(4)) {
                        moduleFinished = 1;
                        buttonModuleDefusedPrint();
                        if (DEBUG_LEVEL >= 1) {
                          Serial.println("Finished: Left led is white + Right led is blue, button has been held");
                        }
                      }
                      else if (btnState == LOW && !checkClock(4)) {
                        addStrike();
                      }
                    }
                  }
                }
                break;
              case 2:
                { // Right led is red
                  // release when timer has a 3 in any position
                  if (millis() - lastDebounceTime > debounceDelayButton) {
                    if (reading != btnState) {
                      btnState = reading;
                      if (btnState == LOW && checkClock(3)) {
                        moduleFinished = 1;
                        buttonModuleDefusedPrint();
                        if (DEBUG_LEVEL >= 1) {
                          Serial.println("Finished: Left led is white + Right led is red, button has been held");
                        }
                      }
                      else if (btnState == LOW && !checkClock(3)) {
                        addStrike();
                      }
                    }
                  }
                }
                break;
              case 3:
                { // Right led is green
                  // release when timer has a 5 in any position
                  if (millis() - lastDebounceTime > debounceDelayButton) {
                    if (reading != btnState) {
                      btnState = reading;
                      if (btnState == LOW && checkClock(5)) {
                        moduleFinished = 1;
                        buttonModuleDefusedPrint();
                        if (DEBUG_LEVEL >= 1) {
                          Serial.println("Finished: Left led is white + Right led is green, button has been held");
                        }
                      }
                      else if (btnState == LOW && !checkClock(5)) {
                        addStrike();
                      }
                    }
                  }
                }
                break;
              case 4:
                { // Right led is white
                  // release when timer has a 1 in any position
                  if (millis() - lastDebounceTime > debounceDelayButton) {
                    if (reading != btnState) {
                      btnState = reading;
                      if (btnState == LOW && checkClock(1)) {
                        moduleFinished = 1;
                        buttonModuleDefusedPrint();
                        if (DEBUG_LEVEL >= 1) {
                          Serial.println("Finished: Left led is white + Right led is white, button has been held");
                        }
                      }
                      else if (btnState == LOW && !checkClock(1)) {
                        addStrike();
                      }
                    }
                  }
                }
                break;
            }
        }
        break;
    }

    lastBtnState = reading;
  }
}
