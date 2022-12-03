#include <stdio.h>
#include <stdlib.h>

#include "include/frame.h"
#include <glad/glad.h>

unsigned int shaderProgram, vao;
int frameRendererInitialized = 0;
mat4 projection;

// Reads a GLSL source file and returns it as a `char *`
char *readShaderSource(const char *filename)
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

// Initialzies the rendering of frames
void frame_init()
{
  // Check to make sure we haven't already initialized rendering
  if (frameRendererInitialized == 1)
  {
    printf("Error: Frame renderer initialized twice.\n");
    exit(1);
  }

  frameRendererInitialized = 1;

  // Read shader sources
  char *vertexShaderSource = readShaderSource("src/shaders/frame/shader.vert");
  char *fragmentShaderSource = readShaderSource("src/shaders/frame/shader.frag");

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

  // Vertex array object
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glm_ortho(0.0f, 1280.0f, 720.0f, 0.0f, -1.0f, 1.0f, projection);
}

// Draws each frame onto the screen
void frame_render()
{
  glUseProgram(shaderProgram);
  glBindVertexArray(vao);

  // Apply projection matrix
  int projectionLocation = glGetUniformLocation(shaderProgram, "projection");
  glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, projection[0]);
}

// Does any extra tasks the frame's renderer needs to do
void frame_finalize()
{
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
}

// Creates a new frame object
Frame frame_new(int x, int y, int width, int height)
{
  if (frameRendererInitialized == 0)
  {
    printf("Error: Failed to render a frame. Create a window first.\n");
    exit(1);
  } 

  Frame frame;

  frame.x = x;
  frame.y = y;
  frame.width = width;
  frame.height = height;

  frame.r = 255;
  frame.g = 255;
  frame.b = 255;

  // Vertex data
  float vertices[] =
  {
    0.5f, -0.5f, 0.0f,
    0.5f, 0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f
  };

  // Index data
  unsigned int indices[] =
  {
    0, 1, 3,
    1, 2, 3
  };

  // OpenGL buffers
  unsigned int vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  unsigned int ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glm_mat4_identity(frame.model);
  return frame;
}

// Changes the color of the provided frame component
void frame_changeColor(Frame *frame, int r, int g, int b)
{
  frame->r = r;
  frame->g = g;
  frame->b = b;
}

// Draws the frame onto the renderer
void frame_draw(Frame *frame)
{
  // Calculate model matrix
  glm_mat4_identity(frame->model);

  vec3 translation = { (float)(frame->x + (frame->width / 2)), (float)(frame->y + (frame->height / 2)), 0.0f };
  glm_translate(frame->model, translation);

  vec3 scale = { (float)frame->width, (float)frame->height, 0.0f };
  glm_scale(frame->model, scale);

  // Calculate color vector
  vec3 color = { (float)frame->r, (float)frame->g, (float)frame->b };

  // Apply model matrix
  int modelLocation = glGetUniformLocation(shaderProgram, "model");
  glUniformMatrix4fv(modelLocation, 1, GL_FALSE, frame->model[0]);

  // Apply color vector
  int colorLocation = glGetUniformLocation(shaderProgram, "color");
  glUniform3fv(colorLocation, 1, color);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
