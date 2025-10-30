#include "orbit.h"

Orbit::Orbit(TransformPtr trf, float speed) : m_trf(trf), m_speed(speed) {}

OrbitPtr Orbit::Make(TransformPtr trf, float speed) {
  return OrbitPtr(new Orbit(trf, speed));
}

void Orbit::Update(float dt) {
  m_trf->Rotate(m_speed * (-dt) / 30.0f * 180.0f, 0, 1, 0);
}
