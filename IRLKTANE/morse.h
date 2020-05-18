// On the Subject of Morse Code
/*
  KNOWN ISSUES:
  THE FIRST LED FLASH IS SHORTER THAN ALL OTHERS FOR SOME REASON.
  - I BELIEVE THIS IS DUE TO WHERE I AM SETTING THE STARTING morsePreviousMillis value in setup rather than it being in the loop?
*/
#define PIN_MORSE_LED_1 29 // yellow flashing LED for morse
#define PIN_MORSE_LED_FIN 27 // module complete LED
#define PIN_MORSE_BUTTON_1 24 // left button
#define PIN_MORSE_BUTTON_2 25 // right button
#define PIN_MORSE_BUTTON_3 26 // TX (submit) button

byte morseCurrentDisplayNumber = 0; // what station is displayed, default starting is 0
byte morseButtonLeftState = 0; // current state of the left button
byte morseButtonRightState = 0; // current state of the right button
byte morseButtonSubmitState = 0; // current state of the submit button
byte lastMorseButtonLeftState = 0; // previous state of the left button
byte lastMorseButtonRightState = 0; // previous state of the right button
byte lastMorseButtonSubmitState = 0; // previous state of the submit button
byte morseCorrectNumber = 0; // picks which word will be correct is made random in setup.
bool morseDisplayActivated = false;

const byte morseYellowLEDDot = 200; // how long should the dot last
const int morseYellowLEDDash = 600; // how long should the dash last
const int morseLetterLEDDelay = 850; // how long should the delay between letters be
const byte morseDotDashLEDDelay = 250; // lenthg of delay for between dot and dash in same letter
const int morseWordLEDDelay = 4000; // delay for morse code between word repeat
const int morseWrongTXDelay = 1500; // delay for wrong TX submited

unsigned long morsePreviousMillis;

// morse words where 1=dot 2=dash, 0=new letter pause, 3=repeat word pause, 4=new dot/dash pause
const byte morseWord0[] = {1, 4, 1, 4, 1, 0, 1, 4, 1, 4, 1, 4, 1, 0, 1, 0, 1, 4, 2, 4, 1, 4, 1, 0, 1, 4, 2, 4, 1, 4, 1, 3}; // shell - 3.505 MHz
const byte morseWord1[] = {1, 4, 1, 4, 1, 4, 1, 0, 1, 4, 2, 0, 1, 4, 2, 4, 1, 4, 1, 0, 1, 4, 2, 4, 1, 4, 1, 0, 1, 4, 1, 4, 1, 3}; // halls - 3.515 MHz
const byte morseWord2[] = {1, 4, 1, 4, 1, 0, 1, 4, 2, 4, 1, 4, 1, 0, 1, 4, 1, 0, 2, 4, 1, 4, 2, 4, 1, 0, 2, 4, 1, 4, 2, 3}; // slick - 3.522 MHz
const byte morseWord3[] = {2, 0, 1, 4, 2, 4, 1, 0, 1, 4, 1, 0, 2, 4, 1, 4, 2, 4, 1, 0, 2, 4, 1, 4, 2, 3}; // trick - 3.532 MHz
const byte morseWord4[] = {2, 4, 1, 4, 1, 4, 1, 0, 2, 4, 2, 4, 2, 0, 2, 4, 1, 4, 1, 4, 2, 0, 1, 0, 1, 4, 1, 4, 1, 3}; // boxes - 3.535 MHz
const byte morseWord5[] = {1, 4, 2, 4, 1, 4, 1, 0, 1, 0, 1, 4, 2, 0, 2, 4, 1, 4, 2, 0, 1, 4, 1, 4, 1, 3}; // leaks - 3.542 MHz
const byte morseWord6[] = {1, 4, 1, 4, 1, 0, 2, 0, 1, 4, 2, 4, 1, 0, 2, 4, 2, 4, 2, 0, 2, 4, 1, 4, 1, 4, 1, 0, 1, 3}; // strobe - 3.545 MHz
const byte morseWord7[] = {2, 4, 1, 4, 1, 4, 1, 0, 1, 4, 1, 0, 1, 4, 1, 4, 1, 0, 2, 0, 1, 4, 2, 4, 1, 0, 2, 4, 2, 4, 2, 3}; // bistro - 3.552 MHz
const byte morseWord8[] = {1, 4, 1, 4, 2, 4, 1, 0, 1, 4, 2, 4, 1, 4, 1, 0, 1, 4, 1, 0, 2, 4, 1, 4, 2, 4, 1, 0, 2, 4, 1, 4, 2, 3}; // flick - 3.555 MHz
const byte morseWord9[] = {2, 4, 1, 4, 1, 4, 1, 0, 2, 4, 2, 4, 2, 0, 2, 4, 2, 0, 2, 4, 1, 4, 1, 4, 1, 0, 1, 4, 1, 4, 1, 3}; // bombs - 3.565 MHz
const byte morseWord10[] = {2, 4, 1, 4, 1, 4, 1, 0, 1, 4, 2, 4, 1, 0, 1, 0, 1, 4, 2, 0, 2, 4, 1, 4, 2, 3}; // break - 3.572 MHz
const byte morseWord11[] = {2, 4, 1, 4, 1, 4, 1, 0, 1, 4, 2, 4, 1, 0, 1, 4, 1, 0, 2, 4, 1, 4, 2, 4, 1, 0, 2, 4, 1, 4, 2, 3}; // brick - 3.575 MHz
const byte morseWord12[] = {1, 4, 1, 4, 1, 0, 2, 0, 1, 0, 1, 4, 2, 0, 2, 4, 1, 4, 2, 3}; // steak - 3.582 MHz
const byte morseWord13[] = {1, 4, 1, 4, 1, 0, 2, 0, 1, 4, 1, 0, 2, 4, 1, 0, 2, 4, 2, 4, 1, 3}; // sting - 3.592 MHz
const byte morseWord14[] = {1, 4, 1, 4, 1, 4, 2, 0, 1, 0, 2, 4, 1, 4, 2, 4, 1, 0, 2, 0, 2, 4, 2, 4, 2, 0, 1, 4, 2, 4, 1, 3}; // vector - 3.595 MHz
const byte morseWord15[] = {2, 4, 1, 4, 1, 4, 1, 0, 1, 0, 1, 4, 2, 0, 2, 0, 1, 4, 1, 4, 1, 3}; // beats - 3.600 MHz

