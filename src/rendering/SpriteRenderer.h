#pragma once
#include <SDL\SDL.h>
#include <vector>
#include "RendererBase.h"

class vSprite3DProperties;
class cTexture;
class iCamera;

class vSpriteRenderer : public iRendererBase
{
public:
    vSpriteRenderer();
    virtual ~vSpriteRenderer();

    virtual bool Initialize(cTexture* error_texture);

    virtual void SetCamera(iCamera *cam) { m_Camera = cam; }

    virtual bool IsValid() const;

    virtual void OnFrame(float dt);

    virtual void Render(const vRenderable &object);

    iCamera *GetCamera() const { return m_Camera; }

protected:

    bool InitSDL();

    iCamera *m_Camera;
    SDL_Surface *m_MainSurface;

    cTexture *m_ErrorTexture;

    std::vector<vSprite3DProperties*> m_SpritesToRender;    
};