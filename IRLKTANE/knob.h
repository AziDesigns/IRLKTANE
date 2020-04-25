// On the Subject of Knobs
/*
  KNOWN ISSUES:
  MODULE IS WIP AND THERE ARE MANY MANY ISSUES & MISSING LOGIC.
*/
/*
   This module consists of a knob that can be turned in four directions (up,down,left,right)
   as well as twelve LEDs that may or may not light up upon the module activation.
   When this needy module activates, some of the LEDs on the bottom will light up,
   and the knob will rotate. The Defuser must communicate the formation of the lit LEDs
   to the Expert who will then tell them which position to set the knob to.
*/
void knobSetup()
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }

}

void knobLoop()
{
  if (DEBUG_LEVEL >= 3) {
    Serial.println (__func__);
  }
  /*
     The knob can be turned to one of four different positions.
      The knob must be in the correct position when this module's timer hits zero.
      The correct position can be determined by the on/off configuration of the twelve LEDs.
      Knob positions are relative to the "UP" label, which may be rotated.



    LED Configurations: (X = Lit LED)
    Up Position:

    0 0 X 0 X X
    X X X X 0 X

    X 0 X 0 X 0
    0 X X 0 X X

    Down Position:

    0 X X 0 0 X
    X X X X 0 X

    X 0 X 0 X 0
    0 X 0 0 0 X

    Left Position:

    0 0 0 0 X 0
    X 0 0 X X X

    0 0 0 0 X 0
    0 0 0 X X 0

    Right Position:

    X 0 X X X X
    X X X 0 X 0

    X 0 X X 0 0
    X X X 0 X 0



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
}
