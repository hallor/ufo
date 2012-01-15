#pragma once
#include <SDL\SDL.h>
#include <string>
#include "GameActions.h"
#include "vec.h"
#include "Timer.h"
#include "Input.h"
#include "Accumulator.h"

class Game
{
public:
    static Game* GetSingleton();
    static void Release();

    int  OnExecute(int argc, char *argv[]);
    void OnInputGameAction(EGameAction::TYPE type);
    void OnSDLEvent(SDL_Event *event);
    void OnExit();
    
    const cInput *GetInput() { return &m_Input; }

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
    SDL_Surface *LoadTexture(const std::string &tex);

    cTimer m_GameTimer;
    
    bool m_Running;
    cInput m_Input;
    Accumulator<float> m_Accumulator;

    static float s_LogicStep;

    // sdl, move it
    SDL_Surface *m_MainSurface;
};