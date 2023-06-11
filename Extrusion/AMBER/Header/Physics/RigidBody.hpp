#ifndef _RIGIDBODY_HPP
#define _RIGIDBODY_HPP

#include "glm/glm.hpp"
#include "btBulletDynamicsCommon.h"
#include "CollisionShape.hpp"
#include "Debug.hpp"
#include "GObject.hpp"
#include "Muscle.hpp"

namespace Ge
{
class RigidBody : public GObject
{
public:
    RigidBody(btDynamicsWorld* world);
    RigidBody(btDynamicsWorld* world, CollisionShape* shape, bool hasInertia = true);
    void Translate(glm::vec3 const& translate);
    void setPosition(glm::vec3 pos) override;
    void setRotation(glm::quat rot) override;
    void setEulerAngles(glm::vec3 eul) override;
    glm::vec3 getPosition() override;
    glm::quat getRotation() override;
    glm::vec3 getEulerAngles() override;
    glm::vec3 getGravity();
    void SetMass(float mass);
    void setGravity(glm::vec3 const& gravity);
    void SetRestitution(float coef);
    void SetLinearVelocity(glm::vec3 const& velocity);    
    void BuildPhysics(CollisionShape* shape,bool hasInertia = true);
    bool IsInitialized();
    glm::vec3 GetLinearVelocity();
    void ApplyImpulse(glm::vec3 impulse, glm::vec3 real_pos);
    void forceActivationState(int newState);
    void SetSleepingThreshold(float linear, float angular);
    void onGUI() override;
    void mapMemory() override;
    virtual ~RigidBody();
    inline btRigidBody* GetBody(void)
    {
        return m_pBody;
    }
protected:
friend class PhysicsEngine;

    btDynamicsWorld         * m_pWorld          = nullptr;
    btRigidBody             * m_pBody           = nullptr;
    btDefaultMotionState    * m_pMotionState    = nullptr;
    CollisionShape          * m_pShape          = nullptr;
    float m_gravity[3];
    float m_velocity[3];
    int style_idx = -1;
    bool m_activeCollision = false;
};



}

#endif