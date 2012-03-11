#pragma once
#include "GameObject.h"

class LevelTileVis;

class LevelTile : public iGameObject
{
public:
    LevelTile();
    virtual ~LevelTile() {};

    virtual void SetId(int id);
    virtual int GetId() const { return m_TileId; }
    
    LevelTileVis* GetTileVis() const;

protected:
    
    int m_TileId;

    virtual iGameObjectVis* CreateVis();
};