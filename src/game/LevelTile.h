#pragma once
#include "GameObject.h"

class LevelTile : public iGameObject
{
public:
    LevelTile() {};
    virtual ~LevelTile() {};

protected:
    
    virtual iGameObjectVis* CreateVis();

};