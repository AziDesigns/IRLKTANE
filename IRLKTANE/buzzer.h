// The buzzer sounds for bomb diffused, bomb explosion, module diffused, and strike added.
/*
  KNOWN ISSUES:
  PITCHES ALL PLAY FOR FIXED AMOUNT OF TIME AND DO NOT REALLY CREATE A TUNE.
  1. UPDATE FOR TIMING WITHOUT DELAY()
  2. VERIFY NOTES SELECTED MATCH KTANE AS CLOSE AS POSSILBE. (SOME DO NOT EXIST)
  THE BOMB EXPLODED SOUND PLAYS DIFFERENTLY WHEN ITS EXPLODED FROM STRIKES VS WHEN IT EXPLODES FROM TIME? WHY?
*/
#include "pitches.h"

#define BUZZER 2

// notes in the melodies:
int countdownMelody[] = {NOTE_C7, NOTE_A6};
int defusedModuleMelody[] = {NOTE_C5, NOTE_C6};
int victoryMelody[] = {NOTE_C6, NOTE_DS5, NOTE_C6};
int strikeMelody[] = {NOTE_GS5, NOTE_DS4};
int boomMelody[] = {NOTE_GS5, NOTE_C5, NOTE_DS4};

// how long each note will be played
int countdownMelodyDuration = 50;
int defusedModuleMelodyDuration = 200;
int victoryMelodyDuration = 300;
int strikeMelodyDuration = 100;
int boomMelodyDuration = 500;

int thisNote = 0;

void countdownBuzzer()
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  //tone(BUZZER, victoryMelody[0], countdownMelodyDuration);
  tone(BUZZER, victoryMelody[1], countdownMelodyDuration);
}
void victoryBuzzer() // the sound of the buzzer when the team wins
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  for (thisNote = 0; thisNote < 3; thisNote++) {
    tone(BUZZER, victoryMelody[thisNote], victoryMelodyDuration);
    delay(500);
  }
  /*
    delay(700);
    tone(BUZZER, 1000);
    delay(250);
    noTone(BUZZER);
    delay(50);
    tone(BUZZER, 600);
    delay(250);
    noTone(BUZZER);
    delay(50);
    tone(BUZZER, 1000);
    delay(1000);
    noTone(BUZZER);
  */
}

void boomBuzzer() // the sound of the buzzer when the team loses
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  for (thisNote = 0; thisNote < 3; thisNote++) {
    tone(BUZZER, boomMelody[thisNote], boomMelodyDuration);
    delay(1000);
  }
  /*
    delay(700);
    for (int i = 0; i < 4; i++) {
    tone(BUZZER, 800);
    delay(300);
    noTone(BUZZER);
    delay(100);

    tone(BUZZER, 500);
    delay(300);
    noTone(BUZZER);

    tone(BUZZER, 300);
    delay(1000);
    noTone(BUZZER);
    }
  */
}

void defusedModuleBuzzer() // the sound of the buzzer when a module is defused
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  for (thisNote = 0; thisNote < 2; thisNote++) {
    tone(BUZZER, defusedModuleMelody[thisNote], defusedModuleMelodyDuration);
  }
  /*
    tone(BUZZER, 500);
    delay(300);
    noTone(BUZZER);
    delay(100);

    tone(BUZZER, 1000);
    delay(300);
    noTone(BUZZER);
  */
}

void strikeBuzzer() // the sound of the buzzer when the team makes a mistake
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  for (thisNote = 0; thisNote < 2; thisNote++) {
    tone(BUZZER, strikeMelody[thisNote], strikeMelodyDuration);
  }
  /*
    tone(BUZZER, 800);
    delay(300);
    noTone(BUZZER);
    delay(100);

    tone(BUZZER, 300);
    delay(300);
    noTone(BUZZER);
  */

}
