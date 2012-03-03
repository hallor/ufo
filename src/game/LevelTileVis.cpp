#include "LevelTileVis.h"
#include "LevelTile.h"
#include "Game.h"

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
    int new_id = -1;
    if(GetTile())
        new_id = GetTile()->GetId();

    SetTexture("resources\\ufodata\\alien1.bmp");
}

LevelTile *LevelTileVis::GetTile() const
{
    return dynamic_cast<LevelTile*>(GetParent());
}
