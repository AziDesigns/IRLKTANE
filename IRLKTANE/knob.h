// On the Subject of Knobs
/*
  KNOWN ISSUES:
  MODULE FULLY FUNCTIONS FROM A LED RANDOMAZATION AND RESETTING TIME ETC,
    HOWEVER THE KNOB (ROTARY SWITCH) HAS STILL NOT BEEN WIRED UP/ INSTALLED AND THAT LOGIC HAS NOT BEEN VERIFIED.
  ALMOST 100% SURE I WILL NEED TO CHANGE THESE VALUES
      #define KNOB_POSITION_1 200
      #define KNOB_POSITION_2 500
      #define KNOB_POSITION_3 900
      #define KNOB_POSITION_4 1200
  PINS FOR LED'S WILL CONFLICT ONCE MERGED WITH MAIN BRANCH. USING 2ND MEGA2560 FOR TESTING THIS MODULE
  NEEDS ITS OWN TIMER / DISPLAY CURRENTLY USING SAME DISPLAY AS OVERALL TIME TO REDUCE EFFORT IN WRITING CODE
*/
/*
   This module consists of a knob that can be turned in four directions (up,down,left,right)
   as well as twelve LEDs that may or may not light up upon the module activation.
   When this needy module activates, some of the LEDs on the bottom will light up,
   and the knob will rotate. The Defuser must communicate the formation of the lit LEDs
   to the Expert who will then tell them which position to set the knob to.
*/

#define KNOB_PIN_CLK 82 // knob countdown clock CLK
#define KNOB_PIN_DIO 83 // knob countdown clock DIO
#define KNOB_DEFAULT_TIME 40 // default time per rotation
#define KNOB_PIN_ROTARY_SWITCH A0 // rotary switch analog input pin
#define PIN_KNOB_LED_1  70
#define PIN_KNOB_LED_2  71
#define PIN_KNOB_LED_3  72
#define PIN_KNOB_LED_4  73
#define PIN_KNOB_LED_5  74
#define PIN_KNOB_LED_6  75
#define PIN_KNOB_LED_7  76
#define PIN_KNOB_LED_8  77
#define PIN_KNOB_LED_9  78
#define PIN_KNOB_LED_10 79
#define PIN_KNOB_LED_11 80
#define PIN_KNOB_LED_12 81
#define KNOB_POSITION_1 200
#define KNOB_POSITION_2 500
#define KNOB_POSITION_3 900
#define KNOB_POSITION_4 1200

SevenSegmentExtended knobTimer(PIN_CLK, PIN_DIO);
unsigned long knobSeconds = 0;
int knobSec = KNOB_DEFAULT_TIME + 1;
int knobLEDArray = 0;
int knobPositionValue = 0;
int knobCurrentState = 0;
int knobCorrectState = 0;

void knobSetup() 
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  knobTimer.begin();            // initializes the display
  knobTimer.setBacklight(100);  // set the brightness to 100%
  pinMode(PIN_KNOB_LED_1, OUTPUT);
  pinMode(PIN_KNOB_LED_2, OUTPUT);
  pinMode(PIN_KNOB_LED_3, OUTPUT);
  pinMode(PIN_KNOB_LED_4, OUTPUT);
  pinMode(PIN_KNOB_LED_5, OUTPUT);
  pinMode(PIN_KNOB_LED_6, OUTPUT);
  pinMode(PIN_KNOB_LED_7, OUTPUT);
  pinMode(PIN_KNOB_LED_8, OUTPUT);
  pinMode(PIN_KNOB_LED_9, OUTPUT);
  pinMode(PIN_KNOB_LED_10, OUTPUT);
  pinMode(PIN_KNOB_LED_11, OUTPUT);
  pinMode(PIN_KNOB_LED_12, OUTPUT);
  knobLEDArray = random(8);
};

void knobCheckKnobPosition() // function that checks if the knob is in the correct position when it hits -1
{
  //knobPositionValue = analogRead(KNOB_PIN_ROTARY_SWITCH); // uncomment once the knob is wired up. Right now its not wired at all.
  if (knobPositionValue<=KNOB_POSITION_1){
    if (DEBUG_LEVEL >= 1) {
      Serial.println("Position 1");
    }
    knobCurrentState = 0;
  } else if (knobPositionValue<=KNOB_POSITION_2) {
    if (DEBUG_LEVEL >= 1) {
      Serial.println("Position 2");
    }
    knobCurrentState = 1;
  } else if (knobPositionValue<=KNOB_POSITION_3) {
    if (DEBUG_LEVEL >= 1) {
      Serial.println("Position 3");
    }
    knobCurrentState = 2;
  } else if (knobPositionValue<=KNOB_POSITION_4) {
    if (DEBUG_LEVEL >= 1) {
      Serial.println("Position 4");
    }
    knobCurrentState = 3;
  }
}

