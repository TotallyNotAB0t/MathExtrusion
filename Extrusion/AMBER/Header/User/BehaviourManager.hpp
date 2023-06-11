#ifndef __ENGINE_BEHAVIOUR_MANAGER__
#define __ENGINE_BEHAVIOUR_MANAGER__

#include "Behaviour.hpp"
#include <vector>
#include <algorithm>

namespace Ge
{
	class BehaviourManager
	{
	public:
		void addBehaviour(Behaviour * b);
		void removeBehaviour(Behaviour * b,bool ignoreStop = false);	
		void update();
		void fixedUpdate();		
	private:
		std::vector<Behaviour *>  m_behaviours;
		std::vector<Behaviour *>  m_startBehaviours;
		std::vector<Behaviour *>  m_stopBehaviours;
	};
}

#endif//__ENGINE_BEHAVIOUR_MANAGER__