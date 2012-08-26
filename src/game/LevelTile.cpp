#include "LevelTile.h"
#include "LevelTileVis.h"
#include "Level.h"

LevelTile::LevelTile()
: m_TileId(-1)
{
    m_Parent = nullptr;
}

void LevelTile::OnCreate(Level *parent)
{
    m_Parent = parent;
    iGameObject::OnCreate();
}

iGameObjectVis* LevelTile::CreateVis()
{
    return new LevelTileVis(this);
}

void LevelTile::SetId(int id)
{
    m_TileId = id;

    if(GetTileVis())
        GetTileVis()->OnTileIdChanged();
}

LevelTileVis* LevelTile::GetTileVis() const
{
    return dynamic_cast<LevelTileVis*>(GetVis());
}
