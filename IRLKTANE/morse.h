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

int morseCurrentDisplayNumber=1; // what station is displayed, default starting is 1
int buttonLeftState=0; // current state of the left button 
int buttonRightState=0; // current state of the right button
int buttonSubmitState=0; // current state of the submit button
int lastButtonLeftState=0; // previous state of the left button 
int lastButtonRightState=0; // previous state of the right button
int lastButtonSubmitState=0; // previous state of the submit button
int morseCorrectNumber=1; // will become random number selection from array in the future, set to dummy for testing

const unsigned long morseYellowLEDDot = 100; // how long should the dot last
const unsigned long morseYellowLEDDash = 400; // how long should the dash last
const unsigned long morseLetterLEDDelay = 500; // how long should the delay between letters be
const unsigned long morseDotDashLEDDelay = 200; // lenthg of delay for between dot and dash in same letter
const unsigned long morseWordLEDDelay = 1500; // delay for morse code between word repeat
unsigned long morseDelayCounter = 0; //a value that will count up while waiting for new letter/word //pointless action for while loop

unsigned long morseYellowLEDtimerOn;
unsigned long morseYellowLEDtimerOff;
unsigned long morseLetterFinishedtimer;
unsigned long morseWordFinishedtimer;

unsigned char morseTable[]=
{0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x00}; 
// 0x3f="0",0x06="1",0x5b="2",0x4f="3",0x66="4",0x6d="5",0x7d="6",0x07="7",0x7f="8",0x6f="9",
//0x77="A",0x7c="b",0x39="C",0x5e="d",0x79="E",0x71="F",0x00="OFF"

int morseWord1[] = {1,1,1,0,1,1,1,1,0,1,0,1,2,1,1,0,1,2,1,1}; //word shell where 1=dot 2=dash, 0=new letter pause
int morseCurrentDotDash=0;


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
  morseYellowLEDtimerOn = millis ();
  morseYellowLEDtimerOff = millis ();
  morseLetterFinishedtimer = millis ();
  morseWordFinishedtimer = millis ();
}

void morseDisplay(unsigned char num)
{
  digitalWrite(morseLatch,LOW);
  shiftOut(morseData,morseClock,MSBFIRST,morseTable[num]);
  digitalWrite(morseLatch,HIGH); 
}

void morseYellowLEDOn ()
  {
  digitalWrite (PIN_MORSE_LED_1, HIGH);

  // remember when we toggled it on
  morseYellowLEDtimerOn = millis ();  
  }  // end of turn on yellow LED

void morseYellowLEDOff ()
  {
  digitalWrite (PIN_MORSE_LED_1, LOW);

  // remember when we toggled it on
  morseYellowLEDtimerOff = millis ();  
  }  // end of turn on yellow LED

void morseDot(){
  if ((digitalRead (PIN_MORSE_LED_1) == LOW) && ((millis () - morseYellowLEDtimerOff) >= morseDotDashLEDDelay)) {
    morseYellowLEDOn ();
  }
  else if ( (millis () - morseYellowLEDtimerOn) >= morseYellowLEDDot) {
     morseYellowLEDOff ();
  }
}

void morseDash(){
  if ((digitalRead (PIN_MORSE_LED_1) == LOW) && ((millis () - morseYellowLEDtimerOff) >= morseDotDashLEDDelay)) {
    morseYellowLEDOn ();
  }
  else if ( (millis () - morseYellowLEDtimerOn) >= morseYellowLEDDash) {
     morseYellowLEDOff ();
  }
}

