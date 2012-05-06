#include "Game.h"
#include "AppSettings.h"
#include "EngineSettings.h"
#include "TextureManager.h"
#include "SoundSourceManager.h"
#include "SoundBufferManager.h"

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

void Game::OnInputGameAction(EGameAction::TYPE action)
{
    switch(action)
    {
        case EGameAction::QUIT_GAME: m_Running = false; break;
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
    
    m_GameTimer.Start();
    m_Accumulator = Accumulator<float>(s_LogicStep);

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

}

void Game::Draw()
{    

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
