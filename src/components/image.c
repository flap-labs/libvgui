#include <stdio.h>
#include <stdlib.h>

#include "include/image.h"

#ifdef __linux__
#include "../linux/include/window.h"
#elif _WIN32
#include "../win32/include/window.h"
#endif

#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <cglm/cglm.h>
#include <stb/stb_image.h>

unsigned int shaderProgram;
unsigned int vao, vbo;
int imgRendererInitialized = 0;
mat4 projection;

// Reads a GLSL source file and returns it as a `char *`
char *img_readShaderSource(const char *filename)
{
  FILE *file = fopen(filename, "rb");
  
  if (file == NULL)
  {
    printf("Error: Invalid filename \"%s\". Does it exist?\n", filename);
    exit(1);
  }

  fseek(file, 0, SEEK_END);
  long length = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *str = malloc(length + 1);
  fread(str, length, 1, file);
  str[length] = '\0';

  fclose(file);
  return str;
}

// Performs any other tasks the image renderer needs to do
void image_finalize()
{
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
}

// Draws each image onto the screen
void image_render()
{
  glUseProgram(shaderProgram);

  int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
  glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection[0]);
}

// Creates a new image
Image image_new(int x, int y, int width, int height, const char *filepath)
{
  // Create the image
  Image image;

  image.x = x;
  image.y = y;
  image.width = width;
  image.height = height;

  // Vertex data
  float vertices[] =
  {
    0.5f, -0.5f, 0.0f,    1.0f, 0.0f,
    0.5f, 0.5f, 0.0f,     1.0f, 1.0f,
    -0.5f, 0.5f, 0.0f,    0.0f, 1.0f,
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f
  };

  // Index data
  unsigned int indices[] =
  {
    0, 1, 3,
    1, 2, 3
  };

  // VAO
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  
  if (imgRendererInitialized == 0)
  {
    imgRendererInitialized = 1;

    // Read shader sources
    char *vertexShaderSource = img_readShaderSource("src/shaders/image/shader.vert");
    char *fragmentShaderSource = img_readShaderSource("src/shaders/image/shader.frag");

    // Shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);

    // Delete shaders we don't need anymore
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Calculate projection matrix
  #ifdef __linux__
    glm_ortho(0.0f, (float)linux_getWindowWidth(), (float)linux_getWindowHeight(), 0.0f, -1.0f, 1.0f, projection);
  #elif _WIN32
    glm_ortho(0.0f, (float)win32_getWidth(), (float)win32_getHeight(), -(720.0f - (float)win32_getHeight()), -1.0f, 1.0f, projection);
  #endif
  }

  // Buffers
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  unsigned int ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Load the image
  glGenTextures(1, &image.texture);
  glBindTexture(GL_TEXTURE_2D, image.texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  int newX, newY, channels;
  unsigned char *data = stbi_load(filepath, &newX, &newY, &channels, 0);

  if (!data)
  {
    printf("Error: Filepath \"%s\" does not exist.\n");
    exit(1);
  }

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, newX, newY, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  return image;
}

// Draws the specified image
void image_draw(Image *image)
{
  // Apply texture
  glBindTexture(GL_TEXTURE_2D, image->texture);

  // Compute model matrix
  glm_mat4_identity(image->model);

  vec3 translate = { (float)(image->x + (image->width / 2)), (float)(image->y + (image->height / 2)), 0.0f };
  vec3 scale = { (float)image->width, (float)image->height, 0.0f };

  glm_translate(image->model, translate);
  glm_scale(image->model, scale);

  // Apply model matrix
  int modelLoc = glGetUniformLocation(shaderProgram, "model");
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, image->model[0]);

  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}