void knobDisplayTime() // function that displays the time on the clock
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  if (knobSeconds < millis()) {
    knobSeconds += 1000;
    knobSec--;
    if (knobSec == -1) {
      knobCheckKnobPosition();
      if (knobCurrentState==knobCorrectState) { 
        knobSec=KNOB_DEFAULT_TIME + 1; // command to restart the countdown timer
        knobLEDArray = random(8); // choses a new random array of LED's to display
      } else {
        addStrike(); // if the time hits zero and the correct knob position is not set the bomb will add a stike
        knobModuleStike = true;
        knobTimer.print("  ");
        digitalWrite (PIN_KNOB_LED_1, LOW);
        digitalWrite (PIN_KNOB_LED_2, LOW);
        digitalWrite (PIN_KNOB_LED_3, LOW);
        digitalWrite (PIN_KNOB_LED_4, LOW);
        digitalWrite (PIN_KNOB_LED_5, LOW);
        digitalWrite (PIN_KNOB_LED_6, LOW);
        digitalWrite (PIN_KNOB_LED_7, LOW);
        digitalWrite (PIN_KNOB_LED_8, LOW);
        digitalWrite (PIN_KNOB_LED_9, LOW);
        digitalWrite (PIN_KNOB_LED_10, LOW);
        digitalWrite (PIN_KNOB_LED_11, LOW);
        digitalWrite (PIN_KNOB_LED_12, LOW);
      }
    }
  knobTimer.printTime(knobSec, false);
  }
}

void knobDisplayLEDArray()
{
  if (knobLEDArray==0) {
    knobCorrectState=0; // correct position = up
    digitalWrite (PIN_KNOB_LED_1, LOW);
    digitalWrite (PIN_KNOB_LED_2, LOW);
    digitalWrite (PIN_KNOB_LED_3, HIGH);
    digitalWrite (PIN_KNOB_LED_4, LOW);
    digitalWrite (PIN_KNOB_LED_5, HIGH);
    digitalWrite (PIN_KNOB_LED_6, HIGH);
    digitalWrite (PIN_KNOB_LED_7, HIGH);
    digitalWrite (PIN_KNOB_LED_8, HIGH);
    digitalWrite (PIN_KNOB_LED_9, HIGH);
    digitalWrite (PIN_KNOB_LED_10, HIGH);
    digitalWrite (PIN_KNOB_LED_11, LOW);
    digitalWrite (PIN_KNOB_LED_12, HIGH);
  } else if (knobLEDArray==1) {
    knobCorrectState=0; // correct position = up
    digitalWrite (PIN_KNOB_LED_1, HIGH);
    digitalWrite (PIN_KNOB_LED_2, LOW);
    digitalWrite (PIN_KNOB_LED_3, HIGH);
    digitalWrite (PIN_KNOB_LED_4, LOW);
    digitalWrite (PIN_KNOB_LED_5, HIGH);
    digitalWrite (PIN_KNOB_LED_6, LOW);
    digitalWrite (PIN_KNOB_LED_7, LOW);
    digitalWrite (PIN_KNOB_LED_8, HIGH);
    digitalWrite (PIN_KNOB_LED_9, HIGH);
    digitalWrite (PIN_KNOB_LED_10, LOW);
    digitalWrite (PIN_KNOB_LED_11, HIGH);
    digitalWrite (PIN_KNOB_LED_12, HIGH);
  } else if (knobLEDArray==2) {
    knobCorrectState=2; // correct position = down
    digitalWrite (PIN_KNOB_LED_1, LOW);
    digitalWrite (PIN_KNOB_LED_2, HIGH);
    digitalWrite (PIN_KNOB_LED_3, HIGH);
    digitalWrite (PIN_KNOB_LED_4, LOW);
    digitalWrite (PIN_KNOB_LED_5, LOW);
    digitalWrite (PIN_KNOB_LED_6, HIGH);
    digitalWrite (PIN_KNOB_LED_7, HIGH);
    digitalWrite (PIN_KNOB_LED_8, HIGH);
    digitalWrite (PIN_KNOB_LED_9, HIGH);
    digitalWrite (PIN_KNOB_LED_10, HIGH);
    digitalWrite (PIN_KNOB_LED_11, LOW);
    digitalWrite (PIN_KNOB_LED_12, HIGH);
  } else if (knobLEDArray==3) {
    knobCorrectState=2; // correct position = down
    digitalWrite (PIN_KNOB_LED_1, HIGH);
    digitalWrite (PIN_KNOB_LED_2, LOW);
    digitalWrite (PIN_KNOB_LED_3, HIGH);
    digitalWrite (PIN_KNOB_LED_4, LOW);
    digitalWrite (PIN_KNOB_LED_5, HIGH);
    digitalWrite (PIN_KNOB_LED_6, LOW);
    digitalWrite (PIN_KNOB_LED_7, LOW);
    digitalWrite (PIN_KNOB_LED_8, HIGH);
    digitalWrite (PIN_KNOB_LED_9, LOW);
    digitalWrite (PIN_KNOB_LED_10, LOW);
    digitalWrite (PIN_KNOB_LED_11, LOW);
    digitalWrite (PIN_KNOB_LED_12, HIGH);
  } else if (knobLEDArray==4) {
    knobCorrectState=3; // correct position = left
    digitalWrite (PIN_KNOB_LED_1, LOW);
    digitalWrite (PIN_KNOB_LED_2, LOW);
    digitalWrite (PIN_KNOB_LED_3, LOW);
    digitalWrite (PIN_KNOB_LED_4, LOW);
    digitalWrite (PIN_KNOB_LED_5, HIGH);
    digitalWrite (PIN_KNOB_LED_6, LOW);
    digitalWrite (PIN_KNOB_LED_7, HIGH);
    digitalWrite (PIN_KNOB_LED_8, LOW);
    digitalWrite (PIN_KNOB_LED_9, LOW);
    digitalWrite (PIN_KNOB_LED_10, HIGH);
    digitalWrite (PIN_KNOB_LED_11, HIGH);
    digitalWrite (PIN_KNOB_LED_12, HIGH);
  } else if (knobLEDArray==5) {
    knobCorrectState=3; // correct position = left
    digitalWrite (PIN_KNOB_LED_1, LOW);
    digitalWrite (PIN_KNOB_LED_2, LOW);
    digitalWrite (PIN_KNOB_LED_3, LOW);
    digitalWrite (PIN_KNOB_LED_4, LOW);
    digitalWrite (PIN_KNOB_LED_5, HIGH);
    digitalWrite (PIN_KNOB_LED_6, LOW);
    digitalWrite (PIN_KNOB_LED_7, LOW);
    digitalWrite (PIN_KNOB_LED_8, LOW);
    digitalWrite (PIN_KNOB_LED_9, LOW);
    digitalWrite (PIN_KNOB_LED_10, HIGH);
    digitalWrite (PIN_KNOB_LED_11, HIGH);
    digitalWrite (PIN_KNOB_LED_12, LOW);
  } else if (knobLEDArray==6) {
    knobCorrectState=1; // correct position = right
    digitalWrite (PIN_KNOB_LED_1, HIGH);
    digitalWrite (PIN_KNOB_LED_2, LOW);
    digitalWrite (PIN_KNOB_LED_3, HIGH);
    digitalWrite (PIN_KNOB_LED_4, HIGH);
    digitalWrite (PIN_KNOB_LED_5, HIGH);
    digitalWrite (PIN_KNOB_LED_6, HIGH);
    digitalWrite (PIN_KNOB_LED_7, HIGH);
    digitalWrite (PIN_KNOB_LED_8, HIGH);
    digitalWrite (PIN_KNOB_LED_9, HIGH);
    digitalWrite (PIN_KNOB_LED_10, LOW);
    digitalWrite (PIN_KNOB_LED_11, HIGH);
    digitalWrite (PIN_KNOB_LED_12, LOW);
  } else if (knobLEDArray==7) {
    knobCorrectState=1; // correct position = right
    digitalWrite (PIN_KNOB_LED_1, HIGH);
    digitalWrite (PIN_KNOB_LED_2, LOW);
    digitalWrite (PIN_KNOB_LED_3, HIGH);
    digitalWrite (PIN_KNOB_LED_4, HIGH);
    digitalWrite (PIN_KNOB_LED_5, LOW);
    digitalWrite (PIN_KNOB_LED_6, LOW);
    digitalWrite (PIN_KNOB_LED_7, HIGH);
    digitalWrite (PIN_KNOB_LED_8, HIGH);
    digitalWrite (PIN_KNOB_LED_9, HIGH);
    digitalWrite (PIN_KNOB_LED_10, LOW);
    digitalWrite (PIN_KNOB_LED_11, HIGH);
    digitalWrite (PIN_KNOB_LED_12, LOW);
  }
}

