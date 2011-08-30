#ifndef CITYBUILDING_H
#define CITYBUILDING_H

#include <vector>
#include <string>
#include <stdint.h>

#include "FileIO.h"
#include "logger.h"

struct CityBuildingRecord
{
  uint16_t name_id;
  uint16_t x1;
  uint16_t x2;
  uint16_t y1;
  uint16_t y2;
  uint8_t unknown[216];
};

class CityBuilding
{
public:
  CityBuilding() : name(0), owner(0), type(0) { }

    void print()
    {
      LogInfo("Building %i, x(%i,%i), y(%i,%i).", rec.name_id, rec.x1, rec.x2, rec.y1, rec.y2);
    }

    const char * name;
    const char * owner;
    const char * type;
    CityBuildingRecord rec;
};

class CityBuildings
{
public:
  ~CityBuildings()
  {
    clear();
  }

  void clear()
  {
    for (int i=0; i<buildings.size(); ++i)
      delete buildings[i];
    buildings.clear();
  }

  bool load(const std::string & filename)
  {
    clear();

    iFile *f = CreateFileIO();

    LogDebug("Loading building data from %s", filename.c_str());

    if (!f->Open(filename, FFileOpenFlags::Read))
    {
      LogError("Error opening file %s.", filename.c_str());
      return false;
    }

    int record_count = f->GetSize() / sizeof(CityBuildingRecord);

    if (f->GetSize() & sizeof(CityBuildingRecord))
      LogWarning("Building map %s has spare bytes.", filename.c_str());

    buildings.reserve(record_count);

    for (int i=0; i<record_count; ++i)
    {
      CityBuilding * bld = new CityBuilding();
      f->Read(&bld->rec, sizeof(CityBuildingRecord));

      buildings.push_back(bld);
      bld->print();
    }

    ReleaseFileIO(f);
  }

  CityBuilding * getBuilding(int no);
  int buildingCount() const;

  std::vector<CityBuilding*> buildings;
};

#endif // CITYBUILDING_H
