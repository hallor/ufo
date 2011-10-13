#pragma once
#include "RendererBase.h"
#include "RenderableBase.h"

class vSoundRenderer : public iRendererBase
{
public:
    vSoundRenderer(){};
    ~vSoundRenderer(){};
    
    virtual bool IsValid() { return false; }

    virtual void OnFrame(){};

    virtual void Render(const vRenderable &object){};

protected:

};