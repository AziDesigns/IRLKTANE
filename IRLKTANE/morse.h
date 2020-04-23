// On the Subject of Morse Code
/*
 * The Morse Code module is a module that consists of a light flashing in Morse Code, a radio with a displayed frequency and a TX button. 
 * The defuser must interpret the flashing Morse Light as dots and dashes to form a word in Morse Code. 
 * This word corresponds to a radio frequency that the expert must tell the defuser to transmit. 
 * The defuser must scroll to that frequency, and press the TX button to solve the module.
 */

#define PIN_MORSE_LED_1 3 // yellow flashing LED for morse
#define PIN_MORSE_LED_GREEN 12 // module complete LED
#define PIN_MORSE_BUTTON_1 4 // left button
#define PIN_MORSE_BUTTON_2 5 // right button
#define PIN_MORSE_BUTTON_3 6 // TX (submit) button

#define MORSE_BUTTON_PRESS_DELAY 50 // time required to prevent button bounce

int morseLatch=9;  // 74HC595  pin 9 STCP
int morseClock=10; // 74HC595  pin 10 SHCP
int morseData=8;   // 74HC595  pin 8 DS

int morseCurrentDisplayNumber=0; // what station is displayed, default starting is 0
int buttonLeftState=0; // current state of the left button 
int buttonRightState=0; // current state of the right button
int buttonSubmitState=0; // current state of the submit button
int lastButtonLeftState=0; // previous state of the left button 
int lastButtonRightState=0; // previous state of the right button
int lastButtonSubmitState=0; // previous state of the submit button
int morseCorrectNumber=0; // picks which word will be correct is made random in setup.

const unsigned long morseYellowLEDDot = 200; // how long should the dot last
const unsigned long morseYellowLEDDash = 600; // how long should the dash last
const unsigned long morseLetterLEDDelay = 800; // how long should the delay between letters be
const unsigned long morseDotDashLEDDelay = 250; // lenthg of delay for between dot and dash in same letter
const unsigned long morseWordLEDDelay = 4000; // delay for morse code between word repeat
const unsigned long morseWrongTXDelay = 1500; // delay for wrong TX submited

unsigned long morsePreviousMillis = 0; 

// this is the table that controls which LED # is shown.
unsigned char morseTable[]=
{0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x00}; 
// 0x3f="0",0x06="1",0x5b="2",0x4f="3",0x66="4",0x6d="5",0x7d="6",0x07="7",0x7f="8",0x6f="9",
//0x77="A",0x7c="b",0x39="C",0x5e="d",0x79="E",0x71="F",0x00="OFF"

// morse words where 1=dot 2=dash, 0=new letter pause, 3=repeat word pause, 4=new dot/dash pause
int morseWord0[] = {1,4,1,4,1,0,1,4,1,4,1,4,1,0,1,0,1,4,2,4,1,4,1,3}; // shell - 3.505 MHz
int morseWord1[] = {1,4,1,4,1,4,1,0,1,4,2,0,1,4,2,4,1,4,1,0,1,4,2,4,1,4,1,0,1,4,1,4,1,3}; // halls - 3.515 MHz
int morseWord2[] = {1,4,1,4,1,0,1,4,2,4,1,4,1,0,1,4,1,0,2,4,1,4,2,4,1,0,2,4,1,4,2,3}; // slick - 3.522 MHz
int morseWord3[] = {2,0,1,4,2,4,1,0,1,4,1,0,2,4,1,4,2,4,1,0,2,4,1,4,2,3}; // trick - 3.532 MHz
int morseWord4[] = {2,4,1,4,1,4,1,0,2,4,2,4,2,0,2,4,1,4,1,4,2,0,1,0,1,4,1,4,1,3}; // boxes - 3.535 MHz
int morseWord5[] = {1,4,2,4,1,4,1,0,1,0,1,4,2,0,2,4,1,4,2,0,1,4,1,4,1,3}; // leaks - 3.542 MHz
int morseWord6[] = {1,4,1,4,1,0,2,0,1,4,2,4,1,0,2,4,2,4,2,0,2,4,1,4,1,4,1,0,1,3}; // strobe - 3.545 MHz
int morseWord7[] = {2,4,1,4,1,4,1,0,1,4,1,0,1,4,1,4,1,0,2,0,1,4,2,4,1,0,2,4,2,4,2,3}; // bistro - 3.552 MHz
int morseWord8[] = {1,4,1,4,2,4,1,0,1,4,2,4,1,4,1,0,1,4,1,0,2,4,1,4,2,4,1,0,2,4,1,4,2,3}; // flick - 3.555 MHz
int morseWord9[] = {2,4,1,4,1,4,1,0,2,4,2,4,2,0,2,4,2,0,2,4,1,4,1,4,1,0,1,4,1,4,1,3}; // bombs - 3.565 MHz
int morseWord10[] ={2,4,1,4,1,4,1,0,1,4,2,4,1,0,1,0,1,4,2,0,2,4,1,4,2,3}; // break - 3.572 MHz
int morseWord11[] ={2,4,1,4,1,4,1,0,1,4,2,4,1,0,1,4,1,0,2,4,1,4,2,4,1,0,2,4,1,4,2,3}; // brick - 3.575 MHz
int morseWord12[] ={1,4,1,4,1,0,2,0,1,0,1,4,2,0,2,4,1,4,2,3}; // steak - 3.582 MHz
int morseWord13[] ={1,4,1,4,1,0,2,0,1,4,1,0,2,4,1,0,2,4,2,4,1,3}; // sting - 3.592 MHz
int morseWord14[] ={1,4,1,4,1,4,2,0,1,0,2,4,1,4,2,4,1,0,2,0,2,4,2,4,2,0,1,4,2,4,1,3}; // vector - 3.595 MHz
int morseWord15[] ={2,4,1,4,1,4,1,0,1,0,1,4,2,0,2,0,1,4,1,4,1,3}; // beats - 3.600 MHz

