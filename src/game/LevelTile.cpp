#include "LevelTile.h"
#include "LevelTileVis.h"

iGameObjectVis* LevelTile::CreateVis()
{
    return new LevelTileVis(this);
}