#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#include "GenericManager.h"

/** Class reposnsible for handling/mapping game events */
class InputManager : public GenericManager
{
public:
		InputManager();
		virtual bool init();
		virtual bool update();
		virtual bool exit();
};

#endif // INPUTMANAGER_H