void knobLoop()
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }

  if (!defused && !exploded) {
    knobDisplayTime();
    knobDisplayLEDArray();
  }
  /*
     The knob can be turned to one of four different positions.
      The knob must be in the correct position when this module's timer hits zero.
      The correct position can be determined by the on/off configuration of the twelve LEDs.
      Knob positions are relative to the "UP" label, which may be rotated.


    Striking and Deactivating
    Once the knob's position has been set, the module will wait until the timer reaches 00 before checking the answer.
    If it is wrong, it will cause a strike.

  */
}

void knobModuleBoom()
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  // when the bomb explodes all 12 + 1 LEDs should turn off and so should countdown timer above module
  if (explodedFromStrikes) knobTimer.printTime(knobSec, false);
  else knobTimer.print("  ");
  digitalWrite (PIN_KNOB_LED_1, LOW);
  digitalWrite (PIN_KNOB_LED_2, LOW);
  digitalWrite (PIN_KNOB_LED_3, LOW);
  digitalWrite (PIN_KNOB_LED_4, LOW);
  digitalWrite (PIN_KNOB_LED_5, LOW);
  digitalWrite (PIN_KNOB_LED_6, LOW);
  digitalWrite (PIN_KNOB_LED_7, LOW);
  digitalWrite (PIN_KNOB_LED_8, LOW);
  digitalWrite (PIN_KNOB_LED_9, LOW);
  digitalWrite (PIN_KNOB_LED_10, LOW);
  digitalWrite (PIN_KNOB_LED_11, LOW);
  digitalWrite (PIN_KNOB_LED_12, LOW);
}
