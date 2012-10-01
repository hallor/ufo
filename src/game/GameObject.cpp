#include "game_pch.h"
#include "GameObject.h"
#include "GameObjectVis.h"

iGameObject::iGameObject()
{
    m_Visualisation = NULL;
}

iGameObject::~iGameObject()
{
}

void iGameObject::OnCreate()
{
    m_Visualisation = CreateVis();
    if(GetVis())
        GetVis()->OnCreate();
}

void iGameObject::OnDestroy()
{
    DestroyVis();
}

void iGameObject::UpdateVisualisation(float dt)
{
    if(GetVis())
        GetVis()->Update(dt);
}

void iGameObject::OnPreRender()
{
    if(GetVis())
        GetVis()->OnPreRender();
}

void iGameObject::DestroyVis()
{
    iGameObjectVis *vis = GetVis();

    if(vis)
    {
        vis->OnDestroy();
        delete vis;
        m_Visualisation = NULL;
    }
}
