// On the Subject of Who's on First
/*
  KNOWN ISSUES:
*/
#define LABELS_LENGTH 28

#define PIN_WHO_LED_1 0,7,3 // stage 1 complete LED
#define PIN_WHO_LED_2 0,0,4 // stage 2 complete LED
#define PIN_WHO_LED_3 0,1,4 // stage 3 complete LED
#define PIN_WHO_LED_FIN 0,2,4 // module complete LED

#define PIN_WHO_BUTTON_1 A15 // Select button for Column 1 Row 1
#define PIN_WHO_BUTTON_2 A14 // Select button for Column 1 Row 2
#define PIN_WHO_BUTTON_3 A13 // Select button for Column 1 Row 3
#define PIN_WHO_BUTTON_4 A12 // Select button for Column 2 Row 1
#define PIN_WHO_BUTTON_5 A11 // Select button for Column 2 Row 2
#define PIN_WHO_BUTTON_6 A10 // Select button for Column 2 Row 3

LiquidCrystal_I2C lcdWho(0x27, 20, 4);

char labels[LABELS_LENGTH][10] = { "YES", "FIRST", "DISPLAY", "OKAY", "SAYS", "NOTHING", "", "BLANK", "NO", "LED", "LEAD", "READ", "RED", "REED", "LEED", "HOLD ON", "YOU", "YOU ARE", "YOUR", "YOU'RE", "UR", "THERE", "THEY'RE", "THEIR", "THEY ARE", "SEE", "C", "CEE" };
char allLabels[LABELS_LENGTH][10] = { "YES", "BLANK", "NOTHING", "DONE", "LIKE", "FIRST", "NEXT", "HOLD", "UHHH", "UH UH", "UH HUH", "LEFT", "MIDDLE", "RIGHT", "OKAY", "PRESS", "NO", "READY", "SURE", "WAIT", "U", "UR", "YOU", "YOUR", "YOU'RE", "YOU ARE", "WHAT", "WHAT?"  };

int indexOfButtonLabels[LABELS_LENGTH][15] = {{ 14, 13, 8, 12, 5, 26, 15, 17, 2, 0}, // the words for the first label, 0 is the first label from allLabels[]
  { 19, 13, 14, 12, 1}, // the words for the second label
  { 8, 13, 14, 12, 0, 1, 16, 15, 11, 26, 19, 5, 2},
  { 18, 10, 6, 27, 23, 21, 24, 7, 4, 22, 20, 25, 9, 3},
  { 24, 6, 20, 21, 7, 3, 9, 27, 10, 22, 4},
  { 11, 14, 0, 12, 16, 13, 2, 8, 19, 17, 1, 26, 15, 5},
  { 27, 10, 9, 23, 7, 18, 6},
  { 25, 20, 3, 9, 22, 21, 18, 27, 24, 6, 7},
  { 17, 2, 11, 26, 14, 0, 13, 16, 15, 1, 8},
  { 21, 20, 25, 24, 6, 9},
  { 10},
  { 13, 11},
  { 1, 17, 14, 26, 2, 15, 16, 19, 11, 12},
  { 0, 2, 17, 15, 16, 19, 26, 13},
  { 12, 16, 5, 0, 8, 2, 19, 14},
  { 13, 12, 0, 17, 15},
  { 1, 8, 19, 5, 26, 17, 13, 0, 2, 11, 15, 14, 16},
  { 0, 14, 26, 12, 11, 15, 13, 1, 17},
  { 25, 3, 4, 24, 22, 7, 10, 21, 18},
  { 8, 16, 1, 14, 0, 11, 5, 15, 26, 19},
  { 10, 18, 6, 27, 24, 21, 9, 3, 20},
  { 3, 20, 21},
  { 18, 25, 23, 24, 6, 10, 21, 7, 27, 22},
  { 9, 25, 10, 23},
  { 22, 24},
  { 23, 6, 4, 10, 27, 3, 9, 7, 22, 20, 24, 18, 21, 25},
  { 8, 26},
  {  22, 7, 24, 23, 20, 3, 9, 4, 25, 10, 21, 6, 27}
};


char words[7][10];
byte correctButton, correctLabel, whoLevel = 1;
bool checkLabel[30];

byte currentButton, prevButton;

void whoModuleBoom() // if the bomb explodes what should the module display
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  lc.setLed(PIN_WHO_LED_1,false);
  lc.setLed(PIN_WHO_LED_2,false);
  lc.setLed(PIN_WHO_LED_3,false);
  lc.setLed(PIN_WHO_LED_FIN,false);
  lcdWho.clear();
  lcdWho.setCursor(0, 0); lcdWho.print(F("===================="));
  lcdWho.setCursor(0, 3); lcdWho.print(F("===================="));
  lcdWho.setCursor(9, 1); lcdWho.print(F("BOMB"));
  lcdWho.setCursor(7, 2); lcdWho.print(F("EXPLODED"));

}

void whoModuleDefusedPrint() // the module has been defused
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  lc.setLed(PIN_WHO_LED_1,false);
  lc.setLed(PIN_WHO_LED_2,false);
  lc.setLed(PIN_WHO_LED_3,false);
  lc.setLed(PIN_WHO_LED_FIN,true);
  defusedModuleBuzzer();
  isAnyModuleDefused=true;
  whoModuleDefused = true;
  lcdWho.clear();
  lcdWho.setCursor(0, 0); lcdWho.print(F("===================="));
  lcdWho.setCursor(0, 3); lcdWho.print(F("===================="));
  lcdWho.setCursor(7, 1); lcdWho.print(F("MODULE"));
  lcdWho.setCursor(7, 2); lcdWho.print(F("DEFUSED"));
}

