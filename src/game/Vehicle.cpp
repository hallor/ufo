#include "game_pch.h"
#include "Vehicle.h"
#include "VehicleVis.h"

float Vehicle::s_DirectionChangeTime = 5.0f;
float Vehicle::s_MoveSpeed = 5.0f;

Vehicle::Vehicle()
: m_LastDirectionChange(0.0f)
{

}

void Vehicle::OnCreate()
{
    __super::OnCreate();
    SetPos(vec3(0.0f, 10.0f, 0.0f));
    SetWantedPos(GetPos());

    RegisterHandler(EEngineHandler::HandlerOnLogicUpdate);
    RegisterHandler(EEngineHandler::HandlerOnRenderFrame);
}

void Vehicle::OnLogicUpdate()
{
    //m_LastDirectionChange += dt;
    //if (m_LastDirectionChange >= s_DirectionChangeTime)
    //{
    //    m_LastDirectionChange -= s_DirectionChangeTime;
    //    SetWantedPos(vec3(rand() % 100, 6, rand() % 100));
    //}
    //
    //vec3 move_dir = GetWantedPos() - GetPos();
    //float move_dt = move_dir.Length() / (s_MoveSpeed * dt);

    //if (move_dt <= 1.0f)
    //    SetPos(GetWantedPos());
    //else 
    //    SetPos(GetPos() + move_dir.Normalize() * s_MoveSpeed * dt);

    __super::OnLogicUpdate();
}

iModelObjectVis* Vehicle::CreateVis()
{
    return new (std::nothrow) VehicleVis(this);
}