// array of all possible morse words // an array and pointers to arrays.
const int* arr_list[16];
const int arr_sizes[16] = { 24, 34, 32, 26, 30, 26, 30, 32, 34, 32, 26, 32, 20, 22, 32, 22 };  // array of the array sizes

void map_arrays() {          // list of pointers with the addresses of the
// arrays
  arr_list[0] = morseWord0;
  arr_list[1] = morseWord1;
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
  arr_list[14] = morseWord15;
}

void morseSetup()
{
  pinMode(morseLatch,OUTPUT);
  pinMode(morseClock,OUTPUT);
  pinMode(morseData,OUTPUT);
  pinMode(PIN_MORSE_LED_1,OUTPUT);
  pinMode(PIN_MORSE_LED_GREEN,OUTPUT);
  pinMode(PIN_MORSE_BUTTON_1,INPUT);
  pinMode(PIN_MORSE_BUTTON_2,INPUT);
  pinMode(PIN_MORSE_BUTTON_3,INPUT);
  morseCorrectNumber=random(16);
  Serial.println("Morse Correct Number: ");
  Serial.println(morseCorrectNumber);
  map_arrays();
}

void morseDisplay(unsigned char num)
{
  digitalWrite(morseLatch,LOW);
  shiftOut(morseData,morseClock,MSBFIRST,morseTable[num]);
  digitalWrite(morseLatch,HIGH); 
}

void morseLeftButtonPressed(){
  if (morseCurrentDisplayNumber > 0)  {
    morseCurrentDisplayNumber=morseCurrentDisplayNumber-1;
    morseDisplay(morseCurrentDisplayNumber);
    //delay(MORSE_BUTTON_PRESS_DELAY);
  } else {
    morseDisplay(morseCurrentDisplayNumber);
    }
}
void morseRightButtonPressed(){
  if  (morseCurrentDisplayNumber < 14)  {
    morseCurrentDisplayNumber=morseCurrentDisplayNumber+1;
    morseDisplay(morseCurrentDisplayNumber);
    //delay(MORSE_BUTTON_PRESS_DELAY);
  } else {
    morseDisplay(morseCurrentDisplayNumber);
    }
}
void morseSubmitButtonPressed(){
  Serial.println("Morse Number Submitted: ");
  Serial.println(morseCurrentDisplayNumber);
  Serial.println("Vs. Morse Correct Number: ");
  Serial.println(morseCorrectNumber);
  if (morseCurrentDisplayNumber==morseCorrectNumber) {
  morseModuleDefused = true;
  Serial.println("Morse Module Defused");
  digitalWrite(PIN_MORSE_LED_GREEN,HIGH);
  }
  else { 
  addStrike();
  morseDisplay(16); // flashes 0000 and goes back to number entered to emphasize strike.
  delay(morseWrongTXDelay); // need to update this from a delay to a millis() comparison like BlinkWithoutDelay
  morseDisplay(morseCurrentDisplayNumber); // goes back to number submitted incorrectly
  }
}

