#include "LevelTile.h"
#include "LevelTileVis.h"

LevelTile::LevelTile()
: m_TileId(-1)
{
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