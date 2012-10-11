#include "game_pch.h"
#include "Level.h"
#include "Game.h"
#include "Terrain.h"
#include "Vehicle.h"

Level::Level()
: m_Terrain(NULL)
{
};

void Level::OnCreate()
{
    __super::OnCreate();
    Load("resources/ufodata/citymap1");
    RegisterHandler(EEngineHandler::HandlerOnLogicUpdate);
}

void Level::OnDestroy()
{
    __super::OnDestroy();
    Unload();
}

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
    DestroyVehicles();

    if (m_Terrain)
        delete m_Terrain;

    m_Terrain = nullptr;
};

void Level::OnLogicUpdate()
{
    if(!IsLoaded())
        return;

    m_Terrain->Update(0.0f);

    CreateVehicles();
}

void Level::CreateVehicles()
{
    static UINT count = 10;

    for (UINT i = m_Vehicles.size(); i < count; ++i)
        m_Vehicles.push_back(g_GOFactory.CreateObject<Vehicle>());
}

void Level::DestroyVehicles()
{
    for (UINT i = 0; i < m_Vehicles.size(); ++i)
        m_Vehicles[i]->Delete();
    
    m_Vehicles.clear();
}