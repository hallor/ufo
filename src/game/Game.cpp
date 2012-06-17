#include "Game.h"
#include "AppSettings.h"
#include "EngineSettings.h"
#include "TextureManager.h"
#include "SoundSourceManager.h"
#include "SoundBufferManager.h"
#include "LevelTile.h"
#include "Level.h"
#include "GameObjectRenderer.h"
#include "CityScapeCamera.h"

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
float layer = 0;

void Game::OnInputGameAction(EGameAction::TYPE action)
{
    switch(action)
    {
        case EGameAction::QUIT_GAME: m_Running = false; break;
        case EGameAction::MOVE_UP: ++layer; if(layer > 9) layer = 9; break;
        case EGameAction::MOVE_DOWN: --layer; if(layer < 0) layer = 0; break;
        default: break;
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

Level level;
vGameObjectRenderer renderer;

void Game::OnExit()
{
    m_Running = false;

    level.Unload();
    
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
        
    level.Load("resources\\citymap1");
    level.Update(0.0f);

    s_LogicStep = 1.0f / EngineSettings::GetLogicUpdateFrequency();

    m_Input.Initialize();
    
    m_GameTimer.Start();
    m_Accumulator = Accumulator<float>(s_LogicStep);

    renderer.Initialize(NULL);
    renderer.SetCamera(&cam);

    return true;
}

void Game::MainLoop()
{        
    while(m_Running)
    {
        m_GameTimer.Tick();

        m_Accumulator.Accumulate(m_GameTimer.GetTimeDelta());

        UpdateSDLEvents();

        for(unsigned int i = 0; i < EngineSettings::GetMaxLogicUpdatesPerFrame() && m_Accumulator.Check(); ++i)
            Update(s_LogicStep);
                    
        Draw();
    }
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
    static float move_speed = 200;
    vec3 move_vec = vec3::ZERO;
    move_vec.x -= GetInput()->GetActionKeyState(EGameAction::MOVE_LEFT) ? 1.0f : 0.0f;
    move_vec.x += GetInput()->GetActionKeyState(EGameAction::MOVE_RIGHT) ? 1.0f : 0.0f;
    move_vec.z -= GetInput()->GetActionKeyState(EGameAction::MOVE_UP) ? 1.0f : 0.0f;
    move_vec.z += GetInput()->GetActionKeyState(EGameAction::MOVE_DOWN) ? 1.0f : 0.0f;

    move_vec.Normalize();

    cam.Move(move_vec * move_speed * dt);
}

void Game::Draw()
{        
    if(GetSDLMainSurface())
        SDL_FillRect(GetSDLMainSurface(), NULL, 0);

    for(int j = 0; j < 100; ++j)
    {
        for(int i = 0; i < 100; ++i)
        {
            for(int k = 0; k < 10; ++k)
            {
                LevelTile *tile = level.GetTileAt(vec3(i, k, j));
                if(tile && tile->GetId() != 0)
                {
                    tile->OnPreRender();
                    renderer.Render(tile);
                }
            }
        }
    }
    
    renderer.OnFrame(0.0f);
}

bool Game::InitSDL()
{
    ClearSDL();

    if(SDL_Init(SDL_INIT_EVERYTHING))
        return false;

    m_MainSurface = SDL_SetVideoMode(AppSettings::GetWindowWidth(), AppSettings::GetWindowHeight(), 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    
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
