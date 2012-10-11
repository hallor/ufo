#pragma once
#include <string>
#include <vector>
#include "GameObject.h"
#include "Terrain.h"

class Terrain;
class Vehicle;

class Level : public iGameObject
{
    DECLARE_GAME_OBJECT(Level, iGameObject);

public:
    Level();

    virtual void OnCreate();
    virtual void OnDestroy();

    bool Load(const std::string &file);
    
    void Unload();

    virtual void OnLogicUpdate();

    Vehicle* GetVehicle(UINT id) const { return m_Vehicles[id]; }
    UINT GetVehiclesCount() const { return m_Vehicles.size(); }

    const Terrain* GetTerrain() const { return m_Terrain; }

    bool IsLoaded() const { return GetTerrain() ? GetTerrain()->IsLoaded() : false; }

protected:

    void CreateVehicles();
    void DestroyVehicles();

    Terrain* m_Terrain;
    std::vector<Vehicle*> m_Vehicles;
};