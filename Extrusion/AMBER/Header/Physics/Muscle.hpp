#ifndef __MUSCLE__
#define __MUSCLE__

#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "btBulletDynamicsCommon.h"
#include "RigidBody.hpp"
#include "Debug.hpp"

namespace Ge
{
    class Muscle
    {
    public:
        Muscle(btDynamicsWorld* world, btRigidBody* bodyA, btRigidBody* bodyB, glm::vec3 pA, glm::vec3 pB, glm::vec3 sA, glm::vec3 sB, float degres, bool adaptePos = true);
        void setContraction(glm::vec3 m_value, float dt);
        void setWing(bool state);
        void reset();
        ~Muscle();
    private:
        btDynamicsWorld* m_world;
        btRigidBody* m_bodyA;
        btRigidBody* m_bodyB;
        btTransform m_localA;
        btTransform m_localB;
        btConeTwistConstraint* m_joint;
        glm::vec3 m_contractions;
        float m_degres;
        float m_ndeg;
        bool m_wing = false;
    };
}

#endif __MUSCLE__