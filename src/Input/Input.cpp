#include "Input.h"
#include "Game.h"

sActionKey::sActionKey()
{
    Clear();
}

sActionKey::sActionKey(SDLKey key)
{
    Clear();
    m_Key = key;
}

sActionKey::sActionKey(Uint8 button)
{
    Clear();
    m_KeyboardInput = false;
    m_Button = button;
}

void sActionKey::Clear()
{
    m_KeyboardInput = true;
    m_Key = SDLK_UNKNOWN;
    m_Button = Uint8(-1);
}

cInput::cInput()
{
    ClearKeyStates();
}

bool cInput::Initialize()
{    
    // temporary key bindings, need to load from .cfg
    m_KeyForAction[EGameAction::QUIT_GAME] = sActionKey(SDLK_ESCAPE);
    m_KeyForAction[EGameAction::MOVE_LEFT] = sActionKey(SDLK_LEFT);
    m_KeyForAction[EGameAction::MOVE_RIGHT] = sActionKey(SDLK_RIGHT);
    m_KeyForAction[EGameAction::MOVE_UP] = sActionKey(SDLK_UP);
    m_KeyForAction[EGameAction::MOVE_DOWN] = sActionKey(SDLK_DOWN);

    return true;
}

void cInput::OnSDLEvent(SDL_Event *evn)
{
    if(!evn)
        return;

    EGameAction::TYPE action = EGameAction::NONE;
    bool pressed = false;

    switch(evn->type)
    {
        case SDL_KEYDOWN:
        {
            action = GetActionForKey(evn->key.keysym.sym);
            pressed = evn->key.state == SDL_PRESSED;
        }; break;

        case SDL_MOUSEBUTTONDOWN:
        {
            action = GetActionForButton(evn->button.button);
            pressed = evn->button.state == SDL_PRESSED;
        }; break;

        default:
            return;
    }

    if(action == EGameAction::NONE)
        return;

    if(pressed)
        Game::GetSingleton()->OnInputGameAction(action);       
}

void cInput::ClearKeyStates()
{
    for(int i = 0; i < EGameAction::_COUNT; ++i)
        m_KeyForAction[i].Clear();
}

bool cInput::GetKeyState(SDLKey key) const
{
    return SDL_GetKeyState(NULL)[key] != 0;
}

bool cInput::GetButtonState(Uint8 button) const
{
    return (SDL_GetMouseState(NULL, NULL) & button) != 0;
}

bool cInput::GetActionKeyState(EGameAction::TYPE action) const
{
    sActionKey key = GetKeyForAction(action);

    if(key.m_Key == SDLK_UNKNOWN && key.m_KeyboardInput)
        return false;

    if(key.m_Button == Uint8(-1) && !key.m_KeyboardInput)
        return false;

    return key.m_KeyboardInput ? SDL_GetKeyState(NULL)[key.m_Key] != 0 : (SDL_GetMouseState(NULL, NULL) & key.m_Button) != 0;
}

vec3 cInput::GetMousePos() const
{
    int x = 0, y = 0;
    SDL_GetMouseState(&x, &y);

    return vec3((float)x, (float)y, 0.0f);
}

EGameAction::TYPE cInput::GetActionForKey(SDLKey key) const
{
    for(int i = 0; i < EGameAction::_COUNT; ++i)
    {
        if(m_KeyForAction[i].m_Key == key && m_KeyForAction[i].m_KeyboardInput)
            return (EGameAction::TYPE)(i);
    }

    return EGameAction::NONE;
}

EGameAction::TYPE cInput::GetActionForButton(Uint8 button) const
{
    for(int i = 0; i < EGameAction::_COUNT; ++i)
    {
        if(m_KeyForAction[i].m_Button == button && !m_KeyForAction[i].m_KeyboardInput)
            return (EGameAction::TYPE)(i);
    }

    return EGameAction::NONE;
}

sActionKey cInput::GetKeyForAction(EGameAction::TYPE action) const
{
    if(action >= EGameAction::_COUNT)
        return sActionKey();
        
    return m_KeyForAction[action];
}