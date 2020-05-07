#include "Robot.h"

Robot::Robot(vector<string> fileContents, int modDim)
{
	int maxLength = 0;
	dimY = fileContents.size();
	for (string row : fileContents)
	{
		if (row.length() > maxLength)
			maxLength = row.length();
	}

	dimX = maxLength;

	//Initialize the modules in the robot
	modules = new Module **[dimY];
	for (int y = 0; y < dimY; y++)
	{
		modules[y] = new Module*[dimX];
		string in = fileContents[y];
		for (int x = 0; x < fileContents[y].size(); x++)
		{
			if (in[x] == '*')
			{
				modules[y][x] = new Module(modDim, x*modDim, y*modDim);
			}
			else {
				modules[y][x] = new Module();
			}
		}
	}
	for (int y = 0; y < dimY; y++) {
		for (int x = 0; x < dimX; x++) {

			if (y != 0)
				modules[y][x]->Attach('n', modules[y - 1][x]);
			if (y != dimY - 1)
				modules[y][x]->Attach('s', modules[y + 1][x]);
			if (x != 0)
				modules[y][x]->Attach('w', modules[y][x - 1]);
			if (x != dimX - 1)
				modules[y][x]->Attach('e', modules[y][x + 1]);
		}
	}
}

Robot::~Robot() {
	for (int y = 0; y < dimY; y++) {
		for (int x = 0; x < dimX; x++) {
			delete modules[y][x];
		}
		delete modules[y];
	}
	delete modules;
}


string Robot::ToString() const {
	string str = "";
	for (int y = 0; y < dimY; y++) {
		for (int x = 0; x < dimX; x++) {
			if (modules[y][x]->IsEmpty()) {
				str += " ";
			}
			else {
				str += "*";
			}
		}
		str += "\n";
	}
	return str;
}

ostream &operator<<(ostream &os, const Robot &robot) {
	os << robot.ToString();
	return os;
}


int Robot::verifyOneTunnel(int x1, int y1, int x2, int y2) { //1 for transition through x first; 0 for y.
	if (x1 == x2 || y1 == y2) {
		throw exception("Coordinates must be different x,y coordinates");
	}
	if (x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0) {
		throw exception("Coordinate out of bounds");
	}
	if (x1 >= dimX || x2 >= dimX || y1 >= dimY || y2 >= dimY) {
		throw exception("Coordinate out of bounds");
	}
	Module *start = modules[y1][x1];
	if (start->IsEmpty()) {
		throw exception("Start is an empty module");
	}
	if (!modules[y2][x2]->IsEmpty()) {
		throw exception("End position is non-empty");
	}
	bool xFirst = true;
	bool yFirst = true;

	if (y1 > y2) {
		for (int y = y1; y > y2; y--) {
			if (modules[y][x1]->IsEmpty()) {
				yFirst = false;
			}
			if (modules[y][x2]->IsEmpty()) {
				xFirst = false;
			}
		}
	}
	else {
		for (int y = y1; y < y2; y++) {
			if (modules[y][x1]->IsEmpty()) {
				yFirst = false;
			}
			if (modules[y][x2]->IsEmpty()) {
				xFirst = false;
			}
		}
	}

	if (x1 > x2) {
		for (int x = x1; x > x2; x--) {
			if (modules[y1][x]->IsEmpty()) {
				xFirst = false;
			}
			if (modules[y2][x]->IsEmpty()) {
				yFirst = false;
			}
		}
	}
	else {
		for (int x = x1; x < x2; x++) {
			if (modules[y1][x]->IsEmpty()) {
				xFirst = false;
			}
			if (modules[y2][x]->IsEmpty()) {
				yFirst = false;
			}
		}
	}
	if (xFirst && yFirst) {
		throw exception("Start is not a leaf node");
	}
	if (xFirst) {
		return 1;
	}
	if (yFirst) {
		return -1;
	}
	throw exception("No path found from start to end");
}