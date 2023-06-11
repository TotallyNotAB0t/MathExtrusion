#include "CollisionBody.hpp"

namespace Ge
{

    CollisionBody::CollisionBody(btDynamicsWorld* world) : m_pWorld(world), GObject()
    {
        setName("CollisionBody");
    }

    CollisionBody::CollisionBody(btDynamicsWorld* world, CollisionShape* shape) : m_pWorld(world), GObject()
    {
        setName("CollisionBody");        
        Build(shape);
    }

    void CollisionBody::setPosition(glm::vec3 pos)
    {
        GObject::setPosition(pos);
        btTransform initialTransform;
        btQuaternion rotation = m_pBody->getWorldTransform().getRotation();        
        
        initialTransform.setOrigin(btVector3(pos.x, pos.y, pos.z));
        initialTransform.setRotation(rotation);

        m_pBody->setWorldTransform(initialTransform);        
    }
    
    void CollisionBody::setScale(glm::vec3 scale)
    {
        m_pShape->GetShape()->setLocalScaling(btVector3(scale.x, scale.y, scale.z));
    }

    glm::vec3 CollisionBody::getScale()
    {
        btVector3 vec = m_pShape->GetShape()->getLocalScaling();
        return glm::vec3(vec.x(), vec.y(), vec.z());
    }

    void CollisionBody::setRotation(glm::quat rot)
    {
        m_transform.rotation = rot;
        m_eulerAngles = GObject::getEulerAngles();
        btTransform initialTransform;
        btVector3   position = m_pBody->getWorldTransform().getOrigin();

        initialTransform.setOrigin(position);
        initialTransform.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));

        m_pBody->setWorldTransform(initialTransform);        
    }

    void CollisionBody::setEulerAngles(glm::vec3 eul)
    {
        GObject::setEulerAngles(eul);
        glm::quat rot = GObject::getRotation();
        btTransform initialTransform;
        btVector3   position = m_pBody->getWorldTransform().getOrigin();

        initialTransform.setOrigin(position);
        initialTransform.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));

        m_pBody->setWorldTransform(initialTransform);        
    }

    glm::vec3 CollisionBody::getPosition()
    {
        btVector3 pos = m_pBody->getWorldTransform().getOrigin();
        return glm::vec3(pos.getX(), pos.getY(), pos.getZ());
    }
    glm::quat CollisionBody::getRotation()
    {
        btQuaternion rot = m_pBody->getWorldTransform().getRotation();
        return glm::quat(rot.w(), rot.x(), rot.y(), rot.z());
    }

    glm::vec3 CollisionBody::getEulerAngles()
    {
        GObject::setRotation(getRotation());
        return GObject::getEulerAngles();
    }

    void CollisionBody::Build(CollisionShape* shape)
    {
        if (IsInitialized())
        {
            return;
        }                
        m_pShape = shape;
        m_pBody = new btCollisionObject();
        m_pBody->setCollisionShape(m_pShape->GetShape());
    }

    void CollisionBody::setFriction(float friction)
    {
        m_pBody->setFriction(friction);
    }

    void CollisionBody::setRestitution(float restitution)
    {
        m_pBody->setRestitution(restitution);
    }

    bool CollisionBody::IsInitialized()
    {
        return m_pBody != nullptr;
    }

    void CollisionBody::mapMemory() {}

    CollisionBody::~CollisionBody(void)
    {        
        delete m_pBody;
    }

}