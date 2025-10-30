#include <memory>
class Orbit;
using OrbitPtr = std::shared_ptr<Orbit>;

#ifndef ORBIT_H
#define ORBIT_H

#include "engine.h"
#include "transform.h"

class Orbit : public Engine {
  TransformPtr m_trf;
  float m_speed;

protected:
  Orbit(TransformPtr trf, float speed);

public:
  static OrbitPtr Make(TransformPtr trf, float speed);
  virtual void Update(float dt);
};

#endif
