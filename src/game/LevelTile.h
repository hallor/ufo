#pragma once
#include "GameObject.h"

class LevelTileVis;
class Level;

class LevelTile : public iGameObject
{
public:
    LevelTile();
    virtual ~LevelTile() {};

    virtual void OnCreate(Level *parent);

    virtual void SetId(int id);
    virtual int GetId() const { return m_TileId; }
    
    LevelTileVis* GetTileVis() const;

    Level *GetLevel() const { return m_Parent; }

protected:
    
    Level *m_Parent;

    int m_TileId;

    virtual iGameObjectVis* CreateVis();
};