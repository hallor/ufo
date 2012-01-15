#pragma once
#include "vec.h"
#include <string>
// TODO GameObject & GameObjectVis
class LevelTile
{
public:
    LevelTile(); 

    int GetTextureId() const { return m_Id; }
    void SetTextureId(int id);

    vec3 GetPos() const { return m_Pos; }
    void SetPos(const vec3 &pos);

protected:

    int m_Id;// move id to GameObjectVis & resource manager later

    vec3 m_Pos;
};

class Level
{
public:
    Level();

    bool Load(const std::string &file);

    const LevelTile* GetTileData(const vec3 &pos) const;

    void Unload();

protected:

    bool CheckTilePositionBounds(const vec3 &pos) const;
    int IndexFromPos(const vec3 &pos) const;

    static const int MAP_DIM_X = 1000, MAP_DIM_Y = 10, MAP_DIM_Z = 1000;

    vec3 m_Dimensions;
    LevelTile *m_Tiles;
};