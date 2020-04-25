// On the Subject of Passwords
/*
  KNOWN ISSUES:
  MODULE IS WIP AND THERE ARE MANY MANY ISSUES & MISSING LOGIC.
*/
/*
  I know there is a lot of repetitive code inside this but when I tried to
  refactor it, everything stopped working so I decided to keep it like this
*/

#define LCD_PASSWORD_CONTRAST 40
#define PIN_PASSWORD_LED_GREEN 25

#define PIN_PASSWORD_BUTTON_1 62 // letter 1 up // invalid pin max number is 53
#define PIN_PASSWORD_BUTTON_2 61 // letter 2 up // invalid pin max number is 53
#define PIN_PASSWORD_BUTTON_3 60 // letter 3 up // invalid pin max number is 53
#define PIN_PASSWORD_BUTTON_4 58 // letter 4 up // invalid pin max number is 53
#define PIN_PASSWORD_BUTTON_5 56 // letter 5 up // invalid pin max number is 53
#define PIN_PASSWORD_BUTTON_6 54 // letter 1 down // invalid pin max number is 53
#define PIN_PASSWORD_BUTTON_7 52 // letter 2 down
#define PIN_PASSWORD_BUTTON_8 59 // letter 3 down // invalid pin max number is 53
#define PIN_PASSWORD_BUTTON_9 57 // letter 4 down // invalid pin max number is 53
#define PIN_PASSWORD_BUTTON_10 53 // letter 5 down

LiquidCrystal lcdPassword(28, 30, 34, 49, 51, 55);  // invalid pin (55) max number is 53
int passWordGen;

void passwordModuleDefusedPrint()
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  passwordModuleDefused = true;
}

void passwordModuleBoom()
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  lcdPassword.clear();
  lcdPassword.setCursor(6, 0);
  lcdPassword.print("BOMB");
  lcdPassword.setCursor(4, 1);
  lcdPassword.print("EXPLODED");
}

