#include "pitches.h"

// change this to make the song slower or faster
int tempo = 108;
int strikeMelodyDuration = 100;
int countdownMelodyDuration = 50;
int defusedModuleMelodyDuration = 200;

// change this to whichever pin you want to use
int buzzer = 2;

// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
int victoryMelody[] = {

  NOTE_D4,16, NOTE_C4,16, NOTE_D4,16, NOTE_F4,16, REST,8, 
  NOTE_G4,16, NOTE_F4,16, NOTE_G4,16, NOTE_A4,16, REST,8, 
  NOTE_B4,16, REST,16, NOTE_C5,16, REST,16, NOTE_D5,2,
  
};

int countdownMelody[] = {

  NOTE_C7,32, NOTE_A6,32,
  
};

int defusedModuleMelody[] = {

  NOTE_C5,32, NOTE_C6,32,
  
};

int strikeMelody[] = {

  NOTE_GS5,32, NOTE_DS4,32,
  
};

int boomMelody[] = {

  NOTE_GS5,16, NOTE_C5,16, NOTE_DS4,2,
  
};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int victoryNotes = sizeof(victoryMelody) / sizeof(victoryMelody[0]) / 2;
int countdownNotes = sizeof(countdownMelody) / sizeof(countdownMelody[0]) / 2;
int defusedModuleNotes = sizeof(defusedModuleMelody) / sizeof(defusedModuleMelody[0]) / 2;
int strikeNotes = sizeof(strikeMelody) / sizeof(strikeMelody[0]) / 2;
int boomNotes = sizeof(boomMelody) / sizeof(boomMelody[0]) / 2;

// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;

void victoryBuzzer() 
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  // iterate over the notes of the melody. 
  // Remember, the array is twice the number of notes (notes + durations)
  for (int thisNote = 0; thisNote < victoryNotes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = victoryMelody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer, victoryMelody[thisNote], noteDuration*0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);
    
    // stop the waveform generation before the next note.
    noTone(buzzer);
  }
  victorySong=true;
}

void countdownBuzzer() 
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  tone(buzzer, countdownMelody[2], countdownMelodyDuration);
}

void defusedModuleBuzzer() 
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  for (int thisNote = 0; thisNote < defusedModuleNotes * 2; thisNote = thisNote + 2) {
    tone(buzzer, defusedModuleMelody[thisNote], defusedModuleMelodyDuration);
  }
}

void strikeBuzzer() 
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  if (exploded==false) {
    for (int thisNote = 0; thisNote < strikeNotes * 2; thisNote = thisNote + 2) {
      tone(buzzer, strikeMelody[thisNote], strikeMelodyDuration);
    }
  }
}

void boomBuzzer() 
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  // iterate over the notes of the melody. 
  // Remember, the array is twice the number of notes (notes + durations)
  for (int thisNote = 0; thisNote < boomNotes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = boomMelody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer, boomMelody[thisNote], noteDuration*0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);
    
    // stop the waveform generation before the next note.
    noTone(buzzer);
  }
}
