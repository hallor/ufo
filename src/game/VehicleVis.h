#pragma once
#include "ModelObjectVis.h"

class Vehicle;

class VehicleVis : public iModelObjectVis
{
    DECLARE_SUPER(iModelObjectVis);
public:
    VehicleVis(iModelObject* parent);

    virtual void OnCreate();

    virtual Vehicle* GetSim() const { return static_cast<Vehicle*>(GetParent()); }

protected:
};