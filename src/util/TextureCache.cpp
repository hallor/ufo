#include <stdio.h>
#include "TextureCache.h"
#include "TextureManager.h"
#include "logger.h"

cTextureCache::cTextureCache()
{
    m_TextureManager = NULL;
}

cTextureCache::~cTextureCache()
{
    ClearCache();
}

int cTextureCache::SetTextureManager(cTextureManager *man)
{
    if(man == GetTextureManager())
        return 0;

    m_TextureManager = man;
    return RebuildCache(m_TextureNameBase, m_Textures.size());
}

int cTextureCache::CreateCache(const std::string &path_base, int count)
{
    ClearCache();
    
    if(!GetTextureManager() || path_base.empty())
        return 0;

    m_TextureNameBase = path_base;
    int loaded = 0;

    for(int i = 0; i < count; ++i)
    {
        m_Textures.push_back(GetTextureManager()->Get(GetPathForId(path_base, i)));
        if(m_Textures.back().Get())
            ++loaded;
    }

    LogInfo("Created texture cache for path %s, loaded %i of %i textures", path_base.c_str(), loaded, count);
    
    return loaded;
}

cTexture cTextureCache::GetCachedTexture(int id) const
{
    if(id >= (int)m_Textures.size())
    {
        if(m_TextureManager)
            return m_TextureManager->Get(GetPathForId(m_TextureNameBase, id));
        else
            return cTexture(NULL);
    }

    return m_Textures[id];
}

void cTextureCache::ClearCache()
{
    if(!m_Textures.size())
        return;

    LogInfo("Clearing %i textures from cache.", m_Textures.size());
    m_Textures.clear();
    m_TextureNameBase.clear();
}

std::string cTextureCache::GetPathForId(const std::string &base, int id) const
{
    char id_str[32] = {0};

    snprintf(id_str, 31, "%d", id);
    std::string name = base;
    name += id_str;
    name += ".bmp";

    return name;
}

int cTextureCache::RebuildCache(const std::string &base, int id)
{
    LogInfo("Rebuilding texture cache.");
    ClearCache();
    return CreateCache(base, id);
}
