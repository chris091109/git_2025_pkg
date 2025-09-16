#include "shader.h"
#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <string.h>

char *readfile(const char *path)
{
    FILE *file = fopen(path, "rb");
    if (!file) {
        printf("Failed to open file: %s\n", path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(length + 1);
    if (!buffer) {
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, length, file);
    buffer[length] = '\0';

    fclose(file);
    return buffer;
}

void compile_shader(Shader *shader, const char *type)
{
    int success;
    char infoLog[1024];
    
    if (strcmp(type, "PROGRAM") != 0)
    {
        glGetShaderiv(shader->shader_id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader->shader_id, 1024, NULL, infoLog);
            printf("Failed to compile %s shader:\n%s\n", type, infoLog);
        }
    }
    else
    {
        glGetProgramiv(shader->shader_id, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader->shader_id, 1024, NULL, infoLog);
            printf("Failed to link shader program:\n%s\n", infoLog);
        }
    }
}

bool shader_init(Shader *shader, const char *vert, const char *frag)
{
    // Read vertex shader file
    char *vertexCode = readfile(vert);
    if (!vertexCode) {
        printf("Failed to read vertex shader: %s\n", vert);
        return false;
    }

    // Read fragment shader file
    char *fragmentCode = readfile(frag);
    if (!fragmentCode) {
        printf("Failed to read fragment shader: %s\n", frag);
        free(vertexCode);
        return false;
    }

    const char* vShaderCode = vertexCode;
    const char* fShaderCode = fragmentCode;

    // Compile shaders
    unsigned int vertex, fragment;
    
    // Vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    
    // Check vertex shader compilation
    int success;
    char infoLog[512];
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        printf("Vertex shader compilation failed:\n%s\n", infoLog);
        free(vertexCode);
        free(fragmentCode);
        return false;
    }

    // Fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    
    // Check fragment shader compilation
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        printf("Fragment shader compilation failed:\n%s\n", infoLog);
        glDeleteShader(vertex);
        free(vertexCode);
        free(fragmentCode);
        return false;
    }

    // Shader program
    shader->shader_id = glCreateProgram();
    glAttachShader(shader->shader_id, vertex);
    glAttachShader(shader->shader_id, fragment);
    glLinkProgram(shader->shader_id);
    
    // Check program linking
    glGetProgramiv(shader->shader_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader->shader_id, 512, NULL, infoLog);
        printf("Shader program linking failed:\n%s\n", infoLog);
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        free(vertexCode);
        free(fragmentCode);
        return false;
    }

    // Cleanup
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    free(vertexCode);
    free(fragmentCode);

    return true;
}

void shader_use(Shader* shader)
{
    glUseProgram(shader->shader_id);
}

void shader_cleanup(Shader *shader)
{
    if (shader->shader_id) {
        glDeleteProgram(shader->shader_id);
        shader->shader_id = 0;
    }
}

void shader_mat4(Shader *shader, const char *name, mat4 value)
{
    int location = glGetUniformLocation(shader->shader_id, name);
    if (location != -1) {
        glUniformMatrix4fv(location, 1, GL_FALSE, (float*)value);
    }
}

void shader_int(Shader *shader, const char *name, int value)
{
    int location = glGetUniformLocation(shader->shader_id, name);
    if (location != -1) {
        glUniform1i(location, value);
    }
}

void shader_vec3(Shader *shader, const char *name, vec3 value)
{
        int location = glGetUniformLocation(shader->shader_id, name);
    if (location != -1) {
        glUniform3fv(location, 1, value);
    }
}
