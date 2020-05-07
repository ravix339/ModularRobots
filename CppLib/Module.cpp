#include "Module.h"

Module::Module() : empty(true) {}

//x and y point to top left of the module
Module::Module(int n, int x, int y) : dim(n), empty(false), minX(x), minY(y)
{
    //Create a nxn module of atoms
    atoms = new Atom *[n];
    for (int i = 0; i < n; i++)
        atoms[i] = new Atom[n];

    for (int y = 0; y < n; y++)
    {
        for (int x = 0; x < n; x++)
        {
            //Manually initialize the atoms
            atoms[y][x].x = minX + x;
            atoms[y][x].y = minY + y;

            if (y != 0)
                atoms[y][x].north = &atoms[y - 1][x];
            if (y != n - 1)
                atoms[y][x].south = &atoms[y + 1][x];
            if (x != n - 1)
                atoms[y][x].east = &atoms[y][x + 1];
            if (x != 0)
                atoms[y][x].west = &atoms[y][x - 1];
        }
    }
}

void Module::Translate(char fromDir, int dx, int dy)
{
    if (empty)
        return;
    minX += dx;
    minY += dy;
    for (int y = 0; y < dim; y++)
    {
        for (int x = 0; x < dim; x++)
        {
            atoms[y][x].x += dx;
            atoms[y][x].y += dy;
        }
    }

    if (fromDir != 'n' && north)
        north->Translate('s', dx, dy);
    if (fromDir != 'e' && east)
        east->Translate('w', dx, dy);
    if (fromDir != 's' && south)
        south->Translate('n', dx, dy);
    if (fromDir != 'w' && west)
        west->Translate('e', dx, dy);
}

bool Module::IsEmpty() { return empty; }

bool Module::Detach(char dir)
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

bool Module::Attach(char dir, Module *other)
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

Atom &Module::GetAtom(int x, int y)
{
	if (empty) {
		throw exception("Module is empty");
	}
    int xLoc = x - minX;
    int yLoc = y - minY;

    if (xLoc < 0 || xLoc >= dim || yLoc < 0 || yLoc >= dim)
    {
        throw exception("(X,Y) coordinates not found in this module");
    }
    return atoms[yLoc][xLoc];
}

Module::~Module()
{
    for (int i = 0; i < dim; i++)
        delete atoms[i];
    delete atoms;
}

bool Module::DetachNorth(bool recurse)
{
    if (north)
    {
        if (recurse)
        {
            for (int x = 0; x < dim; x++)
            {
                atoms[0][x].DetachNorth();
            }
            north->DetachSouth(false);
        }

        north = nullptr;
        return true;
    }
    return false;
}

bool Module::DetachEast(bool recurse)
{
    if (east)
    {
        if (recurse)
        {
            for (int y = 0; y < dim; y++)
            {
                atoms[y][dim - 1].DetachEast();
            }
            east->DetachWest(false);
        }
        east = nullptr;
        return true;
    }
    return false;
}

bool Module::DetachSouth(bool recurse)
{
    if (south)
    {
        if (recurse)
        {
            for (int x = 0; x < dim; x++)
            {
                atoms[dim - 1][x].DetachSouth();
            }
            south->DetachNorth(false);
        }
        south = nullptr;
        return true;
    }
    return false;
}

bool Module::DetachWest(bool recurse)
{
    if (west)
    {
        if (recurse)
        {
            for (int y = 0; y < dim; y++)
            {
                atoms[y][0].DetachWest();
            }
            west->DetachEast(false);
        }
        west = nullptr;
        return true;
    }
    return false;
}

bool Module::AttachNorth(Module *other, bool recurse)
{
    if (!north)
    {
        north = other;
        if (recurse)
        {
            for (int x = 0; x < dim; x++)
            {
                atoms[0][x].AttachNorth(other->GetAtom(minX + x, minY - 1));
            }
            north->AttachSouth(this, false);
        }
        return true;
    }
    return false;
}

bool Module::AttachEast(Module *other, bool recurse)
{
    if (!east)
    {
        east = other;
        if (recurse)
        {
            for (int y = 0; y < dim; y++)
            {
                atoms[y][dim - 1].AttachEast(other->GetAtom(minX + dim, minY + y));
            }
            east->AttachWest(this, false);
        }
        return true;
    }
    return false;
}

bool Module::AttachSouth(Module *other, bool recurse)
{
    if (!south)
    {
        south = other;
        if (recurse)
        {
            for (int x = 0; x < dim; x++)
            {
                atoms[dim - 1][x].AttachSouth(other->GetAtom(minX + x, minY + dim));
            }
            south->AttachNorth(this, false);
        }
        return true;
    }
    return false;
}

bool Module::AttachWest(Module *other, bool recurse)
{
    if (!west)
    {
        west = other;
        if (recurse)
        {
            for (int y = 0; y < dim; y++)
            {
                atoms[y][0].AttachWest(other->GetAtom(minX - 1, minY + y));
            }
            west->AttachEast(this, false);
        }

        return true;
    }
    return false;
}

bool Module::Attachable(char dir, Module *other)
{
    if (empty || other->IsEmpty())
    {
        return false;
    }

    switch (dir)
    {
    case 'n':
        for (int x = 0; x < dim; x++)
        {
            if (atoms[0][x].Attachable('n', other->GetAtom(minX + x, minY - 1)))
            {
                return false;
            }
        }
        break;
    case 'e':
        for (int y = 0; y < dim; y++)
        {
            if (atoms[y][dim - 1].Attachable('e', other->GetAtom(minX + dim, minY + y)))
            {
                return false;
            }
        }
        break;
    case 's':
        for (int x = 0; x < dim; x++)
        {
            if (atoms[dim - 1][x].Attachable('s', other->GetAtom(minX + x, minY + dim)))
            {
                return false;
            }
        }
        break;
    case 'w':
        for (int y = 0; y < dim; y++)
        {
            if (atoms[y][0].Attachable('w', other->GetAtom(minX - 1, minY + y)))
            {
                return false;
            }
        }
        break;
    }

    return true;
}