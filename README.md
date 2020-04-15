# IRLKTANE
A real life (but not really real) version of the game Keep Talking and Nobody Explodes
Built for - Arduino

Inspired by one of my favorite games (KTaNE) and the realazation that this could be made into a physical version after seeing a reddit post by u/advicevice of the "maze" module I decided I had to build it myself.

The goal is not to make hot swappable modules but rather 1 of each module that works. 
Some modules are obisouly not as realistic (ex. wire sequences) if I want it to be EXACTLY like the game but I am willing to make small adjustments to make it follow the same original game rules. 

*I do not plan to make any modded modules at this time but may in the future if all other moduels are completed and optimized.

The game is played with a minumum of 2 players, with one player as the "Defuser" and the remaining player(s) as the "Experts" reading the offical bomb defusal manual from KTaNE. I am not going to cover all game rules here, just go look at the manual on their site.

Currently it is being built with fixed variables but the goal will be once all modules are working as intended, you can create/ custimize the difficulty by turning specific modules on/off as well as adjusting things like # of strikes (3 by default) or starting time (7 minutes by default).
*Fixed Variables include things like ports, batteries, & lit indicators.

Table of Contents
1. Getting Started
- 1.1. Prerequisites
- 1.2. Installation
2. Components
3. Built With
4. Authors
5. Credits

1. Getting Started
These instructions will get you a copy of the project up and running.

1.1 Prerequisites
In order for the code to work, you will need the following libraries:

    SevenSegmentTM1637 - link
    LedControl - link
    LiquidCrystal - link
    LiquidCrystal_I2C - link
    The files in this repository

1.2 Installation
Download the .zip files from the links provided
Open Arduino IDE
Click Sketch -> Include Library -> Add .ZIP Library...
Select the library
Repeat steps 3 and 4 for each library
Download the files in this repository and put all of them in the same folder (folder name must match .ino file name)

A Fritzing scheme will be added to this repository with instructions to build the circuit once each module is tested.

Next, connect the Arduino board to your computer and load the program to the board.

2. Components
2 - Neopixel Stick
1 - Red 7-segment clock display
3 - 7 Segment Display (2-Digit)
1 - Adafruit NeoPixel NeoMatrix 8x8 - 64 RGB LED Pixel Matrix
27 - Green LED
2 - Yellow LED
1 - 7 Segment Display (Single Digit)
2 - 7 Segment Display - 20mm (4 Digit)
1 - Red Square LED Button
1 - Blue Square LED Button
1 - Yellow Square LED Button
1 - Green Square LED Button
1 - 5-way Navigation switch
1 - Snap-Action Standard Micro Switch with Lever
30 - Mini Pushbutton Switch
1 - RGB Button
3 - LCD Display 16x2 I2C
1 - LCD Display 20x4 I2C
1 - large fake capacitor //for the needy module aesthetics
Many Resistors, Capacitors, and Jumpers

3. Built With
Arduino - The IDE used to code the modules logic.
Fritzing - The program used to create the components layout and wiring diagram.

4. Authors
AziAssassin

5. Credits
Thanks to calingeorgeadrian && ema1ih for setting the ground work for me to learn and get the basics of how they wrote their version of a select few of these modules.
Thanks to reddit u/advicevice for making me realize the potential this idea had to become a phyical game.
Last but not least: Keep Talking and Nobody Explodes - the game that inspired me in the first place.
