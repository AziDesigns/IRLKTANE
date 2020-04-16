//On the Subject of Mazes
/*
 * The Maze module is a module that consists of a 6x6 grid of squares with two of the squares containing green indicator circles, 
 * one square containing a white square, and one square containing a red triangle as well as four directional buttons around the maze. 
 * In order to disarm the module, the Defuser must guide the white light to the red triangle without running into any of the walls shown in the manual.
 * The manual page consists of nine mazes, one of which will be the maze present on the module. 
 * The Defuser must communicate the positions of the green circles to the Expert to identify the correct maze, 
 * and the expert must guide the Defuser's white light to the red triangle to defuse the module.
 * 
 * We wont be able to make a green circle around the 2 indicators but can light up the grid in green/ red/ and white for this module.
 * We also cant get a 6x6 matrix unless we make it ourselves. Alt we use a 8x8 grid and black out the 1 line around all sides when mounting.
 */

void mazeSetup()
{
  
}

void mazeLoop()
{
  // THIS NEEDS TO GO SOMEWHERE
  mazeModuleDefused = true;

  /*
   * The bomb will record a strike if the defuser attempts to move through a wall. The wall will light up in red (we cant do this with a 8x8 grid) 
   * and remain lit until the end of the game. 
    The white light will not move ever after being set randomly (until diffused)
    The module will be disarmed when the defuser moves the white light onto the red triangle, provided that there is no wall in between the two.
   */
}

void mazeModuleBoom()
{
  
}
