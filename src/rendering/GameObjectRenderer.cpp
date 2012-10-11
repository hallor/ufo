#include "game_pch.h"
#include <algorithm>
#include "GameObjectRenderer.h"
#include "ModelObject.h"
#include "Sprite3D.h"
#include "Texture.h"
#include "CameraBase.h"
#include "AppSettings.h"
#include "Game.h"
#include "ModelObject.h"
#include "ModelObjectVis.h"

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

bool compare_datas(const SRenderData& d1, const SRenderData& d2)
{
    const vec3& pos1 = d1.m_Pos;
    const vec3& pos2 = d2.m_Pos;

    return pos1.Length2() < pos2.Length2();
}

void vGameObjectRenderer::SortRenderingData()
{
    std::sort(m_RenderData.begin(), m_RenderData.end(), compare_datas);
}

void vGameObjectRenderer::OnFrame(float dt)
{
    if(!IsValid())
        return;

    SortRenderingData();

    vec3 screen_tl = vec3::ZERO;
    vec3 screen_br = vec3::ZERO;
    screen_br.x = (float)AppSettings::GetWindowWidth();
    screen_br.y = (float)AppSettings::GetWindowHeight();

    for (unsigned int i = 0; i < m_RenderData.size(); ++i)
    {
        SRenderData& data = m_RenderData[i];
        cTexture* tex = data.m_Texture.Get() ? &data.m_Texture : m_ErrorTexture.Get() ? &m_ErrorTexture : NULL;

        if (!tex)
            continue;

        vec3 screen_pos = GetCamera()->PointWorldToScreen(data.m_Pos) + m_Offset;
        vec3 size = tex->GetSize();

        if(screen_pos.x > screen_br.x || screen_pos.y > screen_br.y || 
            screen_pos.x + size.x < screen_tl.x || screen_pos.y + size.y < screen_tl.y)
            continue;

        SDL_Rect draw_rect;
        draw_rect.x = (unsigned short)screen_pos.x;
        draw_rect.y = (unsigned short)screen_pos.y;
        draw_rect.w = (unsigned short)size.x;
        draw_rect.h = (unsigned short)size.y;

        SDL_BlitSurface(tex->Get(), NULL, m_MainSurface, &draw_rect);
    }

    SDL_Flip(m_MainSurface);

    m_RenderData.clear();
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

    SRenderData data(props->GetPosition(), props->GetTexture());

    m_RenderData.push_back(data);
}

void vGameObjectRenderer::Render(iModelObject *igo)
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