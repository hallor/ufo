#include "game_pch.h"
#include "ModelObjectVis.h"
#include "ModelObject.h"
#include "Sprite3D.h"

iModelObjectVis::iModelObjectVis(iModelObject* parent)
{
    m_Parent = parent;
    m_Sprite = NULL;
}

void iModelObjectVis::OnCreate()
{
    CHECKI(GetParent(), "");
    InitializeSprite();
}

void iModelObjectVis::OnDestroy()
{
    DestroySprite();
}

void iModelObjectVis::OnRenderFrame()
{
    if (GetSprite3D())
    {
        GetSprite3D()->SetPosition(GetParent()->GetPos() + GetPosOffset());
        GetSprite3D()->PrepareForRendering();
    }
}

bool iModelObjectVis::SetTexture(const cTexture &texture)
{
    if(!GetSprite3D())
        return false;

    GetSprite3D()->SetTexture(texture);
    return true;
}

bool iModelObjectVis::InitializeSprite()
{
    if(GetSprite3D())
       return true;
       
    m_Sprite = new (std::nothrow) cSprite3D; 
    m_Sprite->Initialize();

    return GetSprite3D() != NULL;
}

void iModelObjectVis::DestroySprite()
{
    if(!GetSprite3D())
        return;

    delete m_Sprite;
    m_Sprite = NULL;
}