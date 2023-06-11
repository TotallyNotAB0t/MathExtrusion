#include "RigidBody.hpp"

namespace Ge
{

RigidBody::RigidBody(btDynamicsWorld* world) : m_pWorld(world), GObject() 
{
    setName("RigidBody");
}

RigidBody::RigidBody(btDynamicsWorld* world, CollisionShape* shape, bool hasInertia) : m_pWorld(world), GObject()
{
    setName("RigidBody");
    BuildPhysics(shape, hasInertia);   
    //m_pBody->setCollisionFlags(m_pBody->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);
}

void RigidBody::Translate(glm::vec3 const& translate)
{
    m_pBody->translate( btVector3(translate.x, translate.y, translate.z));
}

void RigidBody::setPosition(glm::vec3 pos) 
{
    GObject::setPosition(pos);
    btTransform initialTransform;
    btQuaternion rotation = m_pBody->getWorldTransform().getRotation();

    initialTransform.setOrigin(btVector3(pos.x, pos.y, pos.z));
    initialTransform.setRotation(rotation);

    m_pBody->setWorldTransform(initialTransform);
    m_pMotionState->setWorldTransform(initialTransform);
}

void RigidBody::setRotation(glm::quat rot) 
{
    GObject::setRotation(rot);
    btTransform initialTransform;
    btVector3   position = m_pBody->getWorldTransform().getOrigin();

    initialTransform.setOrigin(position);
    initialTransform.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));

    m_pBody->setWorldTransform(initialTransform);
    m_pMotionState->setWorldTransform(initialTransform);
}

void RigidBody::setEulerAngles(glm::vec3 eul) 
{
    GObject::setEulerAngles(eul);
    glm::quat rot = GObject::getRotation();
    btTransform initialTransform;
    btVector3   position = m_pBody->getWorldTransform().getOrigin();
    
    initialTransform.setOrigin(position);
    initialTransform.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));

    m_pBody->setWorldTransform(initialTransform);
    m_pMotionState->setWorldTransform(initialTransform);
}

glm::vec3 RigidBody::getPosition() 
{
    btVector3 pos = m_pBody->getWorldTransform().getOrigin();
    return glm::vec3(pos.getX(), pos.getY(), pos.getZ());
}
glm::quat RigidBody::getRotation() 
{
    btQuaternion rot = m_pBody->getWorldTransform().getRotation();
    return glm::quat(rot.w(), rot.x(), rot.y(),rot.z());
}
glm::vec3 RigidBody::getEulerAngles() 
{
    GObject::setRotation(getRotation());
    return GObject::getEulerAngles();
}

glm::vec3 RigidBody::getGravity()
{
    btVector3 gravity = m_pBody->getGravity();
    return glm::vec3(gravity.x(),gravity.y(),gravity.z());
}

void RigidBody::SetMass(float mass)
{
    m_pShape->SetMass(mass);
    glm::vec3 inertia = m_pShape->GetInertia();    
    m_pBody->setMassProps(mass, btVector3(inertia.x, inertia.y, inertia.z) );
}

void RigidBody::setGravity(glm::vec3 const& gravity)
{
    m_pBody->setGravity( btVector3(gravity.x, gravity.y, gravity.z) );
}

void RigidBody::SetRestitution(float coef)
{
    m_pBody->setRestitution(coef);
}

void RigidBody::SetLinearVelocity(glm::vec3 const& velocity)
{
    m_pBody->setLinearVelocity(btVector3(velocity.x, velocity.y, velocity.z));
}

void RigidBody::ApplyImpulse(glm::vec3 impulse, glm::vec3 real_pos)
{
    m_pBody->applyImpulse(btVector3(impulse.x, impulse.y, impulse.z), btVector3(real_pos.x, real_pos.y, real_pos.z));
}

glm::vec3 RigidBody::GetLinearVelocity()
{
    btVector3 vel = m_pBody->getLinearVelocity();
    return glm::vec3(vel.x(), vel.y(), vel.z());
}

void RigidBody::forceActivationState(int newState)
{
    style_idx = newState - 1;
    m_pBody->forceActivationState(newState);
}

void RigidBody::BuildPhysics(CollisionShape* shape, bool hasInertia)
{
    if (IsInitialized())
    {
        return;
    }
    m_pShape = shape;
    m_pMotionState = new btDefaultMotionState();
    
    glm::vec3 inertia;
    if (hasInertia)
    {
        inertia = m_pShape->GetInertia();
    }

    m_pBody = new btRigidBody(m_pShape->GetMass(), m_pMotionState, m_pShape->GetShape(), btVector3(inertia.x, inertia.y, inertia.z));    
}

bool RigidBody::IsInitialized()
{
    return m_pBody != nullptr;
}

void RigidBody::SetSleepingThreshold(float linear, float angular)
{
    m_pBody->setSleepingThresholds(btScalar(linear), btScalar(angular));
}

void RigidBody::mapMemory(){}


void RigidBody::onGUI()
{
    GObject::setPosition(getPosition());    
    GObject::onGUI();

    glm::vec3 vel = GetLinearVelocity();
    m_velocity[0] = vel.x;
    m_velocity[1] = vel.y;
    m_velocity[2] = vel.z;
    glm::vec3 gravity = getGravity();
    m_gravity[0] = gravity.x;
    m_gravity[1] = gravity.y;
    m_gravity[2] = gravity.z;

    ImGui::TextColored(ImVec4(0.2f, 1, 0.2f, 1), "RigidBody\n");
    if (ImGui::DragFloat3("Gravity", m_gravity, 0.2f))
    {
        setGravity(glm::vec3(m_gravity[0], m_gravity[1], m_gravity[2]));
    }
    if (ImGui::DragFloat3("Velocity", m_velocity, 0.2f))
    {
        vel.x = m_velocity[0];
        vel.y = m_velocity[1];
        vel.z = m_velocity[2];
        SetLinearVelocity(vel);
    }
    if (ImGui::Combo("ActivationState", &style_idx, "ACTIVE_TAG\0ISLAND_SLEEPING\0WANTS_DEACTIVATION\0DISABLE_DEACTIVATION\0DISABLE_SIMULATION\0"))
    {
        switch (style_idx)
        {
        case 0: m_pBody->forceActivationState(1); break;
        case 1: m_pBody->forceActivationState(2); break;
        case 2: m_pBody->forceActivationState(3); break;
        case 3: m_pBody->forceActivationState(4); break;
        case 4: m_pBody->forceActivationState(5); break;
        }
    }
}

RigidBody::~RigidBody(void)
{
    delete m_pMotionState;
    delete m_pBody;
}

}