#include "game_pch.h"
#include "Vehicle.h"
#include "VehicleVis.h"
#include "game/Game.h"
#include "resources/TextureManager.h"

VehicleVis::VehicleVis(iModelObject* parent)
: iModelObjectVis(parent)
{
   
}

void VehicleVis::OnCreate()
{
    __super::OnCreate();

    SetTexture(Game::GetSingleton()->GetTextureManager()->Get("resources/ufodata/saucer/saucer92.bmp"));
}