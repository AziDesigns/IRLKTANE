// On the Subject of Simon Says
/*
  KNOWN ISSUES:
*/

#define PIN_SIMON_LED_RED 45
#define PIN_SIMON_LED_GREEN 48
#define PIN_SIMON_LED_YELLOW 47
#define PIN_SIMON_LED_BLUE 46
#define PIN_SIMON_BTN_RED A1
#define PIN_SIMON_BTN_GREEN A4
#define PIN_SIMON_BTN_YELLOW A3
#define PIN_SIMON_BTN_BLUE A2
#define PIN_SIMON_LED_FIN 49

unsigned long lastDebounceTimeRed = 0;
unsigned long lastDebounceTimeGreen = 0;
unsigned long lastDebounceTimeYellow = 0;
unsigned long lastDebounceTimeBlue = 0;
unsigned long debounceDelay = 50;

unsigned long currentMillisRed = 0;
unsigned long currentMillisGreen = 0;
unsigned long currentMillisYellow = 0;
unsigned long currentMillisBlue = 0;

int simonRedLedState = 0;
int simonGreenLedState = 0;
int simonYellowLedState = 0;
int simonBlueLedState = 0;

int simonLedPins[4] = {PIN_SIMON_LED_RED, PIN_SIMON_LED_GREEN, PIN_SIMON_LED_YELLOW, PIN_SIMON_LED_BLUE};
int simonLedStates[4] = {0, 0, 0, 0};

int ledsNumber = 1;
int simonBlinkingTime = 500;
unsigned long simonPreviousMillis = 0;
int ledSequence[4];

// array that contains the answers when the serial number contains a vowel
int answersWithVowel[3][4] = {{4, 3, 2, 1},
  {3, 4, 1, 2},
  {2, 3, 4, 1}
};

// array that contains the answers when the serial number doesn't contain a vowel
int answersWithoutVowel[3][4] = {{4, 2, 1, 3},
  {1, 3, 2, 4},
  {3, 4, 1, 2}
};

int currentLed = 0, animationDelay = 5000;
unsigned long animationMillis = 0, beforeAnimationMillis;
int buttonsPressed = 0;

int beforeAnimationDelay = 1000;

// function that shuts down the leds after the module is finished
void simonModuleBoom()
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  digitalWrite(PIN_SIMON_LED_RED, LOW);
  digitalWrite(PIN_SIMON_LED_GREEN, LOW);
  digitalWrite(PIN_SIMON_LED_YELLOW, LOW);
  digitalWrite(PIN_SIMON_LED_BLUE, LOW);
  digitalWrite(PIN_SIMON_LED_FIN, LOW);
}

// function that sets the module as being defused
void simonModuleDefusedPrint()
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  simonModuleDefused = true;
  digitalWrite(PIN_SIMON_LED_RED, LOW);
  digitalWrite(PIN_SIMON_LED_GREEN, LOW);
  digitalWrite(PIN_SIMON_LED_YELLOW, LOW);
  digitalWrite(PIN_SIMON_LED_BLUE, LOW);
  digitalWrite(PIN_SIMON_LED_FIN, HIGH);
  defusedModuleBuzzer();
  isAnyModuleDefused=true;
}

// function that checks if there's a vowel in the serial code and returns an answer accordingly
bool checkForVowel()
{
  for (int i = 0; i < 7; i++)
    if (serialCode[i] == 'A' || serialCode[i] == 'E' || serialCode[i] == 'I' || serialCode[i] == 'O' || serialCode[i] == 'U')
      return 1;
  return 0;
}

