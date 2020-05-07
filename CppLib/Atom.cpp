#include "Atom.h"

Atom::Atom() 
	: north(nullptr), east(nullptr), south(nullptr), west(nullptr),
	northArm(true), eastArm(true), southArm(true), westArm(true) {}
Atom::Atom(int x, int y, Atom *north, Atom *east, Atom *south, Atom *west)
    : x(x), y(y), north(north), east(east), south(south), west(west),
      northArm(true), eastArm(true), southArm(true), westArm(true) {}

bool Atom::Detach(char dir)
{
    bool ret;
    switch (dir)
    {
    case 'n':
        ret = DetachNorth();
        break;
    case 'e':
        ret = DetachEast();
        break;
    case 's':
        ret = DetachSouth();
        break;
    case 'w':
        ret = DetachWest();
        break;
    default:
        return false;
    }
    return ret;
}

bool Atom::Attach(char dir, Atom &other)
{
    bool ret;

    if (!Attachable(dir, other))
    {
        return false;
    }

    switch (dir)
    {
    case 'n':
        ret = AttachNorth(other);
        break;
    case 'e':
        ret = AttachEast(other);
        break;
    case 's':
        ret = AttachSouth(other);
        break;
    case 'w':
        ret = AttachWest(other);
        break;
    default:
        return false;
    }
    return ret;
}

bool Atom::RetractArm(char dir)
{
    switch (dir)
    {
    case 'n':
        northArm = false;
        if (north)
            north->Translate('s', 0, -0.5);
        break;
    case 'e':
        eastArm = false;
        if (east)
            east->Translate('w', -0.5, 0);
        break;
    case 's':
        southArm = false;
        if (south)
            south->Translate('n', 0, 0.5);
        break;
    case 'w':
        westArm = false;
        if (west)
            west->Translate('e', 0.5, 0);
        break;
    default:
        return false;
    }
    return true;
}

bool Atom::ExtendArm(char dir)
{
    switch (dir)
    {
    case 'n':
        northArm = true;
        if (north)
            north->Translate('s', 0, 0.5);
        break;
    case 'e':
        eastArm = true;
        if (east)
            east->Translate('w', 0.5, 0);
        break;
    case 's':
        southArm = true;
        if (south)
            south->Translate('n', 0, -0.5);
        break;
    case 'w':
        westArm = true;
        if (west)
            west->Translate('e', -0.5, 0);
        break;
    default:
        return false;
    }
    return true;
}

void Atom::Translate(char fromDir, int dx, int dy)
{
    x += dx;
    y += dy;

    if (fromDir != 'n' && north)
        north->Translate('s', dx, dy);
    if (fromDir != 'e' && east)
        east->Translate('w', dx, dy);
    if (fromDir != 's' && south)
        south->Translate('n', dx, dy);
    if (fromDir != 'w' && west)
        west->Translate('e', dx, dy);
}

bool Atom::DetachNorth(bool recurse)
{
    if (north)
    {
        if (recurse)
        {

            north->DetachSouth(false);
        }

        north = nullptr;
        return true;
    }
    return false;
}

bool Atom::DetachEast(bool recurse)
{
    if (east)
    {
        if (recurse)
        {
            east->DetachWest(false);
        }
        east = nullptr;
        return true;
    }
    return false;
}

bool Atom::DetachSouth(bool recurse)
{
    if (south)
    {
        if (recurse)
        {
            south->DetachNorth(false);
        }
        south = nullptr;
        return true;
    }
    return false;
}

bool Atom::DetachWest(bool recurse)
{
    if (west)
    {
        if (recurse)
        {
            west->DetachEast(false);
        }
        west = nullptr;
        return true;
    }
    return false;
}

bool Atom::AttachNorth(Atom &other, bool recurse)
{
    if (!north)
    {
        north = &other;
        if (recurse)
        {
            north->AttachSouth(*this, false);
        }
        return true;
    }
    return false;
}

bool Atom::AttachEast(Atom &other, bool recurse)
{
    if (!east)
    {
        east = &other;
        if (recurse)
        {
            east->AttachWest(*this, false);
        }
        return true;
    }
    return false;
}

bool Atom::AttachSouth(Atom &other, bool recurse)
{
    if (!south)
    {
        south = &other;
        if (recurse)
        {
            south->AttachNorth(*this, false);
        }
        return true;
    }
    return false;
}

bool Atom::AttachWest(Atom &other, bool recurse)
{
    if (!west)
    {
        west = &other;
        if (recurse)
        {
            west->AttachEast(*this, false);
        }

        return true;
    }
    return false;
}

bool Atom::Attachable(char dir, Atom &other)
{
    int otherArm = 0;
    int thisArm = 0;

    switch (dir)
    {
    case 'n':
        thisArm = (this->northArm ? 0.5 : 0);
        otherArm = (other.southArm ? -0.5 : 0);
        break;
    case 'e':
        thisArm = (this->eastArm ? 0.5 : 0);
        otherArm = (other.westArm ? -0.5 : 0);
        break;
    case 's':
        thisArm = (this->southArm ? -0.5 : 0);
        otherArm = (other.northArm ? 0.5 : 0);
        break;
    case 'w':
        thisArm = (this->westArm ? -0.5 : 0);
        otherArm = (other.eastArm ? 0.5 : 0);
        break;
    default:
        return false;
    }

    if (dir == 'e' || dir == 'w')
    {
        return (other.x + otherArm == this->x + thisArm);
    }
    return (other.y + otherArm == this->y + thisArm);
}