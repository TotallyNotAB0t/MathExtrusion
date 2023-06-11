#include "PhysicsEngine.hpp"

namespace Ge
{
	bool PhysicsEngine::Initialize(glm::vec3 const& gravity)
	{
		auto beginPhysicsInit = std::chrono::steady_clock::now();
		Debug::Info("Initialisation du moteur physique");

		m_pCollisionCongifuration = new btDefaultCollisionConfiguration();
		m_pCollisionDispatcher = new btCollisionDispatcher(m_pCollisionCongifuration);
		m_pBroadphaseInterface = new btDbvtBroadphase();
		m_pSequentialConstraintSolver = new btSequentialImpulseConstraintSolver();

		m_pDynamicWorld = new btDiscreteDynamicsWorld(
			m_pCollisionDispatcher,
			m_pBroadphaseInterface,
			m_pSequentialConstraintSolver,
			m_pCollisionCongifuration);

		m_pDynamicWorld->setGravity(btVector3(gravity.x, gravity.y, gravity.z));

		auto endPhysicsInit = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endPhysicsInit - beginPhysicsInit);

		return true;
	}

	void PhysicsEngine::Update(float dt,bool engine)
	{
		if (engine && !m_disableEngineUpdate || !engine)
		{
			m_pDynamicWorld->stepSimulation(dt, 1);
		}
	}

	void PhysicsEngine::Shutdown()
	{
		for (int i = 0; i < m_muscle.size(); i++)
		{
			delete m_muscle[i];
		}
		for (int i = 0; i < m_rigidbody.size(); i++)
		{
			m_pDynamicWorld->removeRigidBody(m_rigidbody[i]->m_pBody);			
			delete m_rigidbody[i];
		}
		for (int i = 0; i < m_collisionbody.size(); i++)
		{
			m_pDynamicWorld->removeCollisionObject(m_collisionbody[i]->m_pBody);
			delete m_collisionbody[i];
		}
		delete m_pCollisionCongifuration;
		delete m_pCollisionDispatcher;
		delete m_pBroadphaseInterface;
		delete m_pSequentialConstraintSolver;
		delete m_pDynamicWorld;
	}

	CollisionBody* PhysicsEngine::AllocateCollision(CollisionShape* shape)
	{
		return new CollisionBody(m_pDynamicWorld, shape);
	}

	void PhysicsEngine::AddCollision(CollisionBody* body)
	{
		if (body != nullptr && body->IsInitialized())
		{
			m_pDynamicWorld->addCollisionObject(body->GetBody());
			m_collisionbody.push_back(body);
		}
		else
		{
			Debug::Error("PhysicsEngine::AddCollision(): Le corps donné n'est pas initialisé.");
		}
	}

	void PhysicsEngine::setDisableEngineUpdate(bool state)
	{
		m_disableEngineUpdate = state;
	}

	bool PhysicsEngine::getDisableEngineUpdate()
	{
		return m_disableEngineUpdate;
	}

	bool PhysicsEngine::raycast(const glm::vec3& start, const glm::vec3& end, glm::vec3& hitPoint)
	{
		btVector3 btstart = btVector3(start.x, start.y, start.z);
		btVector3 btend = btVector3(end.x, end.y, end.z);
		btCollisionWorld::ClosestRayResultCallback rayCallback(btstart, btend);
		rayCallback.m_collisionFilterMask = 1;
		m_pDynamicWorld->rayTest(btstart, btend, rayCallback);

		if (rayCallback.hasHit()) 
		{
			btVector3 hit;
			hit = rayCallback.m_hitPointWorld;
			hitPoint = glm::vec3(hit.x(), hit.y(), hit.z());
			return true;
		}
		else 
		{
			return false;
		}
	}

	void PhysicsEngine::ReleaseCollision(CollisionBody*& pBody)
	{
		std::vector<CollisionBody*>::iterator position = std::find(m_collisionbody.begin(), m_collisionbody.end(), pBody);
		if (position != m_collisionbody.end())
		{
			m_collisionbody.erase(position);
		}
		m_pDynamicWorld->removeCollisionObject(pBody->m_pBody);
		delete pBody;
	}

	RigidBody * PhysicsEngine::AllocateRigidbody(CollisionShape* shape, bool hasInertia)
	{
		return new RigidBody(m_pDynamicWorld, shape, hasInertia);
	}

	Muscle* PhysicsEngine::AllocateMuscle(RigidBody* rb1, RigidBody* rb2,float degres,float scale, bool adaptePos)
	{
		Muscle* m = new Muscle(m_pDynamicWorld, rb1->GetBody(), rb2->GetBody(), rb1->getPosition(), rb2->getPosition(), rb1->getScale()* scale,rb2->getScale()* scale,degres, adaptePos);
		m_muscle.push_back(m);
		return m;
	}

	void PhysicsEngine::ReleaseMuscle(Muscle* pBody)
	{
		std::vector<Muscle*>::iterator position = std::find(m_muscle.begin(), m_muscle.end(), pBody);
		if (position != m_muscle.end())
		{
			m_muscle.erase(position);
		}
		delete pBody;
	}

	void PhysicsEngine::AddRigidbody(RigidBody* body,int group,int mask)
	{
		if (body != nullptr && body->IsInitialized())
		{			
			m_pDynamicWorld->addRigidBody(body->GetBody(),group,mask);
			m_rigidbody.push_back(body);
		}
		else
		{
			Debug::Error("PhysicsEngine::AddRigidbody(): Le corps donné n'est pas initialisé.");
		}
	}

	void PhysicsEngine::ReleaseRigidbody(RigidBody * pBody)
	{
		std::vector<RigidBody*>::iterator position = std::find(m_rigidbody.begin(), m_rigidbody.end(), pBody);
		if (position != m_rigidbody.end())
		{
			m_rigidbody.erase(position);
		}
		m_pDynamicWorld->removeRigidBody(pBody->m_pBody);
		delete pBody;
	}

}

