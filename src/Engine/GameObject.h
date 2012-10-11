#pragma once
#include "HandlerTypes.h"
#include "GameObjectDef.h"

class iGameObject
{
public:
    iGameObject();
    virtual ~iGameObject();

    virtual const char* GetObjectClassName() = 0;

    virtual void OnCreate() {};
    virtual void OnDestroy() {};

    virtual void Delete();
  
    virtual void OnRenderFrame() {};
    virtual void OnLogicUpdate() {};

    virtual bool IsDestroying() const { return m_IsDestroying; }

protected:
    virtual void RegisterHandler(EEngineHandler::TYPE handler);
    virtual void UnregisterHandler(EEngineHandler::TYPE handler);

private:
    bool m_IsDestroying;
};