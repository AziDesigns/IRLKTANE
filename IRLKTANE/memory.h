// On the Subject of Memory
/*
  KNOWN ISSUES:
  MODULE IS MOSTLY DONE BUT HAS NOT BEEN TESTED AT ALL.
  NEEDS COMPARED TO ACTUAL GAME MANUAL FOR ACCURACY.
  LedControl needs to be mapped to the correct pins once able to physically test.
*/
#define PIN_MEMORY_BUTTON_1 A6 // position 1 button
#define PIN_MEMORY_BUTTON_2 A7 // position 2 button
#define PIN_MEMORY_BUTTON_3 A8 // position 3 button
#define PIN_MEMORY_BUTTON_4 A9 // position 4 button

#define PIN_MEMORY_LED_1 31 // stage 1 complete LED
#define PIN_MEMORY_LED_2 33 // stage 2 complete LED
#define PIN_MEMORY_LED_3 35 // stage 3 complete LED
#define PIN_MEMORY_LED_4 37 // stage 4 complete LED
// I think there should be a stage 5 complete LED even though that means the module is defused.
// Unless its going to just be always off? Need to see how KTANE handles that module
#define PIN_MEMORY_LED_GREEN 32 // module complete LED

LedControl lc = LedControl(x, x, x, 2); //DIN, CLK, LOAD, No. DRIVER

int previousButton, button; //the previos and the current button
int rightPoz; //the answer to the current stage
int val[6], pos[6]; // the right values and positions of every stage
int stageNumber = 1;
int digits[5]; // the values of the current stage

bool printAnswer = true;

void turnOffLeds()
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  digitalWrite(PIN_MEMORY_LED_1, LOW);
  digitalWrite(PIN_MEMORY_LED_2, LOW);
  digitalWrite(PIN_MEMORY_LED_3, LOW);
  digitalWrite(PIN_MEMORY_LED_4, LOW);
  digitalWrite(PIN_MEMORY_LED_GREEN, LOW);
}

// function that generate a random digit from 1 to 4
int randomNumber()
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
  lc.setDigit(0, 5, digits[0], false);
  lc.setDigit(0, 0, digits[1], false);
  lc.setDigit(0, 1, digits[2], false);
  lc.setDigit(0, 2, digits[3], false);
  lc.setDigit(0, 3, digits[4], false);
}

// function that returns the postion of a digit
int positionOf(int nr)
{
  for (int i = 1; i < 5; i++)
    if (digits[i] == nr) return i;
}

// function that calculates the answer for the current stage
void stage(int nr)
{
  if (DEBUG_LEVEL >= 2) {
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
  lc.shutdown(0, false); // turn off power saving, enables display

  lc.setIntensity(0, 4); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen

  randomSeed(analogRead(0));

  pinMode(PIN_MEMORY_LED_1, OUTPUT);
  pinMode(PIN_MEMORY_LED_2, OUTPUT);
  pinMode(PIN_MEMORY_LED_3, OUTPUT);
  pinMode(PIN_MEMORY_LED_4, OUTPUT);
  pinMode(PIN_MEMORY_LED_GREEN, OUTPUT);

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
  if (stageNumber == 1) digitalWrite(PIN_MEMORY_LED_1, HIGH);
  else if (stageNumber == 2) digitalWrite(PIN_MEMORY_LED_2, HIGH);
  else if (stageNumber == 3) digitalWrite(PIN_MEMORY_LED_3, HIGH);
  else if (stageNumber == 4) digitalWrite(PIN_MEMORY_LED_4, HIGH);

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

int memoryPressedButton() // function that returns the pressed button
{
  if (DEBUG_LEVEL >= 2) {
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
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  button = memoryPressedButton();
  stage(stageNumber);
  if (printAnswer) { // print the answer to the current stage 
    if (DEBUG_LEVEL >= 1) {
      Serial.print("Memory Stage ");
      Serial.print(stageNumber);
      Serial.print(" : ");
      Serial.println(rightPoz);
    }
    printAnswer = false;
  }

  if (button != 0 && previousButton != button) {
    if (button == rightPoz) { // if the right button was pressed 
      if (stageNumber == 5) { // if it's the last stage, the module is defused 
        turnOffLeds();
        digitalWrite(PIN_MEMORY_LED_GREEN, HIGH);
        lc.clearDisplay(0);
        memoryModuleDefused = true;
        defusedModuleBuzzer();
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
  digitalWrite(PIN_MEMORY_LED_1, LOW);
  digitalWrite(PIN_MEMORY_LED_2, LOW);
  digitalWrite(PIN_MEMORY_LED_3, LOW);
  digitalWrite(PIN_MEMORY_LED_4, LOW);
  digitalWrite(PIN_MEMORY_LED_GREEN, LOW);
  lc.clearDisplay(0);
}

void memoryLoop() // if module not defused keep looping
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  if (!memoryModuleDefused)
    memoryCheckButton();

}
