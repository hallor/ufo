#pragma once
#include "GameObjectVis.h"

class LevelTileVis : public iGameObjectVis
{
public:
    LevelTileVis(iGameObject *parent);
    virtual ~LevelTileVis() {};
};