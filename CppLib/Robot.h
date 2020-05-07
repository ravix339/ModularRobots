#include <string>
#include <vector>
#include "Module.h"
using namespace std;
class Robot
{
public:
	friend ostream &operator<<(ostream &os, const Robot &robot);

	Robot(vector<string> fileContents, int modDim = 32);
	~Robot();
	string ToString() const;
private:
    Module ***modules;
    int dimX;
    int dimY;

	int verifyOneTunnel(int x1, int y1, int x2, int y2);
};