// array of all possible morse words // an array and pointers to arrays.
const byte* arr_list[16];
const byte arr_sizes[16] = { 32, 34, 32, 26, 30, 26, 30, 32, 34, 32, 26, 32, 20, 22, 32, 22 };  // array of the array sizes

void map_arrays() 
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }        // list of pointers with the addresses of the
  // arrays
  arr_list[0] = morseWord0;
  //arr_list[1] = morseWord1;
  arr_list[2] = morseWord2;
  arr_list[3] = morseWord3;
  arr_list[4] = morseWord4;
  arr_list[5] = morseWord5;
  arr_list[6] = morseWord6;
  arr_list[7] = morseWord7;
  arr_list[8] = morseWord8;
  arr_list[9] = morseWord9;
  arr_list[10] = morseWord10;
  arr_list[11] = morseWord11;
  arr_list[12] = morseWord12;
  arr_list[13] = morseWord13;
  arr_list[14] = morseWord14;
  arr_list[15] = morseWord15;
}

void morseSetup()
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  lc.shutdown(0,false);
  lc.setIntensity(0,8);
  lc.clearDisplay(0);
  pinMode(PIN_MORSE_LED_1, OUTPUT);
  pinMode(PIN_MORSE_LED_FIN, OUTPUT);
  pinMode(PIN_MORSE_BUTTON_1, INPUT);
  pinMode(PIN_MORSE_BUTTON_2, INPUT);
  pinMode(PIN_MORSE_BUTTON_3, INPUT);
  morseCorrectNumber = random(16);
  if (DEBUG_LEVEL >= 1) {
    Serial.println(F("Morse Correct Number: "));
    Serial.println(morseCorrectNumber);
  }
  map_arrays();
  morsePreviousMillis = 0;
}