void printPassWord() 
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  switch (passWordGen)
  {
    case 1:
      { //ABOUT
        lcdPassword.setCursor(5, 0);
        lcdPassword.print("ABOUT");
      }
      break;
    case 2:
      { //EVERY
        lcdPassword.setCursor(5, 0);
        lcdPassword.print("EVERY");
      }
      break;
    case 3:
      { //LARGE
        lcdPassword.setCursor(5, 0);
        lcdPassword.print("LARGE");
      }
      break;
    case 4:
      { //PLANT
        lcdPassword.setCursor(5, 0);
        lcdPassword.print("PLANT");
      }
      break;
    case 5:
      { //SPELL
        lcdPassword.setCursor(5, 0);
        lcdPassword.print("SPELL");
      }
      break;
    case 6:
      { //THESE
        lcdPassword.setCursor(5, 0);
        lcdPassword.print("THESE");
      }
      break;
    case 7:
      { //WHERE
        lcdPassword.setCursor(5, 0);
        lcdPassword.print("WHERE");
      }
      break;
    case 8:
      { //AFTER
        lcdPassword.setCursor(5, 0);
        lcdPassword.print("AFTER");
      }
      break;
    case 9:
      { //FIRST
        lcdPassword.setCursor(5, 0);
        lcdPassword.print("FIRST");
      }
      break;
    case 10:
      { //LEARN
        lcdPassword.setCursor(5, 0);
        lcdPassword.print("LEARN");
      }
      break;
    case 11:
      { //POINT
        lcdPassword.setCursor(5, 0);
        lcdPassword.print("POINT");
      }
      break;
    case 12:
      { //STILL
        lcdPassword.setCursor(5, 0);
        lcdPassword.print("STILL");
      }
      break;
    case 13:
      { //THING
        lcdPassword.setCursor(5, 0);
        lcdPassword.print("THING");
      }
      break;
    case 14:
      { //WHICH
        lcdPassword.setCursor(5, 0);
        lcdPassword.print("WHICH");
      }
      break;
    case 15:
      { //AGAIN
        lcdPassword.setCursor(5, 0);
        lcdPassword.print("AGAIN");
      }
      break;
    case 16:
      { //FOUND
        lcdPassword.setCursor(5, 0);
        lcdPassword.print("FOUND");
      }
      break;
    case 17:
      { //NEVER
        lcdPassword.setCursor(5, 0);
        lcdPassword.print("NEVER");
      }
      break;
    case 18:
      { //RIGHT
        lcdPassword.setCursor(5, 0);
        lcdPassword.print("RIGHT");
      }
      break;
    case 19:
      { //STUDY
        lcdPassword.setCursor(5, 0);
        lcdPassword.print("STUDY");
      }
      break;
    case 20:
      { //THINK
        lcdPassword.setCursor(5, 0);
        lcdPassword.print("THINK");
      }
      break;
    case 21:
      { //WORLD
        lcdPassword.setCursor(5, 0);
        lcdPassword.print("WORLD");
      }
      break;
    case 22:
      { //BELOW
        lcdPassword.setCursor(5, 0);
        lcdPassword.print("BELOW");
      }
      break;
    case 23:
      { //GREAT
        lcdPassword.setCursor(5, 0);
        lcdPassword.print("GREAT");
      }
      break;
    case 24:
      { //OTHER
        lcdPassword.setCursor(5, 0);
        lcdPassword.print("OTHER");
      }
      break;
    case 25:
      { //SMALL
        lcdPassword.setCursor(5, 0);
        lcdPassword.print("SMALL");
      }
      break;
    case 26:
      { //THEIR
        lcdPassword.setCursor(5, 0);
        lcdPassword.print("THEIR");
      }
      break;
    case 27:
      { //THREE
        lcdPassword.setCursor(5, 0);
        lcdPassword.print("THREE");
      }
      break;
    case 28:
      { //WOULD
        lcdPassword.setCursor(5, 0);
        lcdPassword.print("WOULD");
      }
      break;
    case 29:
      { //COULD
        lcdPassword.setCursor(5, 0);
        lcdPassword.print("COULD");
      }
      break;
    case 30:
      { //HOUSE
        lcdPassword.setCursor(5, 0);
        lcdPassword.print("HOUSE");
      }
      break;
    case 31:
      { //PLACE
        lcdPassword.setCursor(5, 0);
        lcdPassword.print("PLACE");
      }
      break;
    case 32:
      { //SOUND
        lcdPassword.setCursor(5, 0);
        lcdPassword.print("SOUND");
      }
      break;
    case 33:
      { //THERE
        lcdPassword.setCursor(5, 0);
        lcdPassword.print("THERE");
      }
      break;
    case 34:
      { //WATER
        lcdPassword.setCursor(5, 0);
        lcdPassword.print("WATER");
      }
      break;
    case 35:
      { //WRITE
        lcdPassword.setCursor(5, 0);
        lcdPassword.print("WRITE");
      }
      break;
  }
}

void printPasswordModuleDefused()
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  passwordModuleDefusedPrint();
  lcdPassword.clear();
  lcdPassword.setCursor(1, 0);
  lcdPassword.print("MODULE DEFUSED");
  lcdPassword.setCursor(0, 1);
  passwordModuleDefused = true;
}

void passwordSetup()
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  pinMode(PIN_PASSWORD_LED_GREEN, OUTPUT);
  pinMode(PIN_PASSWORD_BUTTON_1, INPUT);
  pinMode(PIN_PASSWORD_BUTTON_2, INPUT);
  pinMode(PIN_PASSWORD_BUTTON_3, INPUT);
  pinMode(PIN_PASSWORD_BUTTON_4, INPUT);
  pinMode(PIN_PASSWORD_BUTTON_5, INPUT);
  pinMode(PIN_PASSWORD_BUTTON_6, INPUT);
  pinMode(PIN_PASSWORD_BUTTON_7, INPUT);
  pinMode(PIN_PASSWORD_BUTTON_8, INPUT);
  pinMode(PIN_PASSWORD_BUTTON_9, INPUT);
  pinMode(PIN_PASSWORD_BUTTON_10, INPUT);

  analogWrite(V0_PIN, LCD_PASSWORD_CONTRAST);
  lcdPassword.begin(16, 2);

  //generating a seed to use in order to generate random numbers
  randomSeed(analogRead(0));

  printPassWord();
  lcdPassword.setCursor(0, 1);
  if (DEBUG_LEVEL >= 1) {
    Serial.println("Correct Password Generated: ");
    Serial.println(passWordGen);
  }
}


void passwordLoop()
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  if (!passwordModuleDefused) {
    //logic to display random word goes here?
  }
}
