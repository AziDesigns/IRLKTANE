#include "pitches.h"
#define buzzer 2

// change this to make the song slower or faster
byte tempo = 108;

void buzzerSetup()
{
  pinMode(buzzer, OUTPUT);
}

// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
int victoryMelody[] = {
  
  NOTE_C5,32, NOTE_F5,32, NOTE_G5,32, NOTE_C6,32, 
  NOTE_F6,32, NOTE_G6,32, NOTE_C7,32,
  REST,4,
  NOTE_D4,16, NOTE_C4,16, NOTE_D4,16, NOTE_F4,16, REST,8, 
  NOTE_G4,16, NOTE_F4,16, NOTE_G4,16, NOTE_A4,16, REST,8, 
  NOTE_B4,16, REST,16, NOTE_C5,16, REST,16, NOTE_D5,2,
  
};

int boomMelody[] = {

  NOTE_C4,32, NOTE_CS4,32, NOTE_D4,16, REST,16, REST,8,
  NOTE_B3,16, NOTE_F4,16, REST,16, NOTE_F4,16, 
  NOTE_F4,-16, NOTE_E4,-16, NOTE_D4,16, 
  NOTE_C4,16, NOTE_E3,16, REST,16, 
  NOTE_E3,16, NOTE_C3,16, REST,16, REST,8,

  NOTE_C4,16, REST,8, NOTE_G3,16, 
  REST,8, NOTE_E3,8, 
  NOTE_A3,-16, NOTE_B3,-16, NOTE_A3,16,
  NOTE_GS3,8, NOTE_AS3,8,
  NOTE_GS3,8, NOTE_E3,16, NOTE_D3,16,
  NOTE_E3,2,
  
};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
byte victoryNotes = sizeof(victoryMelody) / sizeof(victoryMelody[0]) / 2;
byte boomNotes = sizeof(boomMelody) / sizeof(boomMelody[0]) / 2;

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
  for (byte thisNote = 0; thisNote < victoryNotes * 2; thisNote = thisNote + 2) {

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

void boomBuzzer() 
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  // iterate over the notes of the melody. 
  // Remember, the array is twice the number of notes (notes + durations)
  for (byte thisNote = 0; thisNote < boomNotes * 2; thisNote = thisNote + 2) {

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

// PLAY TONES WITHOUT DELAY =================================
// Pulse the speaker to play a tone for a particular duration
// Only needed for sounds that occur during the gameplay

// MELODY and TIMING  =======================================
//  melody[] is an array of notes, accompanied by beats[],
//  which sets each note's relative length (higher #, longer note)
int countdownMelody[] = {  NOTE_CS7,  NOTE_AS6 };
byte countdownBeats[]  = { 8, 4 };
byte countdownMAX_COUNT = sizeof(countdownMelody) / 2; // Melody length, for looping.

int defusedModuleMelody[] = {  NOTE_C5, NOTE_C6 };
byte defusedModuleBeats[]  = { 8, 16 };
byte defusedModuleMAX_COUNT = sizeof(defusedModuleMelody) / 2; // Melody length, for looping.

int strikeMelody[] = {  NOTE_B3, NOTE_C4, NOTE_B3 };
byte strikeBeats[]  = { 4, 2, 4 };
byte strikeMAX_COUNT = sizeof(strikeMelody) / 2; // Melody length, for looping.

// Set overall tempo
const int noDelayTempo = 10000;
// Set length of pause between notes
const int pause = 1000;
// Loop variable to increase Rest length
byte rest_count = 100; //<-BLETCHEROUS HACK; See NOTES

// Initialize core variables
int tone_ = 0;
byte beat = 0;
long duration  = 0;

void playTone() {
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  long elapsed_time = 0;
  if (tone_ > 0) { // if this isn't a Rest beat, while the tone has
    //  played less long than 'duration', pulse speaker HIGH and LOW
    while (elapsed_time < duration) {

      digitalWrite(buzzer,HIGH);
      delayMicroseconds(tone_ / 2);

      // DOWN
      digitalWrite(buzzer, LOW);
      delayMicroseconds(tone_ / 2);

      // Keep track of how long we pulsed
      elapsed_time += (tone_);
    }
  }
  else { // Rest beat; loop times delay
    for (int j = 0; j < rest_count; j++) { // See NOTE on rest_count
      delayMicroseconds(duration);  
    }                                
  }                                
}

void countdownBuzzer() 
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  // Set up a counter to pull from melody[] and beats[]
  for (byte i=0; i<countdownMAX_COUNT; i++) {
    tone_ = (1000000/countdownMelody[i]);
    beat = countdownBeats[i];
    duration = beat * noDelayTempo; // Set up timing
    playTone();
    // A pause between notes...
    delayMicroseconds(pause);
  }
}

void defusedModuleBuzzer() 
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  // Set up a counter to pull from melody[] and beats[]
  for (byte i=0; i<defusedModuleMAX_COUNT; i++) {
    tone_ = (1000000/defusedModuleMelody[i]);
    beat = defusedModuleBeats[i];
    duration = beat * noDelayTempo; // Set up timing
    playTone();
    // A pause between notes...
    delayMicroseconds(pause);
  }
}

void strikeBuzzer() 
{
  if (DEBUG_LEVEL >= 2) {
    Serial.println (__func__);
  }
  if (exploded==false) {
  // Set up a counter to pull from melody[] and beats[]
    for (byte i=0; i<strikeMAX_COUNT; i++) {
      tone_ = (1000000/strikeMelody[i]);
      beat = strikeBeats[i];
      duration = beat * noDelayTempo; // Set up timing
      playTone();
      // A pause between notes...
      delayMicroseconds(pause);
    }
  }
}
