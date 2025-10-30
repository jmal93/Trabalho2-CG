#include "skybox.h"
#include "grid.h"
#include "error.h"

#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>  // <- necessário para translate
#include <glm/gtc/matrix_inverse.hpp>    // (opcional) para inverse

SkyBoxPtr SkyBox::Make ()
{
  return SkyBoxPtr(new SkyBox());
}

SkyBox::SkyBox ()
{
  // 36 vértices (triângulos) de um cubo unitário centrado na origem
  static const float verts[] = {
    // back face
    -1.f, -1.f, -1.f,  1.f,  1.f, -1.f,  1.f, -1.f, -1.f,
    -1.f, -1.f, -1.f, -1.f,  1.f, -1.f,  1.f,  1.f, -1.f,
    // left face
    -1.f, -1.f,  1.f, -1.f, -1.f, -1.f, -1.f,  1.f, -1.f,
    -1.f, -1.f,  1.f, -1.f,  1.f, -1.f, -1.f,  1.f,  1.f,
    // right face
     1.f, -1.f, -1.f,  1.f,  1.f,  1.f,  1.f, -1.f,  1.f,
     1.f, -1.f, -1.f,  1.f,  1.f, -1.f,  1.f,  1.f,  1.f,
    // front face
    -1.f, -1.f,  1.f,  1.f, -1.f,  1.f,  1.f,  1.f,  1.f,
    -1.f, -1.f,  1.f,  1.f,  1.f,  1.f, -1.f,  1.f,  1.f,
    // top face
    -1.f,  1.f, -1.f, -1.f,  1.f,  1.f,  1.f,  1.f,  1.f,
    -1.f,  1.f, -1.f,  1.f,  1.f,  1.f,  1.f,  1.f, -1.f,
    // bottom face
    -1.f, -1.f, -1.f,  1.f, -1.f, -1.f,  1.f, -1.f,  1.f,
    -1.f, -1.f, -1.f,  1.f, -1.f,  1.f, -1.f, -1.f,  1.f,
  };

  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  glGenBuffers(1, &m_coord_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, m_coord_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

  // LOC::COORD deve ser o location do atributo de posição (vec3)
  glEnableVertexAttribArray(LOC::COORD);
  glVertexAttribPointer(LOC::COORD, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

SkyBox::~SkyBox () 
{
}

void SkyBox::Draw (StatePtr st)
{
  CameraPtr camera = st -> GetCamera();
  glm::vec4 origin = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  glm::vec3 peye = glm::vec3(glm::inverse(camera->GetViewMatrix())*origin);
  glm::mat4 m = glm::translate(glm::mat4(1.0f),peye);
  st->PushMatrix();
  st->MultMatrix(m);
  st->LoadMatrices();
  glDepthMask(GL_FALSE);
  glBindVertexArray(m_vao);
  glBindBuffer( GL_ARRAY_BUFFER, m_coord_buffer);
  glEnableVertexAttribArray(LOC::COORD);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindBuffer(GL_ARRAY_BUFFER, m_coord_buffer);
  glDisableVertexAttribArray(LOC::COORD);
  glDepthMask(GL_TRUE);
  st->PopMatrix();
}
