#include "glutils/glutils.h"
#include <stdio.h>
#include <stdarg.h>

GLboolean gluCreateProgram(GLProgram* program) {
    (*program).id = glCreateProgram();
    return (*program).id != 0;
}

void gluProgramCreateVsh(GLProgram* program, const GLchar* src) {
    program->vshId = gluProgramCreateShader(*program, src, VERTEX_SHADER);
}
void gluProgramCreateFsh(GLProgram* program, const GLchar* src) {
    program->fshId = gluProgramCreateShader(*program, src, FRAGMENT_SHADER);
}
void gluProgramCreateGsh(GLProgram* program, const GLchar* src) {
    program->gshId = gluProgramCreateShader(*program, src, GEOMETRY_SHADER);
}
GLuint gluProgramCreateShader(GLProgram program, const GLchar* src, ShaderType type) {
    GLenum gltype = 0;
    switch (type) {
    case FRAGMENT_SHADER:
        gltype = GL_FRAGMENT_SHADER;
        break;
    case VERTEX_SHADER:
        gltype = GL_VERTEX_SHADER;
        break;
    case GEOMETRY_SHADER:
        gltype = GL_GEOMETRY_SHADER;
        break;
    }
    GLuint shader = glCreateShader(gltype);
    if (shader == 0) {
        const char* nm;
        switch (type) {
        case FRAGMENT_SHADER:
            nm = "fragmentShader";
            break;
        case VERTEX_SHADER:
            nm = "vertexShader";
            break;
        case GEOMETRY_SHADER:
            nm = "geometryShader";
            break;
        }
        printf("An error occurred creating the %s object.", nm);
        return 0;
    }
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    GLboolean status = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        GLchar infoLog[1024] = { 0 };
        glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
        printf("Error compiling shader src: %s", infoLog);
        return shader;
    }
    glAttachShader(program.id, shader);
    return shader;
}

GLboolean gluProgramLink(GLProgram program) {
    int id = program.id;
    glLinkProgram(id);
    GLboolean status;
    glGetProgramiv(id, GL_LINK_STATUS, &status);
    if (!status) {
        GLchar infoLog[1024] = { 0 };
        glGetProgramInfoLog(id, sizeof(infoLog), NULL, infoLog);
        printf("Error linking GL program: %s", infoLog);
        return GL_FALSE;
    }
#define detach(type) \
if (program.##type##shId != 0) glDetachShader(id, program.##type##shId)
    detach(v);
    detach(f);
    detach(g);
#undef detach
    glValidateProgram(id);
    glGetProgramiv(id, GL_VALIDATE_STATUS, &status);
    if (!status) {
        GLchar infoLog[1024] = { 0 };
        glGetProgramInfoLog(id, sizeof(infoLog), NULL, infoLog);
        printf(infoLog);
    }
    return GL_TRUE;
}
void gluProgramBind(GLProgram program) {
    glUseProgram(program.id);
}
void gluProgramUnbind() {
    glUseProgram(0);
}

GLint gluProgramGetUniform(GLProgram program, const GLchar* name) {
    return glGetUniformLocation(program.id, name);
}
void gluProgramSetUniform1i(GLProgram program, const GLchar* name, GLint value) {
    glUniform1i(gluProgramGetUniform(program, name), value);
}
void gluProgramSetUniformMat4f(GLProgram program, const GLchar* name, const GLfloat* matrix4f) {
    glUniformMatrix4fv(gluProgramGetUniform(program, name), 1, GL_FALSE, matrix4f);
}

GLint gluProgramGetAttrib(GLProgram program, const GLchar* name) {
    return glGetAttribLocation(program.id, name);
}
void gluProgramEnableVertexAttribArray(GLProgram program, const GLchar* name) {
    glEnableVertexAttribArray(gluProgramGetAttrib(program, name));
}
#define gpvaa(p,c,o) \
va_list list; \
va_start(list,c); \
for (int i=0; i<c; ++i) { \
    gl##o##ableVertexAttribArray(gluProgramGetAttrib(p,va_arg(list,const GLchar*))); \
} \
va_end(list)
void gluProgramEnableVertexAttribArrays(GLProgram program, int count, ...) {
    gpvaa(program, count, En);
}
void gluProgramDisableVertexAttribArrays(GLProgram program, int count, ...) {
    gpvaa(program, count, Dis);
}
#undef gpvaa
void gluProgramVertexAttribPointer(GLProgram program,
    const GLchar* name,
    GLint size,
    GLenum type,
    GLboolean normalized,
    GLsizei stride,
    const void* pointer) {
    glVertexAttribPointer(gluProgramGetAttrib(program, name),
        size,
        type,
        normalized,
        stride,
        pointer);
}

#define close(id,t) if (program.##id## != 0) glDelete##t##(program.##id##)
void gluProgramClose(GLProgram program) {
    close(id, Program);
    close(vshId, Shader);
    close(fshId, Shader);
    close(gshId, Shader);
}
#undef close