// function that checks if the pressed button is the correct one in the answer sequence and
// turns on the specific led
void pressButton(int ledNr, int btnPin, int led, unsigned long &debounceTime, unsigned long &currentMillis, int &simonLedState)
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  int reading = digitalRead(btnPin);

  if (reading == HIGH) {
    debounceTime = millis();
    simonLedState = 1;
  }

  if (millis() - debounceTime > debounceDelay) {
    if (simonLedState == 1) {
      currentLed = ledsNumber;
      digitalWrite(PIN_SIMON_LED_RED, LOW);
      digitalWrite(PIN_SIMON_LED_GREEN, LOW);
      digitalWrite(PIN_SIMON_LED_YELLOW, LOW);
      digitalWrite(PIN_SIMON_LED_BLUE, LOW);
      buttonsPressed++;
      if (checkForVowel()) {
        if (answersWithVowel[nrStrikes][ledSequence[buttonsPressed - 1] - 1] == ledNr) {
          if (buttonsPressed == ledsNumber) {
            buttonsPressed = 0;
            ledsNumber++;
            beforeAnimationMillis = millis();
            currentLed = -1;
            if (ledsNumber > 4) {
              simonModuleDefusedPrint();
              currentLed = ledsNumber + 2;
            }
          }
        } else {
          addStrike();
          if (nrStrikes < 3) {
            currentLed = -1;
            beforeAnimationMillis = millis();
            buttonsPressed = 0;
          }
        }
      } else {
        if (answersWithoutVowel[nrStrikes][ledSequence[buttonsPressed - 1] - 1] == ledNr) {
          if (buttonsPressed == ledsNumber) {
            buttonsPressed = 0;
            ledsNumber++;
            currentLed = -1;
            beforeAnimationMillis = millis();
            if (ledsNumber > 4) {
              simonModuleDefusedPrint();
              currentLed = ledsNumber + 2;
            }
          }
        } else {
          addStrike();
          if ( nrStrikes < 3) {
            currentLed = -1;
            beforeAnimationMillis = millis();
            buttonsPressed = 0;
          }
        }
      }

      digitalWrite(led, HIGH);
      currentMillis = millis();
      simonLedState = 2;
    }

    if (millis() - currentMillis >= simonBlinkingTime && simonLedState == 2) {
      digitalWrite(led, LOW);
      simonLedState = 0;
    }
  }
}

// function that generates the led sequence that needs to be resolved
void generateLedSequence()
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  for (int i = 0; i < 4; i++)
  {
    int x = random(1, 5);
    ledSequence[i] = x;
  }
}

// function that makes a led blink when a button is pressed
void blinkLed(int led, int &simonLedState)
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  unsigned long currentMillis = millis();

  if (currentMillis - simonPreviousMillis >= simonBlinkingTime) {
    simonPreviousMillis = currentMillis;

    if (simonLedState == LOW) {
      simonLedState = HIGH;
    } else {
      simonLedState = LOW;
      currentLed++;
    }

    digitalWrite(led, simonLedState);
  }
}

// function that makes the leds blink in the order dictated by the led sequence that needs to be solved at the current stage
void ledAnimation()
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  if (currentLed == -1 && millis() - beforeAnimationMillis > beforeAnimationDelay)
    currentLed = 0;

  if (currentLed == 0)
    buttonsPressed = 0;

  if (currentLed < ledsNumber)
    blinkLed(simonLedPins[ledSequence[currentLed] - 1], simonLedStates[ledSequence[currentLed] - 1]);
  else {
    if (currentLed == ledsNumber) {
      animationMillis = millis();
      currentLed++;
    }
    if (currentLed == ledsNumber + 1 && millis() - animationMillis > animationDelay)
      currentLed = 0;
  }
}

void simonSetup()
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  pinMode(PIN_SIMON_LED_RED, OUTPUT);
  pinMode(PIN_SIMON_LED_GREEN, OUTPUT);
  pinMode(PIN_SIMON_LED_YELLOW, OUTPUT);
  pinMode(PIN_SIMON_LED_BLUE, OUTPUT);
  pinMode(PIN_SIMON_LED_FIN, OUTPUT);
  pinMode(PIN_SIMON_BTN_RED, INPUT);
  pinMode(PIN_SIMON_BTN_GREEN, INPUT);
  pinMode(PIN_SIMON_BTN_YELLOW, INPUT);
  pinMode(PIN_SIMON_BTN_BLUE, INPUT);

  generateLedSequence();
}

void simonLoop()
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  if (!simonModuleDefused) {
    ledAnimation();
    pressButton(1, PIN_SIMON_BTN_RED, PIN_SIMON_LED_RED, lastDebounceTimeRed, currentMillisRed, simonRedLedState);
    pressButton(2, PIN_SIMON_BTN_GREEN, PIN_SIMON_LED_GREEN, lastDebounceTimeGreen, currentMillisGreen, simonGreenLedState);
    pressButton(3, PIN_SIMON_BTN_YELLOW, PIN_SIMON_LED_YELLOW, lastDebounceTimeYellow, currentMillisYellow, simonYellowLedState);
    pressButton(4, PIN_SIMON_BTN_BLUE, PIN_SIMON_LED_BLUE, lastDebounceTimeBlue, currentMillisBlue, simonBlueLedState);
  }
}
