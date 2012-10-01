#pragma once
#include <string>
#include "Terrain.h"

class Terrain;

class Level
{
public:
    Level();

    bool Load(const std::string &file);
    
    void Unload();

    void Update(float dt);

    const Terrain* GetTerrain() const { return m_Terrain; }

    bool IsLoaded() const { return GetTerrain() ? GetTerrain()->IsLoaded() : false; }

protected:

    Terrain* m_Terrain;
};