#include "GameObjectRenderer.h"
#include "GameObject.h"
#include "GameObjectVis.h"
#include "Sprite3D.h"
#include "Texture.h"
#include "CameraBase.h"
#include "AppSettings.h"
#include "Game.h"

vGameObjectRenderer::vGameObjectRenderer()
: m_ErrorTexture(NULL)
{
    m_MainSurface = NULL;
    m_Camera = NULL;
}

vGameObjectRenderer::~vGameObjectRenderer()
{
}

bool vGameObjectRenderer::Initialize(const cTexture &error_texture)
{
    m_ErrorTexture = error_texture;

    return InitSDL();
}

bool vGameObjectRenderer::IsValid() const
{
    return m_MainSurface && m_Camera;
}

void vGameObjectRenderer::OnFrame(float dt)
{
    if(!IsValid())
        return;

    vec3 screen_tl = vec3::ZERO;
    vec3 screen_br = vec3::ZERO;
    screen_br.x = (float)AppSettings::GetWindowWidth();
    screen_br.y = (float)AppSettings::GetWindowHeight();

    for(unsigned int i = 0; i < m_SpritesToRender.size(); ++i)
    {
        vSprite3DProperties *sprite = m_SpritesToRender[i];
        cTexture tex = sprite->GetTexture();
        if(!tex.Get())
            tex = m_ErrorTexture;
    
        SDL_Surface *real_texture = tex.Get();

        if(!real_texture)
            continue;

        vec3 screen_pos = GetCamera()->PointWorldToScreen(sprite->GetPosition()) + m_Offset;
        vec3 tex_size = tex.GetSize();
        //screen_pos -= tex_size / 2.0f;

        if(screen_pos.x > screen_br.x || screen_pos.y > screen_br.y || 
           screen_pos.x + tex_size.x < screen_tl.x || screen_pos.y + tex_size.y < screen_tl.y)
           continue;

        SDL_Rect draw_rect;
        draw_rect.x = (unsigned short)screen_pos.x;
        draw_rect.y = (unsigned short)screen_pos.y;
        draw_rect.w = (unsigned short)tex_size.x;
        draw_rect.h = (unsigned short)tex_size.y;

        SDL_BlitSurface(real_texture, NULL, m_MainSurface, &draw_rect);
    }

    SDL_Flip(m_MainSurface);

    m_SpritesToRender.clear();
}

void vGameObjectRenderer::Render(vRenderable *object)
{
    if(!object)
        return;
    
    if(object->GetRenderableType() != ERenderableType::Sprite3D)
        return;

    vSprite3DProperties *props = dynamic_cast<vSprite3DProperties*>(object->GetRenderingProperties());

    if(!props)
        return;

    m_SpritesToRender.push_back(props);
}

void vGameObjectRenderer::Render(iGameObject *igo)
{
    if(!igo || !igo->GetVis())
        return;

    Render(igo->GetVis()->GetSprite3D());
}

bool vGameObjectRenderer::InitSDL()
{
    m_MainSurface = Game::GetSingleton()->GetSDLMainSurface();

    return m_MainSurface != NULL;
}