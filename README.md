# ModularRobots

This is a high level simulation of the Log(N) cube-style modular robot reconfiguration algorithm described in [this paper](http://cglab.ca/~greg/papers/logn-robots-isaac.pdf). This library utilizes the lemmas provided in the paper to represent the modular robot before and after each step of the reconfiguration. As it stands, the high level operations (staircases, elevators, 1-Tunnels, etc.) are not displayed in each step. Future work would implement these steps. 

The modrobots.py file is the import file to get access to various operations after creating an instance of the ModRobot class and specifying the number of 4x4 modules that exist in a block. In order for results to be accurate, this number must be a power of 2 that is at least 8 (meaning that a block is 32x32 atoms). In the initial configuration a block must be filled entirely of atoms or completely empty.

## Methods

#### change_to_desired_form()
This is the main function that takes in an initial configuration and returns a tuple of the final state and the entire progression (including initial and final states) of the robot.

The input is a 2D List of ints (1 or 0 to represent a 4x4 module is there or not). The dimensions of this input list must be BLOCKSIZE\*n x BLOCKSIZE\*n where n is some integer constant. In other words, the input must be a square matrix. Each block, as mentioned before, must be comprised of all 1's or all 0's.

#### block_32_to_basic()
This method simplifies the input process for change_to_desired_form() by taking in a 2d list of blocks (again a square matrix) that contains 1s where there are full blocks and 0s where there are no modules. This will return a BLOCKSIZE\*n x BLOCKSIZE\*n where n is the side length of the input.

#### convert_to_out()
This method takes in a 2D square matrix (of 1s and 0s since this is ideally to be used with progressions) creates a version of the input that is easier to read the results for. Empty spaces become '.'s and where 4x4 modules are, there is a '0'. 

#### display()
This method takes in a 2D square matrix (of 1s and 0s since this is ideally to be used with progressions) and prints it to the screen using the convert_to_out() method.

## Example Code
The driver.py file contains example code which both displays the progressions and writes each to a file labeled by its timestamp. There are multiple example lists that can be uncommented to test.

## Future Work
* Incorporate the high level modular movements
* Create UI for ease of input
* Take output and convert to bitmap.
