#ifndef TILE_H
#define TILE_H


class Tile {
    public:
        Tile();
        int ownerID;
        int militaryPower;
        int civilians;
        int money;
        bool city;
        virtual ~Tile();

    protected:

    private:
};

#endif // TILE_H
