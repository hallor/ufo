#include <fstream>

#include "citymap.h"
#include "logger.h"

using namespace std;

void CityMap::free()
{
  if (map)
    delete [] map;
  map = NULL;
}


bool CityMap::load(const std::string & filename)
{
  free();

  if (width<1 || height<1 || depth < 1)
    return false;

  // TODO: hack to remove ;)
  if (width*height*depth > 1000*1000*100)
    return false;

  map = new Tile [width * height * depth];

  LogDebug("Loading map %ix%ix%i from %s.", width, height, depth, filename.c_str());

  ifstream ff;
  unsigned short tile;
  size_t x=0, num_rec=width*height*depth;
  ff.open(filename.c_str(), ios::in | ios::binary);

  while (ff.good())
  {
    ff.read((char*)&tile, 2);
    map[x] = Tile(tile);
    x++;
    if (x >= num_rec)
      break;
  }

  if (x==num_rec)
  {
    LogInfo("Map %s loaded correctly.", filename.c_str());
    return true;
  } else
  {
    LogError("Unable to load map %s.", filename.c_str());
    return false;
  }
}
