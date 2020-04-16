// On the Subject of Morse Code
/*
 * The Morse Code module is a module that consists of a light flashing in Morse Code, a radio with a displayed frequency and a TX button. 
 * The defuser must interpret the flashing Morse Light as dots and dashes to form a word in Morse Code. 
 * This word corresponds to a radio frequency that the expert must tell the defuser to transmit. 
 * The defuser must scroll to that frequency, and press the TX button to solve the module.
 */
void morseSetup()
{
  
}

void morseLoop()
{
  // THIS NEEDS TO GO SOMEWHERE
  morseModuleDefused = true;


/*
 * Interpret the signal from the flashing light (the flashing should start as soon as the bomb starts countdown) 
 * using the Morse Code chart to spell one of the words on the table.
 * The signal will loop, with a long gap between repetitions. (timing between letters and words will be important to get right)
 * Once the word is identified, set the corresponding frequency and press the transmit (TX) button.

If the word is:   Respond at frequency:
shell             3.505 MHz
halls             3.515 MHz
slick             3.522 MHz
trick             3.532 MHz
boxes             3.535 MHz
leaks             3.542 MHz
strobe            3.545 MHz
bistro            3.552 MHz
flick             3.555 MHz
bombs             3.565 MHz
break             3.572 MHz
brick             3.582 MHz
sting             3.592 MHz
vector            3.595 MHz
beats             3.600 MHz


 * A strike will be recorded if:
 * The TX button is pressed with an incorrect frequency set.

 * The module will be disarmed when:
 * The TX button is pressed with the correct frequency set.
 */
  
}

void morseModuleBoom()
{
  // if the bomb explodes the display with the frequency should clear and the flashing led should stop
}
