#include "Texture.h"

vTextureResource::vTextureResource(SDL_Surface* const &res)
: vResource(res)
{
    m_Type = EResourceType::SDLSurface;
}

vTextureResource::~vTextureResource()
{
    m_Type = EResourceType::None;
}

bool vTextureResource::IsValid() const
{
    return GetState() == EResourceState::Ok && GetSize().x > 0 && GetSize().y > 0;
}

vec2 vTextureResource::GetSize() const
{
    return vec2::ZERO;
}

cTexture::cTexture(vTextureResource *res)
{
    m_Resource = res;
}

SDL_Surface *cTexture::Get() const
{
    return m_Resource ? m_Resource->Get() : 0;
}

