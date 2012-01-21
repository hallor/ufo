#include <cstdlib>
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

void iGameObject::DestroyVis()
{
    if(GetVis())
    {
        GetVis()->OnDestroy();
        delete GetVis();
        m_Visualisation = NULL;
    }
}
