#include "Muscle.hpp"

namespace Ge
{
    Muscle::Muscle(btDynamicsWorld* world, btRigidBody* bodyA, btRigidBody* bodyB, glm::vec3 pA, glm::vec3 pB, glm::vec3 sA, glm::vec3 sB, float degres,bool adaptePos) : m_bodyA(bodyA), m_bodyB(bodyB)
    {
        m_degres = degres;
        glm::vec3 oA;
        glm::vec3 oB;
        if (adaptePos)
        {
            glm::vec3 diff = pB - pA;
            diff = glm::normalize(diff);
            float srA = 0.0f;
            float srB = 0.0f;
            srA = srA > sA.x ? srA : sA.x;
            srA = srA > sA.y ? srA : sA.y;
            srA = srA > sA.z ? srA : sA.z;
            srA = srA / 2.0f;
            srB = srB > sB.x ? srB : sB.x;
            srB = srB > sB.y ? srB : sB.y;
            srB = srB > sB.z ? srB : sB.z;
            srB = srB / 2.0f;
            oA = diff * srA;
            oB = diff * -srB;
        }
        else
        {
            oA = pA;
            oB = pB;
        }
        m_localA.setIdentity();
        m_localB.setIdentity();
        m_localA.getBasis().setEulerZYX(0.0f, 0.0f, 0.0f);
        m_localB.getBasis().setEulerZYX(0.0f, 0.0f, 0.0f);
        m_localA.setOrigin(btVector3(oA.x, oA.y, oA.z));
        m_localB.setOrigin(btVector3(oB.x, oB.y, oB.z));

        m_joint = new btConeTwistConstraint(*(bodyA), *(bodyB), m_localA, m_localB);
        m_joint->setLimit(glm::radians(1.0f), glm::radians(1.0f), glm::radians(0.0f));
        world->addConstraint(m_joint, true);
        m_world = world;
        m_contractions = glm::vec3(0.5f,0.5f,0.5f);
        m_ndeg = glm::radians(m_degres / 2.0f);
    }

    Muscle::~Muscle()
    {
        m_world->removeConstraint(m_joint);
        delete m_joint;
    }

    float lerp(float a, float b, float t) 
    {
        return (1.0f - t) * a + t * b;
    }

    void Muscle::setWing(bool state)
    {
        m_wing = state;
    }

    void Muscle::reset()
    {
        m_contractions = glm::vec3(0.5f, 0.5f, 0.5f);
        m_localA.getBasis().setEulerZYX(0, 0, 0);
        m_localB.getBasis().setEulerZYX(glm::radians(m_degres * m_contractions.x) - m_ndeg, glm::radians(m_degres * m_contractions.y) - m_ndeg, glm::radians(m_degres * m_contractions.z) - m_ndeg);
        m_joint->setFrames(m_localA, m_localB);
    }

    void Muscle::setContraction(glm::vec3 m_value,float dt)
    {
        m_localA.getBasis().setEulerZYX(0, 0, 0);
        m_contractions.x = lerp(m_contractions.x, m_value.x, dt * 5.0f);
        m_contractions.y = lerp(m_contractions.y, m_value.y, dt * 5.0f);
        m_contractions.z = lerp(m_contractions.z, m_value.z, dt * 5.0f);        
        m_localB.getBasis().setEulerZYX(glm::radians(m_degres * m_contractions.x)- m_ndeg, glm::radians(m_degres * m_contractions.y)- m_ndeg, glm::radians(m_degres * m_contractions.z)- m_ndeg);
        m_joint->setFrames(m_localA, m_localB);        
        if (m_wing)
        {
            btVector3 vtv = m_bodyA->getLinearVelocity();
            m_bodyA->setLinearVelocity(vtv+btVector3((m_contractions.x-m_value.x)*dt* 100.0f, abs(m_contractions.y - m_value.y) * dt * 90.0f, (m_contractions.z - m_value.z) * dt * 100.0f));
        }
    }
}