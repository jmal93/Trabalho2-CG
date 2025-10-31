#include <memory>
class CameraTarget;
using CameraTargetPtr = std::shared_ptr<CameraTarget>;

#ifndef CAMERATARGET_H
#define CAMERATARGET_H

#include "engine.h"
#include "camera3d.h"
#include "node.h"
#include "arcball.h"

enum CameraTargetType { Sun, Mercury, Earth, Moon };

class CameraTarget : public Engine {
    Camera3DPtr m_camera;
    NodePtr m_sun, m_mercury, m_earth, m_moon;
    CameraTargetType m_prev_target;
    
public:
    CameraTargetType m_current_target;
    
protected:
    CameraTarget(Camera3DPtr cam, NodePtr s, NodePtr m, NodePtr e, NodePtr mo);

public:
    static CameraTargetPtr Make(Camera3DPtr cam, NodePtr s, NodePtr m, NodePtr e, NodePtr mo);
    
    virtual void Update(float dt) override;
};

#endif