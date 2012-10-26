#pragma once
#include "compat_sdl.h"
#include <string>
#include "GameActions.h"
#include "vec.h"
#include "Timer.h"
#include "Input.h"
#include "Accumulator.h"
#include "GameTime.h"

class cTextureManager;
class cSoundBufferManager;
class cSoundSourceManager;
class Level;

class Game
{
public:
    static Game* GetSingleton();
    static void Release();
    
    cTextureManager     *GetTextureManager() const { return m_TextureManager; }
    cSoundBufferManager *GetSoundBufferManager() const { return m_SoundBufferManager; }
    cSoundSourceManager *GetSoundSourceManager() const { return m_SoundSourceManager; }

    int  OnExecute(int argc, char *argv[]);
    void OnInputGameAction(EGameAction::TYPE type);
    void OnSDLEvent(SDL_Event *event);
    void OnExit();
    
    const cInput *GetInput() { return &m_Input; }

    SDL_Surface *GetSDLMainSurface() const { return m_MainSurface; }

    const GameTime& GetGameTimer() const { return m_GameTimer; }

protected:

    Game();
    ~Game();

    bool Initialize();

    void MainLoop();

    void UpdateSDLEvents();

    void Update(float dt);
    
    // drawing, move it
    void Draw();

    // SDL, move it
    bool InitSDL();
    void ClearSDL();

    cTimer m_RealTimer;
    GameTime m_GameTimer;
    
    Level *m_Level;

    bool m_Running;
    cInput m_Input;
    Accumulator<float> m_Accumulator;

    static float s_LogicStep;

    // sdl, move it
    SDL_Surface *m_MainSurface;

    bool InitManagers();
    void ReleaseManagers();
    
    cTextureManager     *m_TextureManager;
    cSoundBufferManager *m_SoundBufferManager;
    cSoundSourceManager *m_SoundSourceManager;
};
