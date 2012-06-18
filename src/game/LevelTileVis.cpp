#include "LevelTileVis.h"
#include "LevelTile.h"
#include "Level.h"

LevelTileVis::LevelTileVis(iGameObject *parent)
: iGameObjectVis(parent)
{
}

void LevelTileVis::OnCreate()
{
    InitializeSprite();
}

void LevelTileVis::OnTileIdChanged()
{
    if(!GetTile())
        return;
    
    int new_id = GetTile()->GetId();

    if(!GetTile()->GetLevel())
        return;

    SetTexture(GetTile()->GetLevel()->GetTexture(new_id));
}

LevelTile *LevelTileVis::GetTile() const
{
    return dynamic_cast<LevelTile*>(GetParent());
}
