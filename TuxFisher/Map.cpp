#include "Map.h"

Map::Map(const QByteArray& data, unsigned int height, unsigned int width)
    : height(0), width(0)
{
    stats[Square::Tux] = stats[Square::Avalanche] = stats[Square::Error] = 0;
    stats[Square::Fish] = stats[Square::Mountain] = stats[Square::Snow] = stats[Square::Void] = 0;

    loadMap(data, height, width);
}

void Map::set(int x, int y, Square::Type a)
{
    stats[ at(x, y) ] --;
    stats[a]++;

    at(x, y) = a;
}

void Map::switchSquare(int first_x, int first_y, int second_x, int second_y)
{
    Square::Type first = at(first_x, first_y);
    at(first_x, first_y) = at(second_x, second_y);
    at(second_x, second_y) = first;
}

bool Map::loadMap(const QByteArray& data, unsigned int height, unsigned int width)
{
    if (height > HEIGHT_MAX_MAP || width > WIDTH_MAX_MAP ||
	height == 0 || width == 0 ||
	data.size() != (height * width))
	return false;

    // Copy to map
    for (size_t y = 0; y < height; ++y)
    {
	QVarLengthArray< Square::Type, 30 > line;
	line.append( (Square::Type*) data.data(), width);

	// Load the stats
	for (int i = 0; i < line.size(); ++i)
	    stats[ line[i] ] ++;

	map.append(line);
    }

    this->height = height;
    this->width = width;

    return true;
}
