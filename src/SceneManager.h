#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include <GL/glew.h>

#include "screen.h"
#include "shaderprogram.h"
#include "utils.h"
#include "logger.h"

/** Class responsible for visualisation */
class SceneManager
{
public:
	SceneManager()
	{
		m_screen = NULL;
	}

	~SceneManager()
	{
		clear();
	}

	void clear()
	{
		delete m_screen;
		m_screen = 0;
		delete m_shader;
		m_shader = NULL;
	}

	bool init()
	{
		clear();

		//TODO: maybe OGL/OAL/SDL initialization here as well?
		m_screen = new Screen(WIDTH, HEIGHT, "X-Com 42");
		if (!m_screen)
			return false;

		//Initialize GLEW here (window must be created)
		{
			int ret;
			if ( (ret = glewInit())!= GLEW_OK)
			{
				LogError("Error initializing glew: %s", glewGetErrorString(ret));
				return false;
			}
		}

		m_shader = new ShaderProgram("shaders/city.vert", "shaders/city.frag");

		if (!m_shader->compile())
		{
			LogError("Error compiling shader program.");
			return false;
		}

		if (!m_shader->link())
		{
			LogError("Error linking shader program.");
			return false;
		}

		return true;
	}

private:
	Screen * m_screen;
	ShaderProgram *m_shader;
};

#endif // SCENEMANAGER_H
