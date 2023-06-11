#ifndef _COLLISION_SHAPE_HPP
#define _COLLISION_SHAPE_HPP


#include <vector>
#include "glm/glm.hpp"
#include "btBulletDynamicsCommon.h"
#include "VertexVulkan.hpp"

namespace Ge
{

class CollisionShape
{
    public:
        CollisionShape(float mass);
        void                SetMass(float mass);
        glm::vec3           GetInertia(void) const;
        float               GetMass(void) const;
        btCollisionShape*   GetShape(void) const;
        virtual ~CollisionShape();

    protected:
        btCollisionShape*   m_pShape            = nullptr;
        btScalar            m_inertia;
        float               m_mass              = 0;
};

class BoxShape : public CollisionShape
{
    public:
    BoxShape(glm::vec3 halfExtends, float mass);

    protected:
    glm::vec3 m_halfExtents;
};

class SphereShape : public CollisionShape
{
public:
    SphereShape(float radius, float mass);

protected:
    float m_radius;
};

class CapsuleShape : public CollisionShape
{
public:
    CapsuleShape(float radius, float height, float mass);

protected:
    float m_radius;
    float m_height;
};

class VertexShape : public CollisionShape
{
public:
    VertexShape(float mass);
    VertexShape(float mass, std::vector<Vertex> const& m_triangle);

    void SetTriangles(std::vector<Vertex> const& m_triangle);

    ~VertexShape() override;

protected:
    btTriangleMesh         * m_triangleMesh;
};

}

#endif