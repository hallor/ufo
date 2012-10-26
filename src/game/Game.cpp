#include "game_pch.h"
#include <stdio.h>
#include <algorithm>
#include "Game.h"
#include "AppSettings.h"
#include "EngineSettings.h"
#include "TextureManager.h"
#include "SoundSourceManager.h"
#include "SoundBufferManager.h"
#include "Level.h"
#include "GameObjectRenderer.h"
#include "CityScapeCamera.h"
#include "game/Terrain.h"
#include "game/Vehicle.h"
#include "GameTime.h"

Game* g_GameInstance = NULL;

float Game::s_LogicStep = 1.0f / 60.0f; // 60 updates/second

Game *Game::GetSingleton()
{
    if(!g_GameInstance)
        g_GameInstance = new Game();

    return g_GameInstance;
}

void Game::Release()
{
    if(g_GameInstance)
        delete g_GameInstance;

    g_GameInstance = NULL;
}

Game::Game()
{
    m_Running = false;

    m_MainSurface = NULL;
    m_TextureManager = NULL;
    m_SoundBufferManager = NULL;
    m_SoundSourceManager = NULL;

    m_Level = nullptr;
}

Game::~Game()
{
    OnExit();
}

int Game::OnExecute(int argc, char *argv[])
{
    if(m_Running)
        return -1;

    if(!Initialize())
        return -2;

    m_Running = true;
        
    MainLoop();
    
    OnExit();

    return 0;
}

CityScapeCamera cam;
int l = 0;

void Game::OnInputGameAction(EGameAction::TYPE action)
{
    EGameSpeed::TYPE game_speed = m_GameTimer.GetGameSpeed();
    int wanted_level = l;
    switch(action)
    {
        case EGameAction::QUIT_GAME: m_Running = false; break;
        case EGameAction::LEVEL_NEXT: ++wanted_level; break;
        case EGameAction::LEVEL_PREV: --wanted_level; break;
        case EGameAction::GAME_SPEED_PAUSED: game_speed = EGameSpeed::SpeedPaused; break;
        case EGameAction::GAME_SPEED_5S: game_speed = EGameSpeed::Speed5s; break;
        case EGameAction::GAME_SPEED_1MIN: game_speed = EGameSpeed::Speed1min; break;
        case EGameAction::GAME_SPEED_15MIN: game_speed = EGameSpeed::Speed15min; break;
        case EGameAction::GAME_SPEED_1HOUR: game_speed = EGameSpeed::Speed1hour; break;
        case EGameAction::GAME_SPEED_1DAY: game_speed = EGameSpeed::Speed1day; break;
        default: break;
    }

    m_GameTimer.SetGameSpeed(game_speed);

    if(wanted_level > 4)
        wanted_level = 4;
    if(wanted_level < 0)
        wanted_level = 0;

    if(wanted_level != l)
    {
        char asd[32] = {0};

        sprintf(asd, "%d", wanted_level + 1);
        std::string name = "resources/ufodata/citymap";
        if(wanted_level > 0)
            name += asd;

        l = wanted_level;
        m_Level->Load(name);

        m_GameTimer.Reset();
    }
}

void Game::OnSDLEvent(SDL_Event *event)
{
    if(!event)
        return;

    if(event->type == SDL_QUIT)
        m_Running = false;

    m_Input.OnSDLEvent(event);
}

vGameObjectRenderer renderer;

void Game::OnExit()
{
    m_Running = false;
    
    ReleaseManagers();
    
    ClearSDL();

    AppSettings::Free();
    EngineSettings::Free();
}


bool Game::Initialize()
{
    if(!InitSDL())
        return false;

    if(!InitManagers())
        return false;

    s_LogicStep = 1.0f / EngineSettings::GetLogicUpdateFrequency();

    m_Input.Initialize();
    
    m_RealTimer.Start();
    m_Accumulator = Accumulator<float>(s_LogicStep);

    renderer.Initialize(NULL);
    renderer.SetCamera(&cam);
	renderer.SetOffset(vec3(640, 240, 0));

    m_Level = g_GOFactory.CreateObject<Level>();

    return true;
}