// function that displays the labels
void printLabels()
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  lcdWho.setCursor((20 - strlen(words[0])) / 2, 0);
  lcdWho.print(words[0]);

  for (byte i = 1; i <= 3; i++)
  {
    lcdWho.setCursor(0, i);
    lcdWho.print(words[i]);
    lcdWho.setCursor(20 - strlen(words[3 + i]), i);
    lcdWho.print(words[3 + i]);
  }

}

byte findCorrectLabel(byte nr) // function that returns the index of the label you have to read, depending on the first label
{
  switch (nr) {
    case 0: return 2;
    case 1: return 4;
    case 2: return 6;
    case 3: return 4;
    case 4: return 6;
    case 5: return 2;
    case 6: return 3;
    case 7: return 5;
    case 8: return 6;
    case 9: return 2;
    case 10: return 6;
    case 11: return 5;
    case 12: return 5;
    case 13: return 3;
    case 14: return 3;
    case 15: return 6;
    case 16: return 5;
    case 17: return 6;
    case 18: return 5;
    case 19: return 5;
    case 20: return 1;
    case 21: return 6;
    case 22: return 3;
    case 23: return 5;
    case 24: return 2;
    case 25: return 6;
    case 26: return 4;
    case 27: return 6;
  }
}

byte indexOfLabel(char word[10]) // function that returns the index of a word from allLabels[]
{
  for (byte i = 0; i < LABELS_LENGTH; i++)
    if (strcmp(word, allLabels[i]) == 0) return i;
}

byte indexOfButton(byte index) // function that returns the index of a button
{
  for (byte i = 1; i <= 6; i++)
    if (strcmp(words[i], allLabels[index]) == 0) return i;
}

byte findCorrectButton(byte index) // function that returns the right button
{
  byte k = 0;
  while (checkLabel[indexOfButtonLabels[index][k]] == 0) k++;
  return indexOfButton(indexOfButtonLabels[index][k]);
}

void generateWords() // function that generates the words of the current stage
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  byte number = random(LABELS_LENGTH);
  for (byte i = 0; i < 100; i++) number = random(LABELS_LENGTH);

  strcpy(words[0], labels[number]); // the first label
  correctLabel = findCorrectLabel(number);

  for (byte i = 0; i < LABELS_LENGTH; i++) checkLabel[i] = 0; // we need this array so the words won't repeat

  for (byte i = 1; i <= 6; i++)
  {
    byte number = random(LABELS_LENGTH);
    while (checkLabel[number] == 1) number = random(LABELS_LENGTH); // if the words repeat generate another one
    checkLabel[number] = 1;
    strcpy(words[i], allLabels[number]);
  }

  byte index = indexOfLabel(words[correctLabel]);
  correctButton = findCorrectButton(index);

  // print the answer to the current stage
  if (DEBUG_LEVEL >= 1) {
    Serial.print(F("Who's on First Stage "));
    Serial.print(whoLevel);
    Serial.print(F(" : "));
    Serial.println(correctButton);
  }
}

void nextLevel() // moves you to the next stage of who
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  whoLevel++;
  if (whoLevel == 2) lc.setLed(PIN_WHO_LED_1,true);
  else if (whoLevel == 3) lc.setLed(PIN_WHO_LED_2,true);
  else if (whoLevel == 4) {
    lc.setLed(PIN_WHO_LED_3,true);
    whoModuleDefusedPrint();
  }
  if (whoLevel != 4) { // if the module hasn't been defused yet, go to the next stage 
    lcdWho.clear();
    generateWords();
    printLabels();
  }
}

void whoReset() // function that resets the module
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  addStrike();
  if (nrStrikes < 3) {
    whoLevel = 1;
    lc.setLed(PIN_WHO_LED_1,false);
    lc.setLed(PIN_WHO_LED_2,false);
    lc.setLed(PIN_WHO_LED_3,false);
    lcdWho.clear();
    generateWords();
    printLabels();
  }
}

void whoSetup() // define outputs and inputs for who
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  lcdWho.init();
  lcdWho.backlight();

  // first stage
  generateWords();
  printLabels();

  pinMode(PIN_WHO_BUTTON_1, INPUT);
  pinMode(PIN_WHO_BUTTON_2, INPUT);
  pinMode(PIN_WHO_BUTTON_3, INPUT);
  pinMode(PIN_WHO_BUTTON_4, INPUT);
  pinMode(PIN_WHO_BUTTON_5, INPUT);
  pinMode(PIN_WHO_BUTTON_6, INPUT);
}

byte whoPressedButton() // function that returns the pressed button
{
  prevButton = currentButton;
  if (digitalRead(PIN_WHO_BUTTON_1) == 1) return 1;
  if (digitalRead(PIN_WHO_BUTTON_2) == 1) return 2;
  if (digitalRead(PIN_WHO_BUTTON_3) == 1) return 3;
  if (digitalRead(PIN_WHO_BUTTON_4) == 1) return 4;
  if (digitalRead(PIN_WHO_BUTTON_5) == 1) return 5;
  if (digitalRead(PIN_WHO_BUTTON_6) == 1) return 6;
  return 0;
}


void whoCheckButton() // function that checks if a button is pressed
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  currentButton = whoPressedButton();
  if (currentButton != prevButton && currentButton != 0) {
    if (currentButton == correctButton) nextLevel(); // if you pressed the right answer, go to the next stage
    else whoReset();
  }
}

void whoLoop()
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  if (!whoModuleDefused)
    whoCheckButton();
}
