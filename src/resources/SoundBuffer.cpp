#include "game_pch.h"
#include "SoundBuffer.h"
#ifdef _WIN32
#include <oal/al.h>
#else
#include <AL/al.h>
#endif

vSoundBufferResource::vSoundBufferResource(const ALuint &res)
: vResource(res)
{
    m_Type = EResourceType::OalSoundBuffer;
}

vSoundBufferResource::~vSoundBufferResource()
{
    m_Type = EResourceType::None;
}

bool vSoundBufferResource::IsValid() const
{
    return GetState() == EResourceState::Ok && alIsBuffer(Get()) == AL_TRUE;
}

cSoundBuffer::cSoundBuffer(vSoundBufferResource *res)
{
    m_Resource = res;
}

ALuint cSoundBuffer::Get() const
{
    return m_Resource ? m_Resource->Get() : 0;
}

