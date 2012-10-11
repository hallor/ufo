#pragma once
#include "Factions.h"
#include "ModelObject.h"

class VehicleVis;

class Vehicle : public iModelObject
{
    DECLARE_GAME_OBJECT(Vehicle, iModelObject);

public:
    Vehicle();

    virtual void OnCreate();

    virtual void OnLogicUpdate();
    virtual VehicleVis* GetVehicleVis() const { return (VehicleVis*)GetVis(); }

protected:
    virtual iModelObjectVis* CreateVis();

private:

    float m_LastDirectionChange;

    static float s_DirectionChangeTime;
    static float s_MoveSpeed;
};