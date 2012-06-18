#pragma once
#include "compat_sdl.h"
#include <vector>
#include "RendererBase.h"
#include "vec.h"
#include "Texture.h"

class vSprite3DProperties;
class iCamera;
class iGameObject;

class vGameObjectRenderer : public iRendererBase
{
public:
    vGameObjectRenderer();
    virtual ~vGameObjectRenderer();

    virtual bool Initialize(const cTexture &error_texture);

    virtual void SetCamera(iCamera *cam) { m_Camera = cam; }

    virtual bool IsValid() const;

    virtual void OnFrame(float dt);

    virtual void Render(vRenderable *object);
    virtual void Render(iGameObject *igo);

    virtual void SetOffset(const vec3 &vec) { m_Offset = vec; }

    iCamera *GetCamera() const { return m_Camera; }

protected:

    bool InitSDL();

    vec3 m_Offset;

    iCamera *m_Camera;
    SDL_Surface *m_MainSurface;

    cTexture m_ErrorTexture;

    std::vector<vSprite3DProperties*> m_SpritesToRender;    
};
