#ifndef CITYMAP_H
#define CITYMAP_H

#include <string>

#include "SDL/SDL.h"
#include "gfx.h"

class Tile
{
public:
  Uint16 tile;
};

class CityMap
{
public:
    CityMap(int w, int h, int d) : width(w), height(h), depth(d), map(NULL) {}

    ~CityMap()
    {
      free();
    }

    void free();

    bool load(const std::string & filename);

    int width, height, depth;
    Tile ***map;
};

#endif // CITYMAP_H
