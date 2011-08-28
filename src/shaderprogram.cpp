#include <iostream>
// TODO: replace glew with pure ogl??
#include <SDL/SDL.h>
#include <GL/glew.h>
#include "shaderprogram.h"

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
		cout << infoLog << endl;
}

char *file2string(const char *path)
{
	FILE *fd;
	long len,
		 r;
	char *str;

	if (!(fd = fopen(path, "r")))
	{
		cerr << "Can't open file '" << path << "' for reading." << endl;
		return NULL;
	}

	fseek(fd, 0, SEEK_END);
	len = ftell(fd);

	cout << "File '" << path << "' is " << len << " long" << endl;

	fseek(fd, 0, SEEK_SET);

	if (!(str = new char[len]))
	{
		cerr << "Can't malloc space for '" << path << "'." << endl;
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

	cout << "Loading shader programs..." << endl;
	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, (const GLchar**)&vsource, NULL);

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, (const GLchar**)&fsource, NULL);
	is_loaded = true;
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
