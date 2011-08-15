#include <fstream>
#include <iostream>

#include "citymap.h"

using namespace std;

void CityMap::free()
{
  if (map)
    for (int i=0; i<width; i++)
    {
      for (int j=0; j<height; j++)
        delete [] map[i][j];
      delete [] map[i];
    }
    delete [] map;
  map = NULL;
}


bool CityMap::load(const std::string & filename)
{
  free();

  if (width<1 || height<1)
    return false;

  map = new Tile **[width];
  for (int i=0; i<width; ++i)
  {
    map[i] = new Tile *[height];
    for (int j=0; j<height; j++)
      map[i][j] = new Tile[depth];
  }

  cout << "Loading map " << width << "x" << height << "x" << depth << " from " << filename << endl;

  ifstream ff;
  int x=0,y=0,z=0;
  Uint16 tile;
  ff.open(filename.c_str(), ios::in | ios::binary);
  while (ff.good())
  {
    ff.read((char*)&tile, 2);
    map[x][y][z].tile = tile;
    x++;
    if (x >= width)
    {
      y++;
      if (y >= height)
      {
        z++;
        if (z >= depth)
          break;
        y=0;
      }
      x = 0;
    }
  }
  cout << "Loaded map of size: " << x << "x" << y <<"x" << z << endl;

  return x+1 == width && y+1 == height && z+1 == depth;
}
