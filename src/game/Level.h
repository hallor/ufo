#pragma once
#include <string>
#include "vec.h"

class LevelTile;

class Level
{
public:
    Level();

    bool Load(const std::string &file);

    const LevelTile* GetTile(const vec3 &pos) const;

    void Unload();

protected:

    bool CheckTilePositionBounds(const vec3 &pos) const;
    int IndexFromPos(const vec3 &pos) const;

    static const int MAP_DIM_X = 1000, MAP_DIM_Y = 10, MAP_DIM_Z = 1000;

    vec3 m_Dimensions;
    LevelTile *m_Tiles;
};