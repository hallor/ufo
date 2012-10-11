#include "game_pch.h"
#include "ModelObject.h"
#include "ModelObjectVis.h"

iModelObject::iModelObject()
{
    m_Visualisation = nullptr;
}

void iModelObject::OnCreate()
{
    __super::OnCreate();

    m_Visualisation = CreateVis();
    if(GetVis())
        GetVis()->OnCreate();

}

void iModelObject::OnDestroy()
{
    DestroyVis();
    __super::OnDestroy();
}

void iModelObject::OnRenderFrame()
{
    if (GetVis())
        GetVis()->OnRenderFrame();
}

void iModelObject::DestroyVis()
{
    iModelObjectVis *vis = GetVis();

    if(vis)
    {
        vis->OnDestroy();
        delete vis;
        m_Visualisation = nullptr;
    }
}
