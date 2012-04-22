#include <cstdlib>
#include "GameObjectVis.h"
#include "GameObject.h"
#include "Sprite3D.h"

iGameObjectVis::iGameObjectVis(iGameObject *parent)
{
    m_Parent = parent;
    m_Sprite = NULL;
}

iGameObjectVis::~iGameObjectVis()
{
}

void iGameObjectVis::OnDestroy()
{
    DestroySprite();
}

void iGameObjectVis::Update(float dt)
{
    if(GetSprite3D())
        GetSprite3D()->SetPosition(GetParent()->GetPos() + GetPosOffset());
}

bool iGameObjectVis::SetTexture(const std::string &texture)
{
    if(!GetSprite3D())
        return false;

    GetSprite3D()->SetTexture(texture);
    return true;
}

bool iGameObjectVis::InitializeSprite()
{
    if(GetSprite3D())
       return true;
       
    m_Sprite = new (std::nothrow) cSprite3D; 
    m_Sprite->Initialize();

    return GetSprite3D() != NULL;
}

void iGameObjectVis::DestroySprite()
{
    if(!GetSprite3D())
        return;

    delete m_Sprite;
    m_Sprite = NULL;
}