#include "Atom.h"
#include <iostream>
#include <exception>

using std::exception;

/*
 * Known Bugs:
 *	- Module width doesn't take into account the full width of a atom (1, 1.5, or 2) so
 *		the attachability and (x, y) coordinates calculated from a module and the (x, y)
 *		coordinates in the atom can differ
 */

class Module
{
public:
    Module();
    Module(int n, int x = 0, int y = 0); //x and y point to top left of the module
    ~Module();

    void Translate(char fromDir, int dx, int dy);
    bool IsEmpty();

    bool Detach(char dir);
    bool Attach(char dir, Module *other);

    Atom &GetAtom(int x, int y);

private:
    bool empty;
    int minX;
    int minY;

    Module *north;
    Module *south;
    Module *east;
    Module *west;
    int dim;
    Atom **atoms;

    bool DetachNorth(bool recurse = true);
    bool DetachEast(bool recurse = true);
    bool DetachSouth(bool recurse = true);
    bool DetachWest(bool recurse = true);

    bool AttachNorth(Module *other, bool recurse = true);
    bool AttachEast(Module *other, bool recurse = true);
    bool AttachSouth(Module *other, bool recurse = true);
    bool AttachWest(Module *other, bool recurse = true);
    
    bool Attachable(char dir, Module *other);
};