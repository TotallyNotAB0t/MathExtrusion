#include "FlyCamera.hpp"
#include "Time.hpp"

namespace Ge
{
	FlyCamera::FlyCamera(VulkanMisc * vM, InputManager * input) : Camera(vM)
	{
		m_im = input;
		m_angleY = 0.0f;
		m_angleX = 0.0f;
		m_moveSpeed = 0.0f;
		m_priority = 1.0f;
	}

	void FlyCamera::updateCamera()
	{
		if (m_im->getKey(340))
		{
			m_addMoveSpeed += Time::GetDeltaTime() * m_fastSpeedMult;
		}
		else
		{
			m_addMoveSpeed = 0.0f;
		}
		m_moveSpeed = Time::GetDeltaTime() * m_maxSpeed;
		m_moveSpeed += m_addMoveSpeed;

		if (m_im->getMouse(1))
		{
			m_angleY -= m_im->axisMouseY() * m_maxSpeedRotate;
			m_angleX -= m_im->axisMouseX() * m_maxSpeedRotate;
			setEulerAngles(glm::vec3(m_angleY, m_angleX, 0.0f));
		}

		if (m_im->getKey(87))
		{
			setPosition(getPosition() + transformDirectionAxeZ() * m_moveSpeed);
		}
		else if (m_im->getKey(83))
		{
			setPosition(getPosition() + transformDirectionAxeZ() * -m_moveSpeed);
		}

		if (m_im->getKey(68))
		{
			setPosition(getPosition() + transformDirectionAxeX() * -m_moveSpeed);
		}
		else if (m_im->getKey(65))
		{
			setPosition(getPosition() + transformDirectionAxeX() * m_moveSpeed);
		}

		if (m_im->getKey(341))
		{
			setPosition(getPosition() + glm::vec3(0, -1, 0) * m_moveSpeed);
		}
		else if (m_im->getKey(32))
		{
			setPosition(getPosition() + glm::vec3(0, 1, 0) * m_moveSpeed);
		}		
	}
}