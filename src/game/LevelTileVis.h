#pragma once
#include "GameObjectVis.h"

class LevelTile;

class LevelTileVis : public iGameObjectVis
{
public:
    LevelTileVis(iGameObject *parent);
    virtual ~LevelTileVis() {};

    virtual void OnTileIdChanged();

protected:
    
    LevelTile* GetTile() const;
};