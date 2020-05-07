class Atom
{
    friend class Module;

public:
    Atom();
    Atom(int x, int y, Atom *north = nullptr, Atom *east = nullptr, Atom *south = nullptr, Atom *west = nullptr);

    bool Detach(char dir);
    bool Attach(char dir, Atom &other);
    bool RetractArm(char dir);
    bool ExtendArm(char dir);
    void Translate(char fromDir, int dx, int dy); //Not used by module since the movements get complicated

private:
    int x;
    int y;

    Atom *north;
    Atom *east;
    Atom *south;
    Atom *west;

    bool northArm;
    bool eastArm;
    bool southArm;
    bool westArm;

    bool DetachNorth(bool recurse = true);
    bool DetachEast(bool recurse = true);
    bool DetachSouth(bool recurse = true);
    bool DetachWest(bool recurse = true);

    bool AttachNorth(Atom &other, bool recurse = true);
    bool AttachEast(Atom &other, bool recurse = true);
    bool AttachSouth(Atom &other, bool recurse = true);
    bool AttachWest(Atom &other, bool recurse = true);

    bool Attachable(char dir, Atom &other);
};