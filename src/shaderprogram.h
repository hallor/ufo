#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <GL/gl.h>

class ShaderProgram
{
public:
    ShaderProgram(const char * vertex, const char * fragment);
    ~ShaderProgram();
    bool compile();
    bool link();
    void use();
    void unuse();
    GLuint get_sp() const { return sp; }
private:
    char * vsource, *fsource;
    GLuint vs, fs, sp;
    bool is_loaded;
    bool is_built;
    bool is_attached;
};

#endif // SHADERPROGRAM_H
