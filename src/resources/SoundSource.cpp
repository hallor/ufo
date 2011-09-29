#include "SoundSource.h"

vSoundSourceResource::vSoundSourceResource(const ALuint &res)
: vResource(res)
{
    m_Type = EResourceType::OalSoundSource;
}

vSoundSourceResource::~vSoundSourceResource()
{
    m_Type = EResourceType::None;
}

cSoundSource::cSoundSource(vSoundSourceResource *res)
{
    m_Resource = res;
}

ALuint cSoundSource::Get() const
{
    return m_Resource ? m_Resource->Get() : 0;
}

