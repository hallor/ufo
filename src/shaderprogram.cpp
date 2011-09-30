#include <iostream>
// TODO: replace glew with pure ogl??
#include <SDL/SDL.h>
#include <GL/glew.h>
#include "shaderprogram.h"
#include "logger.h"

using namespace std;

void printLog(GLuint obj)
{
  int infologLength = 0;
  char infoLog[1024];

	if (glIsShader(obj))
		glGetShaderInfoLog(obj, 1024, &infologLength, infoLog);
	else
		glGetProgramInfoLog(obj, 1024, &infologLength, infoLog);

	if (infologLength > 0)
		LogError("%s", infoLog);
}

char *file2string(const char *path)
{
	FILE *fd;
	long len,
		 r;
	char *str;

	if (!(fd = fopen(path, "r")))
	{
		LogError("Can't open file %s for reading.", path);
		return NULL;
	}

	fseek(fd, 0, SEEK_END);
	len = ftell(fd);

	fseek(fd, 0, SEEK_SET);

	if (!(str = new char[len]))
	{
		LogError("Can't alloc space for %s.", path);
		return NULL;
	}

	r = fread(str, sizeof(char), len, fd);

	str[r - 1] = '\0'; /* Shader sources have to term with null */

	fclose(fd);

	return str;
}

ShaderProgram::ShaderProgram(const char * vertex, const char * fragment)
{
	if (vertex)
		vsource = file2string(vertex);
	if (fragment)
		fsource = file2string(fragment);

	LogInfo("Loading shader programs...");
	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, (const GLchar**)&vsource, NULL);

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, (const GLchar**)&fsource, NULL);
	is_loaded = true;
	is_built = false;
	is_attached = false;
}

ShaderProgram::~ShaderProgram()
{
	delete [] fsource;
	delete [] vsource;
}

bool ShaderProgram::compile()
{
	if (!is_built)
	{
		glCompileShader(vs);
		printLog(vs);

		glCompileShader(fs);
		printLog(fs);
		is_built = glGetError()==0;
	}
	return is_built;
}

bool ShaderProgram::link()
{
	if (!is_attached)
	{
		sp = glCreateProgram();
		glAttachShader(sp, vs);
		glAttachShader(sp, fs);
		glLinkProgram(sp);
		is_attached = glGetError()==0;
	}
	return is_attached;
}

void ShaderProgram::use()
{
	glUseProgram(sp);
}

void ShaderProgram::unuse()
{
	glUseProgram(0);
}
