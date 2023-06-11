#include "CollisionShape.hpp"

namespace Ge
{

CollisionShape::CollisionShape(float mass) : m_mass(mass)
{
}

void CollisionShape::SetMass(float mass)
{
    m_mass = mass;
}

glm::vec3 CollisionShape::GetInertia(void) const
{
    btVector3 inertia;
    m_pShape->calculateLocalInertia(m_mass, inertia);

    return glm::vec3( inertia.x(), inertia.y(), inertia.z() );
}

float CollisionShape::GetMass(void) const
{
    return m_mass;
}

btCollisionShape* CollisionShape::GetShape(void) const
{
    return m_pShape;
}

CollisionShape::~CollisionShape()
{
    delete m_pShape;
}

BoxShape::BoxShape(glm::vec3 halfExtents, float mass) : CollisionShape(mass), m_halfExtents(halfExtents)
{
    m_pShape = new btBoxShape(btVector3(halfExtents.x, halfExtents.y, halfExtents.z));    
}

SphereShape::SphereShape(float radius, float mass) : CollisionShape(mass), m_radius(radius)
{
    m_pShape = new btSphereShape(radius);
}

CapsuleShape::CapsuleShape(float radius, float height, float mass) : CollisionShape(mass), m_radius(radius), m_height(height)
{
    m_pShape = new btCapsuleShape(radius, height);
}

VertexShape::VertexShape(float mass) :
    CollisionShape(mass)
{
   m_triangleMesh   = new btTriangleMesh();
}

VertexShape::VertexShape(float mass, std::vector<Vertex> const& m_triangle) : CollisionShape(mass)
{
    m_triangleMesh = new btTriangleMesh();
    SetTriangles(m_triangle);
}

void VertexShape::SetTriangles(std::vector<Vertex> const& verteces)
{
    size_t size = verteces.size();

    for (size_t i = 0ul; i < size; i += 3)
    {
        m_triangleMesh->addTriangle( 
            btVector3(verteces[i].pos.x, verteces[i].pos.y, verteces[i].pos.z),
            btVector3(verteces[i + 1].pos.x, verteces[i+ 1].pos.y, verteces[i+ 1].pos.z),
            btVector3(verteces[i + 2].pos.x, verteces[i+ 2].pos.y, verteces[i+ 2].pos.z)
            );
    }
    
    if (m_pShape != nullptr)
    {
        delete m_pShape;
    }

    m_pShape = new btBvhTriangleMeshShape(m_triangleMesh, true, true);
}

VertexShape::~VertexShape()
{
    delete m_triangleMesh;
}

}