#include "Sprite3D.h"
#include "GameObject.h"
#include "Texture.h"
#include "Game.h"
#include "TextureManager.h"

cSprite3D::cSprite3D()
{
     __super::m_RenderableType = ERenderableType::Sprite3D;
    DefaultPosition();
}

void cSprite3D::PrepareForRendering()
{
    vSprite3DProperties *prop = GetProperties();

    if(!prop)
        return;

    prop->Synchronize(this);
}

bool cSprite3D::CreateRenderingProperties()
{
    if(GetProperties())
        return true;

    vSprite3DProperties *tmp = new (std::nothrow) vSprite3DProperties;
    
    __super::m_RenderingProperties = tmp;

    return GetProperties() != NULL;
}

vSprite3DProperties *cSprite3D::GetProperties() const
{
    return dynamic_cast<vSprite3DProperties*>(GetRenderingProperties());
}

vSprite3DProperties::vSprite3DProperties()
{
    DefaultPosition();
    DefaultTexture();
}

vSprite3DProperties::~vSprite3DProperties()
{
    if(GetTexture())
        GetTexture()->Release();
}

void vSprite3DProperties::Synchronize(const vRenderable *object)
{
    const cSprite3D *sprite = dynamic_cast<const cSprite3D*>(object);

    if(!sprite)
        return;

    SetPosition(sprite->GetPosition());

    if(m_TextureName != sprite->GetTexture())
    {
        if(GetTexture())
            GetTexture()->Release();
        
        SetTexture(NULL);

        if(Game::GetSingleton()->GetTextureManager())
            SetTexture(Game::GetSingleton()->GetTextureManager()->Get(sprite->GetTexture()));
    }

    m_TextureName = sprite->GetTexture();
}