void Game::MainLoop()
{        
    while(m_Running)
    {
        m_RealTimer.Tick();

        m_Accumulator.Accumulate(m_RealTimer.GetTimeDelta());

        UpdateSDLEvents();

		bool draw_frame = false;

        for(unsigned int i = 0; i < EngineSettings::GetMaxLogicUpdatesPerFrame() && m_Accumulator.Check(); ++i)
		{
            m_GameTimer.Advance(s_LogicStep);
            Update(s_LogicStep);
			draw_frame = true;
		}           
	
		if (draw_frame)
        {
            g_GOFactory.CallHandler(EEngineHandler::HandlerOnRenderFrame);
	        Draw();
        }

        sHMSTime hmstime = m_GameTimer.GetTimeHMSFormat();

        char time[64] = {0};
        sprintf_s(time, "%s %i:%i:%i", EGameSpeed::ToString(m_GameTimer.GetGameSpeed()).c_str(), hmstime.m_Hours, hmstime.m_Minutes, hmstime.m_Seconds);

        SDL_WM_SetCaption(time, nullptr);
    }
    
    m_Level->Delete();
}

void Game::UpdateSDLEvents()
{
    SDL_Event event;

    SDL_PumpEvents();

    while(SDL_PollEvent(&event))
        OnSDLEvent(&event);
}

void Game::Update(float dt)
{
    g_GOFactory.CallHandler(EEngineHandler::HandlerOnLogicUpdate);
    
    static float move_speed = 20;
    vec3 move_vec = vec3::ZERO;
    if (GetInput()->GetActionKeyState(EGameAction::MOVE_LEFT))
	{
		move_vec.x += 1.0f;
		move_vec.z -= 1.0f;
	}

    if (GetInput()->GetActionKeyState(EGameAction::MOVE_RIGHT)) 
	{
		move_vec.x -= 1.0f;
		move_vec.z += 1.0f;
	}

    if (GetInput()->GetActionKeyState(EGameAction::MOVE_UP)) 
	{
		move_vec.z -= 1.0f;
		move_vec.x -= 1.0f;
	}

    if (GetInput()->GetActionKeyState(EGameAction::MOVE_DOWN))
	{
		move_vec.z += 1.0f;
		move_vec.x += 1.0f;
	}

    move_vec.Normalize();

    cam.Move(move_vec * move_speed * dt);

	if (GetInput()->GetKeyState(SDLK_r))
		cam.SetPos(vec3::ZERO);
}

void Game::Draw()
{        
    if(GetSDLMainSurface())
        SDL_FillRect(GetSDLMainSurface(), NULL, 0);

	vec3 cam_pos = cam.GetPos();

    const Terrain* terrain = m_Level->GetTerrain();
    if (terrain)
    {
        const cTextureCache& texture_cache = terrain->GetTextureCache();

        for(int j = (int)cam_pos.z - 10; j < terrain->GetBreadth() && j < (int)cam_pos.z + 25; ++j)
        {
            for(int i = (int)cam_pos.x - 10; i < terrain->GetWidth() && i < (int)cam_pos.x + 25; ++i)
            {
                for(int k = 0; k < terrain->GetHeight(); ++k)
                {
                    const STerrainElement& tile = terrain->GetTileAt(vec3((float)i, (float)k, (float)j));
                    if(tile.m_Id != 0 && !STerrainElement::IsDummy(tile))
                        renderer.Render(SRenderData(tile.m_Pos, texture_cache.GetCachedTexture(tile.m_Id)));
                }
            }
        }
    }

    for (UINT i = 0; i < m_Level->GetVehiclesCount(); ++i)
        renderer.Render(m_Level->GetVehicle(i));
    

    renderer.OnFrame(0.0f);
}

bool Game::InitSDL()
{
    ClearSDL();

    if(SDL_Init(SDL_INIT_EVERYTHING))
        return false;

    m_MainSurface = SDL_SetVideoMode(AppSettings::GetWindowWidth(), AppSettings::GetWindowHeight(), 32, SDL_ANYFORMAT | SDL_HWSURFACE | SDL_DOUBLEBUF);
    
    return m_MainSurface != NULL;
}

void Game::ClearSDL()
{
    m_MainSurface = NULL;

    SDL_Quit();
}

bool Game::InitManagers()
{
    if(!GetTextureManager())
        m_TextureManager = new cTextureManager;

    if(!GetSoundSourceManager())
        m_SoundSourceManager = new cSoundSourceManager;

    if(!GetSoundBufferManager())
        m_SoundBufferManager = new cSoundBufferManager;


    return GetTextureManager() && GetSoundSourceManager() && GetSoundBufferManager();
}

void Game::ReleaseManagers()
{
    if(GetTextureManager())
        delete GetTextureManager();

    if(GetSoundSourceManager())
        delete GetSoundSourceManager();

    if(GetSoundBufferManager())
        delete GetSoundBufferManager();

    m_TextureManager = NULL;
    m_SoundSourceManager = NULL;
    m_SoundBufferManager = NULL;
}
