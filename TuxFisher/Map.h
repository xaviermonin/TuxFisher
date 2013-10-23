#ifndef MAP_H
#define MAP_H

#include "../Commun/definitions.h"
#include <QByteArray>
#include <QVarLengthArray>
#include <QMap>

namespace Square {
    enum Type { Avalanche=33, Void, Tux, Snow, Fish, Mountain, Error=-1};
}

class Map
{
public:
    Map(const QByteArray& data, unsigned int height, unsigned int width);

    bool loadMap(const QByteArray& data, unsigned int height, unsigned int width);

    inline const Square::Type& at(int x, int y) const { return map[y][x]; }
    void set(int x, int y, Square::Type a);

    void switchSquare(int first_x, int first_y, int second_x, int second_y);

    inline unsigned int getQuantityOf(Square::Type a) const { return stats[a]; }

    inline unsigned int getWidth() const { return width; }
    inline unsigned int getHeight() const { return height; }

private:

    inline Square::Type& at(int x, int y) { return map[y][x]; }

    // Max Size of a map: 30x20.
    // Access : [column, line]
    QVarLengthArray< QVarLengthArray< Square::Type , WIDTH_MAX_MAP > , HEIGHT_MAX_MAP > map;
    QMap< Square::Type, unsigned int> stats;

    unsigned int height;
    unsigned int width;
};

#endif // MAP_H
