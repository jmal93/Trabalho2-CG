#include "disk.h"

#include <cmath>
#include <complex>
#include <glad/glad.h>
#include <vector>

DiskPtr Disk::Make(int nslice) {
  return DiskPtr(new Disk(nslice));
}

Disk::Disk(int nslice) {
  float radius = 1.0f;
  std::vector<float> coords;
  std::vector<float> texcoords;
  std::vector<float> normals;

  coords.push_back(0.0f);
  coords.push_back(0.0f);
  texcoords.push_back(0.5f);
  texcoords.push_back(0.5f);

  for (int i = 0; i <= nslice; i++) {
    float angle = 2.0f * M_PI * i / nslice;
    float x = radius * cos(angle);
    float y = radius * sin(angle);

    coords.push_back(x);
    coords.push_back(y);

    texcoords.push_back(0.5f + 0.5f * cos(angle));
    texcoords.push_back(0.5f + 0.5f * sin(angle));

    normals.push_back(0.0f);
    normals.push_back(0.0f);
    normals.push_back(1.0f);
  }

  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  GLuint ids[3];
  glGenBuffers(3, ids);

  glBindBuffer(GL_ARRAY_BUFFER, ids[0]);
  glBufferData(GL_ARRAY_BUFFER, coords.size() * sizeof(float), coords.data(),
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, ids[1]);
  glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(),
               GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, ids[2]);
  glBufferData(GL_ARRAY_BUFFER, texcoords.size() * sizeof(float), texcoords.data(),
               GL_STATIC_DRAW);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(2);

  m_nslice = coords.size() / 2;
}

Disk::~Disk() {}

void Disk::Draw(StatePtr) {
  glBindVertexArray(m_vao);
  glDrawArrays(GL_TRIANGLE_FAN, 0, m_nslice);
}
