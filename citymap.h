#ifndef CITYMAP_H
#define CITYMAP_H
#include <string>

/** Single city tile, number == number of pixmap in city/alien.pck file */
class Tile
{
public:
  Tile(unsigned short id = 0) : t(id){}

  operator unsigned short() const { return t; }

private:
  unsigned short t;
};

/** Map of whole city loaded from alienmap and citymap* files. */
class CityMap
{
public:
    CityMap(int w, int h, int d) : width(w), height(h), depth(d), map(NULL), nulltile(0) { }

    ~CityMap()
    {
      free();
    }

    void free();

    bool load(const std::string & filename);

    inline Tile getTile(const unsigned int & x, const unsigned int & y, const unsigned int & z) const
    {
      if (x < width && x < height && z < depth)
        return map[x + y*width + z * width * height ];
      return nulltile;
    }

    inline const Tile * getTileLine(const  unsigned int & y, const unsigned int & z) const
    {
      if (y < height && z < depth)
        return map + y*width + z * width * height;
      return NULL;
    }

    bool setTile(const unsigned int & x, const unsigned int & y, const unsigned int & z, const Tile t)
    {
      if (x < width && x < height && z < depth)
      {
        map[x + y*width + z * width * height ] = t;
        return true;
      }
      return false;
    }

private:
    unsigned int width, height, depth;
    Tile *map;
    Tile nulltile;
};

#endif // CITYMAP_H
