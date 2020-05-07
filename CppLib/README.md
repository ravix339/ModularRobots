# C++ Modular Robot Library

This folder contains a C++ library that models a 2D modular robot from the atoms to the a robot. The atom and module implementations have methods for attaching and detaching to other atoms/modules. A sample driver code that initializes a robot (and its internal modules and atoms) is included in `main.cpp`. 
In order to initialize a robot, a text file is read that uses <b>*</b> for locations of modules and any other character to define an empty space. 

## To Do:
- Include methods for K-Tunnel and other basic operations
- Implementation of a full scale algorithm using the operations
- Adjust X,Y coordinates of atoms when attaching and detaching them.