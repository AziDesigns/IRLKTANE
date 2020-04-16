// On the Subject of Capacitor Discharge

#define PIN_DISCHARGE_YELLOW_LED 7 // Yellow LED for showing active discharge occuring
#define PIN_DISCHARGE_LEVER 8 // lever that is pressed to discharge the module
#define PIN_DISCHARGE_CLK 14 // discharge display CLK
#define PIN_DISCHARGE_DIO 15 // discharge display DIO
#define DISCHARGE_TIMER_SECONDS 30 // default countdown between discharges
#define LEVER_PRESSED 1 // unknown value will be generated by lever when pressed will populate upon testing
#define LEVER_RELEASED 0 // unknown value will be generated by lever when not pressed will populate upon testing

int disSec = DISCHARGE_TIMER_SECONDS; // starting seconds remaining before timer expires
int leverPressed = PIN_DISCHARGE_LEVER; // set lever value to pin input
SevenSegmentExtended dischargeTimer(PIN_DISCHARGE_CLK, PIN_DISCHARGE_DIO);

void dischargeSetup() //turn on lights, display
{
pinMode(PIN_DISCHARGE_YELLOW_LED,OUTPUT);
dischargeTimer.begin();
dischargeTimer.setBacklight(100);
dischargeTimer.print(disSec);
}

void dischargeLoop() //loop to decrease time or increase time on timer.
{
  if (disSec == -1) {
    bombExploded(); // if the time hits zero, the bomb will go off
    digitalWrite(PIN_DISCHARGE_YELLOW_LED, LOW); //TURN DISCHARGE LED OFF
  }
  
  else if ((leverPressed == LEVER_PRESSED) && (disSec < DISCHARGE_TIMER_SECONDS)) {
    disSec = disSec + 1; //if lever is pressed add time back
    digitalWrite(PIN_DISCHARGE_YELLOW_LED, HIGH); //TURN DISCHARGE LED ON
    dischargeTimer.print(disSec);
    delay(1000);
  }
  
  else if ((leverPressed == LEVER_PRESSED) && (disSec = DISCHARGE_TIMER_SECONDS)) {
    digitalWrite(PIN_DISCHARGE_YELLOW_LED, HIGH); //TURN DISCHARGE LED ON
    dischargeTimer.print(disSec);
    delay(1000);
  }
  
  else if (leverPressed == LEVER_RELEASED) {
    disSec = disSec - 1; //if lever is not pressed reduce seconds by 1
    digitalWrite(PIN_DISCHARGE_YELLOW_LED, LOW); //TURN DISCHARGE LED OFF
    dischargeTimer.print(disSec);
    delay(1000);
  }
}

void dischargeModuleBoom() // if the bomb explodes what should the module display
{
  dischargeTimer.print("00");
}