void morseDisplay(unsigned char num)
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  if (num==0) {
    lc.setDigit(0,3,5,false);
    lc.setDigit(0,2,0,false);
    lc.setDigit(0,1,5,false);
    lc.setDigit(0,0,3,true);
  } else if (num==1) {
    lc.setDigit(0,3,5,false);
    lc.setDigit(0,2,1,false);
    lc.setDigit(0,1,5,false);
    lc.setDigit(0,0,3,true);
  } else if (num==2) {
    lc.setDigit(0,3,2,false);
    lc.setDigit(0,2,2,false);
    lc.setDigit(0,1,5,false);
    lc.setDigit(0,0,3,true);
  } else if (num==3) {
    lc.setDigit(0,3,2,false);
    lc.setDigit(0,2,3,false);
    lc.setDigit(0,1,5,false);
    lc.setDigit(0,0,3,true);
  } else if (num==4) {
    lc.setDigit(0,3,5,false);
    lc.setDigit(0,2,3,false);
    lc.setDigit(0,1,5,false);
    lc.setDigit(0,0,3,true);
  } else if (num==5) {
    lc.setDigit(0,3,2,false);
    lc.setDigit(0,2,4,false);
    lc.setDigit(0,1,5,false);
    lc.setDigit(0,0,3,true);
  } else if (num==6) {
    lc.setDigit(0,3,5,false);
    lc.setDigit(0,2,4,false);
    lc.setDigit(0,1,5,false);
    lc.setDigit(0,0,3,true);
  } else if (num==7) {
    lc.setDigit(0,3,2,false);
    lc.setDigit(0,2,5,false);
    lc.setDigit(0,1,5,false);
    lc.setDigit(0,0,3,true);
  } else if (num==8) {
    lc.setDigit(0,3,5,false);
    lc.setDigit(0,2,5,false);
    lc.setDigit(0,1,5,false);
    lc.setDigit(0,0,3,true);
  } else if (num==9) {
    lc.setDigit(0,3,5,false);
    lc.setDigit(0,2,6,false);
    lc.setDigit(0,1,5,false);
    lc.setDigit(0,0,3,true);
  } else if (num==10) {
    lc.setDigit(0,3,2,false);
    lc.setDigit(0,2,7,false);
    lc.setDigit(0,1,5,false);
    lc.setDigit(0,0,3,true);
  } else if (num==11) {
    lc.setDigit(0,3,5,false);
    lc.setDigit(0,2,7,false);
    lc.setDigit(0,1,5,false);
    lc.setDigit(0,0,3,true);
  } else if (num==12) {
    lc.setDigit(0,3,2,false);
    lc.setDigit(0,2,8,false);
    lc.setDigit(0,1,5,false);
    lc.setDigit(0,0,3,true);
  } else if (num==13) {
    lc.setDigit(0,3,2,false);
    lc.setDigit(0,2,9,false);
    lc.setDigit(0,1,5,false);
    lc.setDigit(0,0,3,true);
  } else if (num==14) {
    lc.setDigit(0,3,5,false);
    lc.setDigit(0,2,9,false);
    lc.setDigit(0,1,5,false);
    lc.setDigit(0,0,3,true);
  } else if (num==15) {
    lc.setDigit(0,3,0,false);
    lc.setDigit(0,2,0,false);
    lc.setDigit(0,1,6,false);
    lc.setDigit(0,0,3,true);
  } else if (num==16) {
    lc.setDigit(0,3,0,false);
    lc.setDigit(0,2,0,false);
    lc.setDigit(0,1,0,false);
    lc.setDigit(0,0,0,false);
  }
}

void morseLeftButtonPressed() 
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  if (morseCurrentDisplayNumber > 0)  {
    morseCurrentDisplayNumber = morseCurrentDisplayNumber - 1;
    morseDisplay(morseCurrentDisplayNumber);
  } else {
    morseDisplay(morseCurrentDisplayNumber);
  }
}
void morseRightButtonPressed() 
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  if  (morseCurrentDisplayNumber < 15)  {
    morseCurrentDisplayNumber = morseCurrentDisplayNumber + 1;
    morseDisplay(morseCurrentDisplayNumber);
  } else {
    morseDisplay(morseCurrentDisplayNumber);
  }
}
void morseSubmitButtonPressed() 
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  if (DEBUG_LEVEL >= 1) {
    Serial.println(F("Morse Number Submitted: "));
    Serial.println(morseCurrentDisplayNumber);
    Serial.println(F("Vs. Morse Correct Number: "));
    Serial.println(morseCorrectNumber);
  }
  if (morseCurrentDisplayNumber == morseCorrectNumber) {
    morseModuleDefused = true;
    if (DEBUG_LEVEL >= 1) {
      Serial.println(F("Morse Module Defused"));
    }
    digitalWrite(PIN_MORSE_LED_FIN, HIGH); // turn on module complete light
    digitalWrite(PIN_MORSE_LED_1, LOW); // turn off flashing light upon success
    lc.shutdown(0,true); //shows "    " on module tx display
    defusedModuleBuzzer();
    isAnyModuleDefused=true;
  } else {
    addStrike();
  }
}

