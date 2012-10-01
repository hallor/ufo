#pragma once
#include "TextureCache.h"

class Level;

// this struct could be created using malloc rather than new
struct STerrainElement
{
    STerrainElement(int id, const vec3& pos)
    : m_Id(id)
    , m_Pos(pos)
    {
    }

    STerrainElement()
    {
        m_Id = -1;
        m_Pos = vec3::ZERO;
    }

    int m_Id;
    vec3 m_Pos;

    static const STerrainElement s_DummyElement;

    static inline bool IsDummy(const STerrainElement& elem)
    {
        return &elem == &s_DummyElement;
    }
};

class Terrain
{
public: 
    Terrain(Level* owner);
    ~Terrain();

    bool Load(const std::string& file);
    void Unload();

    void Update(float dt);

    const cTextureCache& GetTextureCache() const { return m_TextureCache; }

    const STerrainElement& GetTileAt(const vec3 &pos) const;

    int GetWidth() const	{ return (int)m_Dimensions.x; }
    int GetHeight()	const	{ return (int)m_Dimensions.y; }
    int GetBreadth() const	{ return (int)m_Dimensions.z; }

    bool IsLoaded() const { return m_Loaded; }

    Level* GetOwner() const { return m_Owner; }

private:
    
    bool CreateTiles(unsigned int count);
    void DestroyTiles();

    void CacheTextures();

    Level* m_Owner;

    vec3 m_Dimensions;

    FixedArray<STerrainElement> *m_Tiles;
    
    cTextureCache m_TextureCache;
    std::string m_TextureSetPrefix;

    bool m_Loaded;
};