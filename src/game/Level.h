#pragma once
#include <string>
#include "vec.h"

class LevelTile;

class Level
{
public:
    Level();

    bool Load(const std::string &file);
    
    void Unload();

protected:
};