#pragma once
#include <SDL\SDL.h>
#include "GameActions.h"
#include "vec.h"

struct sActionKey
{
    sActionKey();
    sActionKey(SDLKey key);
    sActionKey(Uint8 button);

    void Clear();

    bool                m_KeyboardInput;
    SDLKey              m_Key;
    Uint8               m_Button;
};

class cInput
{
public:
    cInput();

    bool Initialize();

    void OnSDLEvent(SDL_Event *evn);

    void ClearKeyStates();

    bool GetKeyState(SDLKey key) const;
    bool GetButtonState(Uint8 button) const;
    bool GetActionKeyState(EGameAction::TYPE action) const;

    vec2 GetMousePos() const;
    
    EGameAction::TYPE GetActionForKey(SDLKey key) const;
    EGameAction::TYPE GetActionForButton(Uint8 button) const;
    sActionKey GetKeyForAction(EGameAction::TYPE action) const;

protected:

    sActionKey m_KeyForAction[EGameAction::_COUNT];
};