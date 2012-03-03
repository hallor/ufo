#include "Game.h"
#include "AppSettings.h"
#include "EngineSettings.h"
#include "Level.h"
#include "pckfile.h"

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

    Level level;
    level.Load("xcoma\\ufodata\\citymap1");
    level.Unload();

    MainLoop();
    
    OnExit();

    return 0;
}

void Game::OnInputGameAction(EGameAction::TYPE action)
{
    switch(action)
    {
        case EGameAction::QUIT_GAME: m_Running = false; break;
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

    AppSettings::Free();
    EngineSettings::Free();

    ClearSDL();
}

bool Game::Initialize()
{
    if(!InitSDL())
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

        for(int i = 0; i < EngineSettings::GetMaxLogicUpdatesPerFrame() && m_Accumulator.Check(); ++i)
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
    SDL_Flip(m_MainSurface);
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

SDL_Surface *Game::LoadTexture(const std::string &tex)
{
    if(!tex.length())
        return NULL;

    SDL_Surface *tmp = SDL_LoadBMP(tex.c_str());
    if(!tmp)
        return NULL;

    SDL_Surface *ret = SDL_DisplayFormat(tmp);
    
    SDL_FreeSurface(tmp);

    SDL_SetColorKey(ret, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(ret->format, 255, 0, 255));

    return ret;
}