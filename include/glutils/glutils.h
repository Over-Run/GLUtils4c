#pragma once
#include <gl/glad.h>

#ifdef __cplusplus
extern "C" {
#endif
    typedef struct GLProgram_ {
        GLuint id;
        GLuint vshId;
        GLuint fshId;
        GLuint gshId;
    } GLProgram;

    typedef enum ShaderType_ {
        FRAGMENT_SHADER,
        VERTEX_SHADER,
        GEOMETRY_SHADER
    } ShaderType;

    GLboolean gluCreateProgram(GLProgram* program);

    void gluProgramCreateVsh(GLProgram* program, const GLchar* src);
    void gluProgramCreateFsh(GLProgram* program, const GLchar* src);
    void gluProgramCreateGsh(GLProgram* program, const GLchar* src);
    GLuint gluProgramCreateShader(GLProgram program, const GLchar* src, ShaderType type);

    GLboolean gluProgramLink(GLProgram program);
    void gluProgramBind(GLProgram program);
    void gluProgramUnbind();

    GLint gluProgramGetUniform(GLProgram program, const GLchar* name);
    void gluProgramSetUniform1i(GLProgram program, const GLchar* name, GLint value);
    void gluProgramSetUniformMat4f(GLProgram program, const GLchar* name, const GLfloat* matrix4f);

    GLint gluProgramGetAttrib(GLProgram program, const GLchar* name);
    void gluProgramEnableVertexAttribArray(GLProgram program, const GLchar* name);
    void gluProgramEnableVertexAttribArrays(GLProgram program, int count, ...);
    void gluProgramDisableVertexAttribArrays(GLProgram program, int count, ...);
    void gluProgramVertexAttribPointer(GLProgram program,
        const GLchar* name,
        GLint size,
        GLenum type,
        GLboolean normalized,
        GLsizei stride,
        const void* pointer);

    void gluProgramClose(GLProgram program);
#ifdef __cplusplus
}
#endif
