#ifndef _COLLISIONBODY_HPP
#define _COLLISIONBODY_HPP

#include "glm/glm.hpp"
#include "btBulletDynamicsCommon.h"
#include "CollisionShape.hpp"
#include "Debug.hpp"
#include "GObject.hpp"

namespace Ge
{
    class CollisionBody : public GObject
    {
    public:
        CollisionBody(btDynamicsWorld* world);
        CollisionBody(btDynamicsWorld* world, CollisionShape* shape);
        void Build(CollisionShape* shape);
        void setPosition(glm::vec3 pos) override;
        void setRotation(glm::quat rot) override;
        void setEulerAngles(glm::vec3 eul) override;
        void setScale(glm::vec3 scale) override;
        glm::vec3 getPosition() override;
        glm::quat getRotation() override;
        glm::vec3 getEulerAngles() override;
        glm::vec3 getScale() override;
        void setFriction(float friction);
        void setRestitution(float restitution);
        bool IsInitialized();
        void mapMemory() override;
        virtual ~CollisionBody();
    protected:
        friend class PhysicsEngine;

        inline btCollisionObject* GetBody(void)
        {
            return m_pBody;
        }

        btDynamicsWorld* m_pWorld = nullptr;
        btCollisionObject* m_pBody = nullptr;        
        CollisionShape* m_pShape = nullptr;        
    };



}

#endif