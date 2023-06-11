#include "Time.hpp"

namespace Ge
{
	Time* Time::s_pInstance = nullptr;

	Time::Time()
	{
		Time::s_pInstance = this;
		m_time = 0.0f;
	}

	void Time::startTime()
	{
		m_startTime = glfwGetTime();
		m_currentTime = m_startTime;
		m_currentTimeF = m_startTime;
	}

	void Time::fixedUpdateTime()
	{
		m_lastTimeF = m_currentTimeF;
		m_currentTimeF = glfwGetTime();
		m_time = static_cast<float>(m_currentTimeF - m_startTime);
		m_fixedDeltaTime = static_cast<float>(m_currentTimeF - m_lastTimeF);
	}

	void Time::updateTime()
	{
		m_lastTime = m_currentTime;
		m_currentTime = glfwGetTime();
		m_deltaTime = static_cast<float>(m_currentTime - m_lastTime);
	}

	void Time::release()
	{
		Time::s_pInstance = nullptr;
	}

	float Time::getDeltaTime()
	{
		return m_deltaTime;
	}

	float Time::getFixedDeltaTime()
	{
		return m_fixedDeltaTime;
	}

	float Time::getTime()
	{
		return m_time;
	}

	float Time::GetFixedDeltaTime()
	{
		return Time::s_pInstance->m_fixedDeltaTime;
	}

	float Time::GetDeltaTime()
	{
		return Time::s_pInstance->m_deltaTime;
	}

	float Time::GetTime()
	{
		return Time::s_pInstance->m_time;
	}
}