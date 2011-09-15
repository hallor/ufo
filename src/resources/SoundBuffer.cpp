#include "SoundBuffer.h"

vSoundBufferResource::vSoundBufferResource()
{
    m_Type = EResourceType::OalSoundBuffer;
}

vSoundBufferResource::~vSoundBufferResource()
{
    m_Type = EResourceType::None;
}

cSoundBuffer::cSoundBuffer(vSoundBufferResource *res)
{
    m_Resource = res;
}

ALuint cSoundBuffer::Get() const
{
    return m_Resource ? m_Resource->Get() : 0;
}

