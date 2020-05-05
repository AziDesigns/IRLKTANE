# IRLKTANE
A real life (but not really real) version of the game Keep Talking and Nobody Explodes
Built for - Arduino

Inspired by one of my favorite games (KTaNE) and the realization that this could be made into a physical version after seeing a reddit post by u/advicevice of the "maze" module I decided I had to build it myself.

The goal is not to make hot swappable modules but rather 1 of each module that works. 
Some modules are obviously not as realistic (ex. wire sequences) if I want it to be EXACTLY like the game but I am willing to make small adjustments to make it follow the same original game rules. 

*I do not plan to make any moded modules at this time but may in the future if all other modules are completed and optimized.*

The game is played with a minimum of 2 players, with one player as the "Defuser" and the remaining player(s) as the "Experts" reading the official bomb defusal manual from KTaNE VERSION 1 - VERIFICATION CODE #241 ONLY. I am not going to cover all game rules here, just go look at the manual on their site.

Currently it is being built with fixed variables but the goal will be once all modules are working as intended, you can create/ customize the difficulty by turning specific modules on/off as well as adjusting things like # of strikes (3 by default) or starting time (7 minutes by default).
*Fixed Variables include things like ports, batteries, & lit indicators.*

There will NOT be any 3d models released as I plan to make this out of wood, sheet metal, and plexiglass cutouts to hide/show the batteries, ports, etc.

#### Table of Contents
1. Getting Started w/ Software
- 1.1. Needed Libraries/ Files
- 1.2. Installation
2. Components
3. Modules
4. Built With
5. Authors
6. Credits

#### 1. Getting Started
These instructions will get you a copy of the project up and running.

#### 1.1 Prerequisites
In order for the code to work, you will need the following libraries:

- SevenSegmentTM1637 - [link](https://github.com/bremme/arduino-tm1637) 
- LedControl - [link](https://github.com/wayoda/LedControl)
- LiquidCrystal - [link](https://www.arduinolibraries.info/libraries/liquid-crystal)
- LiquidCrystal_I2C - [link](https://www.arduinolibraries.info/libraries/liquid-crystal-i2-c?fbclid=IwAR016eH4yHLd4HKqA-OcUghdO19ukQlUcGXE2mY0AUaA8aWecJZJvaQrt5E)
- The files in this repository

#### 1.2 Installation
    1 - Download the .zip files for the libraries
    2 - Open Arduino IDE
    3 - Click Sketch -> Include Library -> Add .ZIP Library
    4 - Select the library zip file you downloaded
    5 - Repeat steps 3 and 4 for each library
    6 - Download the files in this repository and put all of them in the Arduino folder (sub-folder name must match .ino file name)

A Fritzing scheme has been added to this repository with instructions to build the circuit. It is a *WIP*, and as each module gets tested I will continue to update the pin mapping. (For example, right now each LED goes directly to the arduino, in the future they will all be run from a MAX7219 to reduce pin usage)

Next, connect the Arduino board to your computer and load the program to the board.

#### 2. Components (List Subject to Change)
    1 - Arduino MEGA 2560 R3
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


#### 3. Modules

| Module Type  | Module Name         | Sketched         | Coded            | Made             |
| ------------ | ------------------- |:----------------:|:----------------:|:----------------:|
| Normal       | Button              |:heavy_check_mark:|:heavy_check_mark:|:heavy_check_mark:|
| Needy        | Capacitor Discharge |:heavy_check_mark:|:heavy_check_mark:|:heavy_check_mark:|
| Needy        | Knob                |:heavy_check_mark:|:heavy_check_mark:|:heavy_check_mark:|
| Normal       | Maze                |:heavy_check_mark:|:heavy_check_mark:|:heavy_check_mark:|
| Normal       | Memory              |:heavy_check_mark:|:heavy_check_mark:|                  |
| Normal       | Morse               |:heavy_check_mark:|:heavy_check_mark:|:heavy_check_mark:|
| Normal       | Password            |:heavy_check_mark:|                  |                  |
| Normal       | Simon Says          |:heavy_check_mark:|:heavy_check_mark:|:heavy_check_mark:|
| Needy        | Venting Gas         |:heavy_check_mark:|                  |                  |
| Normal       | Who's On First      |:heavy_check_mark:|:heavy_check_mark:|                  |
| Widget       | Buzzer              |:heavy_check_mark:|:heavy_check_mark:|:heavy_check_mark:|
| Widget       | Timer               |:heavy_check_mark:|:heavy_check_mark:|:heavy_check_mark:|
| Widget       | Strikes             |:heavy_check_mark:|:heavy_check_mark:|:heavy_check_mark:|
| Widget       | Batteries           |:heavy_check_mark:|                  |                  |
| Widget       | Indicators          |:heavy_check_mark:|                  |                  |
| Widget       | Ports               |:heavy_check_mark:|                  |                  |
| Normal       | Complicated Wires   |                  |                  |                  |
| Normal       | Keypad (Symbols)    |                  |                  |                  |
| Normal       | Wire Sequence       |                  |                  |                  |
| Normal       | Wires               |                  |                  |                  |

#### 4. Built With
[Arduino](https://www.arduino.cc/en/main/software) - The IDE used to code the modules logic.

[Fritzing](http://fritzing.org/home/) - The program used to create the components layout and wiring diagram.

#### 5. Authors
AziAssassin

#### 6. Credits
Thanks to calingeorgeadrian && ema1ih for setting the ground work for me to learn and get the basics of how they wrote their version of a select few of these modules.

Thanks to reddit u/advicevice for making me realize the potential this idea had to become a phyical game.

Last but not least: Keep Talking and Nobody Explodes - the [game](https://keeptalkinggame.com/) that inspired me in the first place.