void morseBlinkWord(){ // not expanded for other words as only 1 word array is built for testing
  if (morseCorrectNumber=1) {
    for (int i = 0; i < 20; i = i + 1) {
      if(i == 1){
        morseDot();
      }
      else if(i == 2){
        morseDash();
      }
      else if(i == 0){ 
        morseLetterFinishedtimer = millis () ;
        Serial.print(morseLetterFinishedtimer);
        while (millis () - morseLetterFinishedtimer < morseLetterLEDDelay){
        Serial.print(millis () - morseWordFinishedtimer);
        morseDelayCounter+1 ;
        }
      }
      morseWordFinishedtimer = millis () ;
      while (millis () - morseWordFinishedtimer < morseWordFinishedtimer){
      Serial.print(millis () - morseWordFinishedtimer);
      morseDelayCounter+1 ;
      }
    }
  }
}

void morseLeftButtonPressed(){
  if (morseCurrentDisplayNumber > 0)  {
    morseCurrentDisplayNumber=morseCurrentDisplayNumber-1;
    morseDisplay(morseCurrentDisplayNumber);
    delay(MORSE_BUTTON_PRESS_DELAY);
  } else {
    morseDisplay(morseCurrentDisplayNumber);
    }
}
void morseRightButtonPressed(){
  if  (morseCurrentDisplayNumber < 15)  {
    morseCurrentDisplayNumber=morseCurrentDisplayNumber+1;
    morseDisplay(morseCurrentDisplayNumber);
    delay(MORSE_BUTTON_PRESS_DELAY);
  } else {
    morseDisplay(morseCurrentDisplayNumber);
    }
}
void morseSubmitButtonPressed(){
  Serial.println(morseCurrentDisplayNumber);
  Serial.println(morseCorrectNumber);
  if (morseCurrentDisplayNumber==morseCorrectNumber) {
  morseModuleDefused = true;
  digitalWrite(PIN_MORSE_LED_GREEN,HIGH);
  }
  else { //if (morseCurrentDisplayNumber!=morseCorrectNumber) {
  addStrike();
  morseDisplay(0); // flashes 0000 and goes back to number entered to emphasize strike.
  delay(MORSE_BUTTON_PRESS_DELAY); // Holds for short time
  morseDisplay(morseCurrentDisplayNumber); // goes back to number submitted incorrectly
  }
}

void morseLoop()
{
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
  
  if (!morseModuleDefused) morseBlinkWord();
  
/*
 * Interpret the signal from the flashing light (the flashing should start as soon as the bomb starts countdown) 
 * using the Morse Code chart to spell one of the words on the table.
 * The signal will loop, with a long gap between repetitions. (timing between letters and words will be important to get right)
 * Once the word is identified, set the corresponding frequency and press the transmit (TX) button.

If the word is:   Respond at frequency:   Pattern (1=dot,2=dash)
shell             3.505 MHz               111 1111 1 1211 1211
halls             3.515 MHz               1111 12 1211 1211 111
slick             3.522 MHz               111 1211 11 2121 212
trick             3.532 MHz               2 121 11 2121 212
boxes             3.535 MHz               2111 222 2112 1 111
leaks             3.542 MHz               1211 1 12 212 111
strobe            3.545 MHz               111 2 121 222 2111 1
bistro            3.552 MHz               2111 11 111 2 121 222
flick             3.555 MHz               1121 1211 11 2121 212
bombs             3.565 MHz               2111 222 22 2111 111
break             3.572 MHz               2111 121 1 12 212
brick             3.582 MHz               2111 121 11 2121 212
sting             3.592 MHz               111 2 11 21 221
vector            3.595 MHz               1112 1 2121 2 222 121
beats             3.600 MHz               2111 1 12 2 111


 * A strike will be recorded if:
 * The TX button is pressed with an incorrect frequency set.

 * The module will be disarmed when:
 * The TX button is pressed with the correct frequency set.
 */
  
}

void morseModuleBoom()
{
  // if the bomb explodes the display with the frequency should clear and the flashing led should stop
  digitalWrite(PIN_MORSE_LED_1,LOW); // stop flashing yellow morse
  digitalWrite(PIN_MORSE_LED_GREEN,LOW); // turn of green module complete LED
  morseDisplay(16); //shows "    " on module tx display
}
