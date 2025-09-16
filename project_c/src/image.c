#include "image.h"
#include "cglm/affine-pre.h"
#include "cglm/affine.h"
#include "cglm/mat4.h"
#include "shader.h"
#include <stb_image.h>

float vertices[] = {
    // positions          // colors           // texture coords
    0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
    0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
};
unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

bool image_create(Image *image, Shader *shader, const char *path)
{
    image->image_shader = shader;

    glGenVertexArrays(1, &image->VAO);
    glGenBuffers(1, &image->VBO);
    glGenBuffers(1, &image->EBO);

    glBindVertexArray(image->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, image->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, image->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glGenTextures(1, &image->texture);
    glBindTexture(GL_TEXTURE_2D, image->texture); 
     // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps

    stbi_set_flip_vertically_on_load(image->filp_vertical);
    unsigned char *data = stbi_load(path, &image->width, &image->height,
            &image->nrChannels, 0);
    if (data)
    {
        image->format = GL_RGB;
        if (image->nrChannels == 1)
            image->format = GL_RED;
        else if (image->nrChannels == 3)
            image->format = GL_RGB;
        else if (image->nrChannels == 4)
            image->format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, image->format, image->width, image->height,
                0, image->format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        printf("format: %i", image->nrChannels);
    }
    else
    {
        printf("failed to load texture\n");
        return false;
    }
    stbi_image_free(data);
    image->filp_vertical = true;
    return true;
}

void image_draw(Image *image, vec2 loc, float scale)
{
    mat4 model;
    glm_mat4_identity(model);

    glm_translate(model, (vec3){loc[0], loc[1], 0.0f});
    glm_scale(model, (vec3){image->width * scale, image->height * scale, 1.0f});

    shader_mat4(image->image_shader, "model", model);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, image->texture);

    glBindVertexArray(image->VAO); 
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void image_cleanup(Image *image)
{
    glDeleteVertexArrays(1, &image->VAO);
    glDeleteBuffers(1, &image->VBO);
    glDeleteBuffers(1, &image->EBO);
    glDeleteTextures(1, &image->texture);
}
