// On the Subject of Simon Says
/*
  KNOWN ISSUES:
  MODULE IS MOSTLY DONE BUT HAS NOT BEEN TESTED AT ALL.
  NEEDS COMPARED TO ACTUAL GAME MANUAL FOR ACCURACY.
*/

#define RED_LED_PIN 45
#define GREEN_LED_PIN 48
#define YELLOW_LED_PIN 47
#define BLUE_LED_PIN 46
#define RED_BTN_PIN A1
#define GREEN_BTN_PIN A4
#define YELLOW_BTN_PIN A3
#define BLUE_BTN_PIN A2
#define PIN_SIMON_LED_GREEN 49

unsigned long lastDebounceTimeRed = 0;
unsigned long lastDebounceTimeGreen = 0;
unsigned long lastDebounceTimeYellow = 0;
unsigned long lastDebounceTimeBlue = 0;
unsigned long debounceDelay = 50;

unsigned long currentMillisRed = 0;
unsigned long currentMillisGreen = 0;
unsigned long currentMillisYellow = 0;
unsigned long currentMillisBlue = 0;

int redLedState = 0;
int greenLedState = 0;
int yellowLedState = 0;
int blueLedState = 0;

int ledPins[4] = {RED_LED_PIN, GREEN_LED_PIN, YELLOW_LED_PIN, BLUE_LED_PIN};
int ledStates[4] = {0, 0, 0, 0};

int ledsNumber = 1;
int blinkingTime = 500;
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
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(YELLOW_LED_PIN, LOW);
  digitalWrite(BLUE_LED_PIN, LOW);
}

// function that sets the module as being defused
void simonModuleDefusedPrint()
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  simonModuleDefused = true;
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(YELLOW_LED_PIN, LOW);
  digitalWrite(BLUE_LED_PIN, LOW);
  digitalWrite(PIN_SIMON_LED_GREEN, HIGH);
  defusedModuleBuzzer();
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
void pressButton(int ledNr, int btnPin, int led, unsigned long &debounceTime, unsigned long &currentMillis, int &ledState)
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  int reading = digitalRead(btnPin);

  if (reading == HIGH) {
    debounceTime = millis();
    ledState = 1;
  }

  if (millis() - debounceTime > debounceDelay) {
    if (ledState == 1) {
      currentLed = ledsNumber;
      digitalWrite(RED_LED_PIN, LOW);
      digitalWrite(GREEN_LED_PIN, LOW);
      digitalWrite(YELLOW_LED_PIN, LOW);
      digitalWrite(BLUE_LED_PIN, LOW);
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
      ledState = 2;
    }

    if (millis() - currentMillis >= blinkingTime && ledState == 2) {
      digitalWrite(led, LOW);
      ledState = 0;
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
void blinkLed(int led, int &ledState)
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  unsigned long currentMillis = millis();

  if (currentMillis - simonPreviousMillis >= blinkingTime) {
    simonPreviousMillis = currentMillis;

    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
      currentLed++;
    }

    digitalWrite(led, ledState);
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
    blinkLed(ledPins[ledSequence[currentLed] - 1], ledStates[ledSequence[currentLed] - 1]);
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
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(PIN_SIMON_LED_GREEN, OUTPUT);
  pinMode(RED_BTN_PIN, INPUT);
  pinMode(GREEN_BTN_PIN, INPUT);
  pinMode(YELLOW_BTN_PIN, INPUT);
  pinMode(BLUE_BTN_PIN, INPUT);

  generateLedSequence();
}

void simonLoop()
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  if (!simonModuleDefused) {
    ledAnimation();
    pressButton(1, RED_BTN_PIN, RED_LED_PIN, lastDebounceTimeRed, currentMillisRed, redLedState);
    pressButton(2, GREEN_BTN_PIN, GREEN_LED_PIN, lastDebounceTimeGreen, currentMillisGreen, greenLedState);
    pressButton(3, YELLOW_BTN_PIN, YELLOW_LED_PIN, lastDebounceTimeYellow, currentMillisYellow, yellowLedState);
    pressButton(4, BLUE_BTN_PIN, BLUE_LED_PIN, lastDebounceTimeBlue, currentMillisBlue, blueLedState);

  }

}