void morseLoop()
{
  if (!morseModuleDefused) {
    // read the pushbutton input pins:
    buttonLeftState = digitalRead(PIN_MORSE_BUTTON_1);
    buttonRightState = digitalRead(PIN_MORSE_BUTTON_2);
    buttonSubmitState = digitalRead(PIN_MORSE_BUTTON_3);
  
    // compare the buttonLeftState to its previous state
    if (buttonLeftState != lastButtonLeftState) {
      // if the state has changed, increment the counter
      if (buttonLeftState == HIGH) {
        // if the current state is HIGH then the button went from off to on:
        Serial.println("leftOn");
        morseLeftButtonPressed();
      } else {
        // if the current state is LOW then the button went from on to off:
        Serial.println("leftOff");
      }
      // Delay a little bit to avoid bouncing
      delay(MORSE_BUTTON_PRESS_DELAY);
    }
    // save the current state as the last state, for next time through the loop
    lastButtonLeftState = buttonLeftState;
  
    // compare the buttonRightState to its previous state
    if (buttonRightState != lastButtonRightState) {
      // if the state has changed, increment the counter
      if (buttonRightState == HIGH) {
        // if the current state is HIGH then the button went from off to on:
        Serial.println("rightOn");
        morseRightButtonPressed();
      } else {
        // if the current state is LOW then the button went from on to off:
        Serial.println("rightOff");
      }
      // Delay a little bit to avoid bouncing
      delay(MORSE_BUTTON_PRESS_DELAY);
    }
    // save the current state as the last state, for next time through the loop
    lastButtonRightState = buttonRightState;
  
    // compare the buttonSubmitState to its previous state
    if (buttonSubmitState != lastButtonSubmitState) {
      // if the state has changed, increment the counter
      if (buttonSubmitState == HIGH) {
        // if the current state is HIGH then the button went from off to on:
        Serial.println("submitOn");
        morseSubmitButtonPressed();
      } else {
        // if the current state is LOW then the button went from on to off:
        Serial.println("submitOff");
      }
      // Delay a little bit to avoid bouncing
      delay(MORSE_BUTTON_PRESS_DELAY);
    }
    // save the current state as the last state, for next time through the loop
    lastButtonSubmitState = buttonSubmitState;
    
    // display starting number (always fixed lowest # in array)
    morseDisplay(morseCurrentDisplayNumber);

// -----------------------------------------------------------------------------

  
  static unsigned i = 0;
    switch (arr_list[morseCorrectNumber] [i])  { 
      case 4:
          digitalWrite (PIN_MORSE_LED_1, LOW);
          if (millis() - morsePreviousMillis >= 250) {
            morsePreviousMillis = millis(); // save the last time you blinked the LED
            i = i < arr_sizes[morseCorrectNumber]-1 ? i+1 : 0;
          }
          else 
          break;
      
      case 3:
          digitalWrite (PIN_MORSE_LED_1, LOW);
          if (millis() - morsePreviousMillis >= 4000) {
            morsePreviousMillis = millis(); // save the last time you blinked the LED
            i = i < arr_sizes[morseCorrectNumber]-1 ? i+1 : 0;
          }
          else 
          break;
      
      case 2:
          digitalWrite (PIN_MORSE_LED_1, HIGH);
          if (millis() - morsePreviousMillis >= 600) {
            morsePreviousMillis = millis(); // save the last time you blinked the LED
            i = i < arr_sizes[morseCorrectNumber]-1 ? i+1 : 0;
          }
          else 
          break;
  
      case 1:
          digitalWrite (PIN_MORSE_LED_1, HIGH);
          if (millis() - morsePreviousMillis >= 200) {
            morsePreviousMillis = millis(); // save the last time you blinked the LED
            i = i < arr_sizes[morseCorrectNumber]-1 ? i+1 : 0;
          }
          else 
          break;
  
      case 0:
      default:
          digitalWrite (PIN_MORSE_LED_1, LOW);
          if (millis() - morsePreviousMillis >= 800) {
            morsePreviousMillis = millis(); // save the last time you blinked the LED
            i = i < arr_sizes[morseCorrectNumber]-1 ? i+1 : 0;
          }
          else 
          break;
      }
  }
}

void morseModuleBoom()
{
  // if the bomb explodes the display with the frequency should clear and the flashing led should stop
  digitalWrite(PIN_MORSE_LED_1,LOW); // stop flashing yellow morse
  digitalWrite(PIN_MORSE_LED_GREEN,LOW); // turn of green module complete LED
  morseDisplay(16); //shows "    " on module tx display
}