void morseLoop()
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  if (morseDisplayActivated == false) {
    morseDisplay(morseCurrentDisplayNumber);
    morseDisplayActivated = true;
  }
  if (!morseModuleDefused) {
    // read the pushbutton input pins:
    morseButtonLeftState = digitalRead(PIN_MORSE_BUTTON_1);
    morseButtonRightState = digitalRead(PIN_MORSE_BUTTON_2);
    morseButtonSubmitState = digitalRead(PIN_MORSE_BUTTON_3);

    // compare the morseButtonLeftState to its previous state
    if (morseButtonLeftState != lastMorseButtonLeftState) {
      if (morseButtonLeftState == HIGH) {
        if (DEBUG_LEVEL >= 1) {
          Serial.println(F("morseLeftOn"));
        }
        morseLeftButtonPressed();
      } else {
        if (DEBUG_LEVEL >= 1) {
          Serial.println(F("morseLeftOff"));
        }
      }
    }
    // save the current state as the last state, for next time through the loop
    lastMorseButtonLeftState = morseButtonLeftState;

    // compare the morseButtonRightState to its previous state
    if (morseButtonRightState != lastMorseButtonRightState) {
      if (morseButtonRightState == HIGH) {
        if (DEBUG_LEVEL >= 1) {
          Serial.println(F("morseRightOn"));
        }
        morseRightButtonPressed();
      } else {
        if (DEBUG_LEVEL >= 1) {
          Serial.println(F("morseRightOff"));
        }
      }
    }
    // save the current state as the last state, for next time through the loop
    lastMorseButtonRightState = morseButtonRightState;

    // compare the morseButtonSubmitState to its previous state
    if (morseButtonSubmitState != lastMorseButtonSubmitState) {
      if (morseButtonSubmitState == HIGH) {
        if (DEBUG_LEVEL >= 1) {
          Serial.println(F("morseSubmitOn"));
        }
        morseSubmitButtonPressed();
      } else {
        if (DEBUG_LEVEL >= 1) {
          Serial.println(F("morseSubmitOff"));
        }
      }
    }
    // save the current state as the last state, for next time through the loop
    lastMorseButtonSubmitState = morseButtonSubmitState;


    // -----------------------------------------------------------------------------

    if (exploded != true) {
      static unsigned i = 0;
      switch (arr_list[morseCorrectNumber] [i])  {
        case 4:
          digitalWrite (PIN_MORSE_LED_1, LOW);
          if (millis() - morsePreviousMillis >= morseDotDashLEDDelay) {
            morsePreviousMillis = millis(); // save the last time you blinked the LED
            i = i < arr_sizes[morseCorrectNumber] - 1 ? i + 1 : 0;
          }
          else
            break;

        case 3:
          digitalWrite (PIN_MORSE_LED_1, LOW);
          if (millis() - morsePreviousMillis >= morseWordLEDDelay) {
            morsePreviousMillis = millis(); // save the last time you blinked the LED
            i = i < arr_sizes[morseCorrectNumber] - 1 ? i + 1 : 0;
          }
          else
            break;

        case 2:
          digitalWrite (PIN_MORSE_LED_1, HIGH);
          if (millis() - morsePreviousMillis >= morseYellowLEDDash) {
            morsePreviousMillis = millis(); // save the last time you blinked the LED
            i = i < arr_sizes[morseCorrectNumber] - 1 ? i + 1 : 0;
          }
          else
            break;

        case 1:
          digitalWrite (PIN_MORSE_LED_1, HIGH);
          if (millis() - morsePreviousMillis >= morseYellowLEDDot) {
            morsePreviousMillis = millis(); // save the last time you blinked the LED
            i = i < arr_sizes[morseCorrectNumber] - 1 ? i + 1 : 0;
          }
          else
            break;

        case 0:
        default:
          digitalWrite (PIN_MORSE_LED_1, LOW);
          if (millis() - morsePreviousMillis >= morseLetterLEDDelay) {
            morsePreviousMillis = millis(); // save the last time you blinked the LED
            i = i < arr_sizes[morseCorrectNumber] - 1 ? i + 1 : 0;
          }
          else
            break;
      }
    }
  }
}

void morseModuleBoom()
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  // if the bomb explodes the display with the frequency should clear and the flashing led should stop
  digitalWrite(PIN_MORSE_LED_1, LOW); // stop flashing yellow morse
  digitalWrite(PIN_MORSE_LED_FIN, LOW); // turn of green module complete LED
  lc.shutdown(0,true); //shows "    " on module tx display
}
