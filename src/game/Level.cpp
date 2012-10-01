#include "game_pch.h"
#include "Level.h"
#include "Game.h"
#include "Terrain.h"


Level::Level()
: m_Terrain(NULL)
{
};

bool Level::Load(const std::string &file)
{
    Unload();
    
    m_Terrain = new (std::nothrow) Terrain(this);
    
    if (!m_Terrain)
        return false;

    return m_Terrain->Load(file);
};

void Level::Unload()
{
    if (m_Terrain)
        delete m_Terrain;

    m_Terrain = nullptr;
};

void Level::Update(float dt)
{
    if(!IsLoaded())
        return;

    m_Terrain->Update(dt);
}
