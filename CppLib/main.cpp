#include <iostream>
#include <fstream>
#include <vector>
#include "Robot.h"

using namespace std;

int main() {
    ifstream fileIn;
    string read;
    vector<string> fileFull;
    fileIn.open("robot.txt");
    while (getline(fileIn, read)) {
        fileFull.push_back(read);
    }

    Robot x = Robot(fileFull,32);
    cout << x <<endl;
}