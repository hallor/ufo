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

bool iGameObjectVis::SetTexture(const std::string &texture)
{
    if(!GetSprite3D())
        return false;

    GetSprite3D()->SetTexture(texture);
    return false;
}

bool iGameObjectVis::InitializeSprite()
{
    if(GetSprite3D())
       return true;
       
    m_Sprite = new (std::nothrow) cSprite3D; 

    return GetSprite3D() != NULL;
}

void iGameObjectVis::DestroySprite()
{
    if(!GetSprite3D())
        return;

    delete m_Sprite;
    m_Sprite = NULL;
}