#include "game_pch.h"
#include "RenderableBase.h"
#include <string>

vRenderable::vRenderable()
{
    // Type has to be set in constructor for every specialised clas
    m_RenderableType = ERenderableType::Unknown;
    
    m_RenderingProperties = NULL;
};

vRenderable::~vRenderable()
{
    DeleteRenderingProperties();
};

bool vRenderable::Initialize()
{
    // Do not reinitialize
    if(IsValid())
        return true;

    CreateRenderingProperties();

    return IsValid();
}

void vRenderable::PrepareForRendering()
{
    // Nothing to be done here
}

bool vRenderable::IsValid() const
{
    if(!GetRenderingProperties())
        return false;

    if(!ERenderableType::IsValidType(GetRenderableType()))
        return false;

    return true;
}

// This method cannot succed as it is not possible to create properties for Unknown type
bool vRenderable::CreateRenderingProperties()
{   
    return false;
};

void vRenderable::DeleteRenderingProperties()
{
    if(m_RenderingProperties)
    {
        delete m_RenderingProperties;
        m_RenderingProperties = NULL;
    }
};
