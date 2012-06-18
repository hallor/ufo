#pragma once
#include <vector>
#include <string>
#include "Texture.h"

class cTextureManager;

class cTextureCache
{
public:
    cTextureCache();
    ~cTextureCache();

    int SetTextureManager(cTextureManager *man);

    int CreateCache(const std::string &path_base, int count);

    int GetCacheSize() const { return (int)m_Textures.size(); }

    cTexture GetCachedTexture(int id) const;

    void ClearCache();

    cTextureManager* GetTextureManager() const { return m_TextureManager; }

protected:

    std::string GetPathForId(const std::string &base, int id) const;
    int RebuildCache(const std::string &base, int id);
    
    std::string m_TextureNameBase;

    std::vector<cTexture> m_Textures;

    cTextureManager *m_TextureManager;
};