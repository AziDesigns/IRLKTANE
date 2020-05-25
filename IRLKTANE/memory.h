// On the Subject of Memory
/*
  KNOWN ISSUES:
  MODULE IS MOSTLY DONE BUT HAS NOT BEEN TESTED AT ALL.
  NEEDS COMPARED TO ACTUAL GAME MANUAL FOR ACCURACY.
  LedControl needs to be mapped to the correct pins once able to physically test.
*/
#define PIN_MEMORY_BUTTON_1 22 // position 1 button
#define PIN_MEMORY_BUTTON_2 23 // position 2 button
#define PIN_MEMORY_BUTTON_3 29 // position 3 button
#define PIN_MEMORY_BUTTON_4 44 // position 4 button

#define PIN_MEMORY_LED_1 2,6,2 // stage 1 complete LED
#define PIN_MEMORY_LED_2 2,7,2 // stage 2 complete LED
#define PIN_MEMORY_LED_3 2,0,3 // stage 3 complete LED
#define PIN_MEMORY_LED_4 2,1,3 // stage 4 complete LED
#define PIN_MEMORY_LED_FIN 2,2,3 // module complete LED

byte previousButton, button; //the previos and the current button
byte rightPoz; //the answer to the current stage
byte val[6], pos[6]; // the right values and positions of every stage
byte stageNumber = 1;
byte digits[5]; // the values of the current stage

bool printAnswer = true;

void turnOffLeds()
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  lc.setLed(PIN_MEMORY_LED_1,false);
  lc.setLed(PIN_MEMORY_LED_2,false);
  lc.setLed(PIN_MEMORY_LED_3,false);
  lc.setLed(PIN_MEMORY_LED_4,false);
  lc.setLed(PIN_MEMORY_LED_FIN,false);
}

// function that generate a random digit from 1 to 4
byte randomNumber()
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  return (random(20, 100)) / 20;
}

// generates the digits of the current stage
void generateNumbers()
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  digits[0] = randomNumber();
  digits[1] = randomNumber();
  digits[2] = randomNumber();
  digits[3] = randomNumber();

  // if the digits repeat:
  while (digits[2] == digits[1]) digits[2] = randomNumber();
  while (digits[3] == digits[1] || digits[3] == digits[2]) digits[3] = randomNumber();

  digits[4] = 10 - digits[3] - digits[2] - digits[1]; // the last digit is the last number from 1-4
}

// display the current digits
void displayNumbers()
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
//  memorylc.setDigit(0, 5, digits[0], false);
//  memorylc.setDigit(0, 0, digits[1], false);
//  memorylc.setDigit(0, 1, digits[2], false);
//  memorylc.setDigit(0, 2, digits[3], false);
//  memorylc.setDigit(0, 3, digits[4], false);
}

// function that returns the postion of a digit
byte positionOf(byte nr)
{
  for (byte i = 1; i < 5; i++)
    if (digits[i] == nr) return i;
}

// function that calculates the answer for the current stage
void stage(byte nr)
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  if (nr == 1) {
    if (digits[0] == 1) rightPoz = 2;
    else if (digits[0] == 2) rightPoz = 2;
    else if (digits[0] == 3) rightPoz = 3;
    else if (digits[0] == 4) rightPoz = 4;
  } else if (nr == 2) {
    if (digits[0] == 1) rightPoz = positionOf(4);
    else if (digits[0] == 2) rightPoz = pos[1];
    else if (digits[0] == 3) rightPoz = 1;
    else if (digits[0] == 4) rightPoz = pos[1];
  } else if (nr == 3) {
    if (digits[0] == 1) rightPoz = positionOf(val[2]);
    else if (digits[0] == 2) rightPoz = positionOf(val[1]);
    else if (digits[0] == 3) rightPoz = 3;
    else if (digits[0] == 4) rightPoz = positionOf(4);
  } else if (nr == 4) {
    if (digits[0] == 1) rightPoz = pos[1];
    else if (digits[0] == 2) rightPoz = 1;
    else if (digits[0] == 3) rightPoz = pos[2];
    else if (digits[0] == 4) rightPoz = pos[2];
  } else if (nr == 5) {
    if (digits[0] == 1) rightPoz = positionOf(val[1]);
    else if (digits[0] == 2) rightPoz = positionOf(val[2]);
    else if (digits[0] == 3) rightPoz = positionOf(val[4]); //3
    else if (digits[0] == 4) rightPoz = positionOf(val[3]); //4
  }

  pos[nr] = rightPoz;
  val[nr] = digits[rightPoz];
}

void memorySetup()
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
//  memorylc.shutdown(0, false); // turn off power saving, enables display

//  memorylc.setIntensity(0, 4); // sets brightness (0~15 possible values)
//  memorylc.clearDisplay(0);// clear screen

  turnOffLeds();

  // stage 1
  generateNumbers();
  displayNumbers();
  stageNumber = 1;
}

void changeStage() // the next stage
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  if (stageNumber == 1) lc.setLed(PIN_MEMORY_LED_1,true);
  else if (stageNumber == 2) lc.setLed(PIN_MEMORY_LED_2,true);
  else if (stageNumber == 3) lc.setLed(PIN_MEMORY_LED_3,true);
  else if (stageNumber == 4) lc.setLed(PIN_MEMORY_LED_4,true);

  generateNumbers();
  stageNumber++;
  displayNumbers();
  printAnswer = true;
}

void memoryReset() // function that resets the module
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  addStrike(); 
  if (nrStrikes < 3) {
    stageNumber = 0;
    changeStage();
    turnOffLeds();
  }
}

byte memoryPressedButton() // function that returns the pressed button
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  previousButton = button;
  if (digitalRead(PIN_MEMORY_BUTTON_1) == 1) return 1;
  if (digitalRead(PIN_MEMORY_BUTTON_2) == 1) return 2;
  if (digitalRead(PIN_MEMORY_BUTTON_3) == 1) return 3;
  if (digitalRead(PIN_MEMORY_BUTTON_4) == 1) return 4;
  return 0;
}

void memoryCheckButton() // function that checks if a button is pressed
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  button = memoryPressedButton();
  stage(stageNumber);
  if (printAnswer) { // print the answer to the current stage 
    if (DEBUG_LEVEL >= 1) {
      Serial.print(F("Memory Stage "));
      Serial.print(stageNumber);
      Serial.print(F(" : "));
      Serial.println(rightPoz);
    }
    printAnswer = false;
  }

  if (button != 0 && previousButton != button) {
    if (button == rightPoz) { // if the right button was pressed 
      if (stageNumber == 5) { // if it's the last stage, the module is defused 
        turnOffLeds();
        lc.setLed(PIN_MEMORY_LED_FIN,true);
//        memorylc.clearDisplay(0);
        memoryModuleDefused = true;
        defusedModuleBuzzer();
        isAnyModuleDefused=true;
        stageNumber++;
      } else
        changeStage(); // else go to the next stage
    } else
      memoryReset(); // else reset the module
  }
}

void memoryModuleBoom()  // if the bomb explodes what should the module display
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  lc.setLed(PIN_MEMORY_LED_1,false);
  lc.setLed(PIN_MEMORY_LED_2,false);
  lc.setLed(PIN_MEMORY_LED_3,false);
  lc.setLed(PIN_MEMORY_LED_4,false);
  lc.setLed(PIN_MEMORY_LED_FIN,false);
//  memorylc.clearDisplay(0);
}

void memoryLoop() // if module not defused keep looping
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  if (!memoryModuleDefused)
    memoryCheckButton();

}
