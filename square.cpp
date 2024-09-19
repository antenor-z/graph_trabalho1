#include "square.h"

#include <iostream>

#ifdef _WIN32
#include <glad/glad.h>
#elif __linux__
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#else
#include <OpenGL/gl3.h>
#endif

SquarePtr Square::Make ()
{
  return SquarePtr(new Square());
}

Square::Square ()
{
  float coord[] = {-1.0f,-1.0f,1.0f,-1.0f,1.0f,1.0f,1.0f,1.0f,-1.0f,1.0f,-1.0f,-1.0f};
  std::vector<float> texcoord = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f};

  // Create VAO
  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  GLuint id;
  glGenBuffers(1, &id);
  glBindBuffer(GL_ARRAY_BUFFER, id);
  glBufferData(GL_ARRAY_BUFFER, sizeof(coord), coord, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);  
  glEnableVertexAttribArray(0);

  GLuint texCoordBuffer;
  glGenBuffers(1, &texCoordBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
  glBufferData(GL_ARRAY_BUFFER, texcoord.size() * sizeof(float), texcoord.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);  // texture coordinates
  glEnableVertexAttribArray(1);
}

Square::~Square ()
{
    glDeleteVertexArrays(1, &m_vao);
}

void Square::Draw (StatePtr )
{
  glBindVertexArray(m_vao);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}
