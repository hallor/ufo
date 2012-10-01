#include "game_pch.h"
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

vec3 vTextureResource::GetSize() const
{
    vec3 size = vec3::ZERO;
    size.x = Get() ? Get()->w : 0.0f;
    size.y = Get() ? Get()->h : 0.0f;

    return size;
}

cTexture::cTexture(vTextureResource *res)
{
    m_Resource = res;
}

cTexture::cTexture(const cTexture &other)
{
    if(this == &other)
        return;

    m_Resource = other.m_Resource;
}

cTexture& cTexture::operator=(const cTexture& other)
{
    if(this == &other)
        return *this;

    m_Resource = other.m_Resource;
    return *this;
}

SDL_Surface *cTexture::Get() const
{
    return m_Resource ? m_Resource->Get() : 0;
}

