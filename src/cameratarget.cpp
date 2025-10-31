#include "cameratarget.h"

CameraTarget::CameraTarget(Camera3DPtr cam, NodePtr s, NodePtr m, NodePtr e, NodePtr mo) 
: m_camera(cam), m_sun(s), m_mercury(m), m_earth(e), m_moon(mo), 
  m_prev_target(Sun), m_current_target(Sun) // Inicializa ambos como Sun (inicialmente o programa roda pelo sun)
{
}

CameraTargetPtr CameraTarget::Make(Camera3DPtr cam, NodePtr s, NodePtr m, NodePtr e, NodePtr mo) {
  return CameraTargetPtr(new CameraTarget(cam, s, m, e, mo));
}

void CameraTarget::Update(float dt) {
    if (m_current_target != m_prev_target) {
        NodePtr target_node = nullptr;
        switch (m_current_target) {
            case Sun:     
                target_node = m_sun; 
                break;
            case Mercury: 
                target_node = m_mercury; 
                break;
            case Earth:   
                target_node = m_earth; 
                break;
            case Moon:    
                target_node = m_moon; 
                break;
        }
        
        m_camera->SetReference(target_node); 
        
        if (m_camera->GetArcball()) {
            m_camera->GetArcball()->Reset(); 
        }
        
        m_prev_target = m_current_target;
    }
}