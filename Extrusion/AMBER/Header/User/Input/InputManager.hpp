#ifndef __ENGINE_INPUT_MANAGER__
#define __ENGINE_INPUT_MANAGER__

#include "Window.hpp"
#include "GLFW/glfw3.h"
#include <map>
#include "Debug.hpp"
#include "glm/glm.hpp"

namespace Ge
{
	class InputManager
	{
	private:
		friend class GameEngine;
		bool initialize(VulkanMisc * vM);
		void updateAxis();
		void release();
	public:
		const char * getGamepadName(int jid);
		bool getGamepadState(int jid,int key);
		float getGamepadAxis(int jid, int indice);
		bool getKey(int key);
		bool getKeyUp(int key);
		bool getKeyDown(int key);
		bool getMouse(int key);
		double axisMouseX();
		double axisMouseY();
		double getMousePosX();
		double getMousePosY();
	private:
		std::map<int, bool> mapPressedInput;
		std::map<int, bool> mapReleasedInput;
		glm::tvec2<double> m_lastMousePos;
		glm::tvec2<double> m_axisMouse;
		glm::tvec2<double> m_Mpos;
		GLFWwindow * m_window;
	};
}

#endif //__ENGINE_INPUT_MANAGER__