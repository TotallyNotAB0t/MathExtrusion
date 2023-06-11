#include "SettingManager.hpp"

namespace Ge
{
    double SettingManager::getFps()
    {

        return m_settingInfo.m_fps;
    }

    void SettingManager::setFps(double fps)
    {

        m_settingInfo.m_fps = fps;
    }

    const char *SettingManager::getName()
    {
        return m_settingInfo.m_name;
    }

    void SettingManager::setName(const char *name)
    {

        m_settingInfo.m_name = name;
    }
    double SettingManager::getWindowHeight()
    {

        return m_settingInfo.m_sizeHeightWindow;
    }

    void SettingManager::setWindowHeight(double height)
    {

        m_settingInfo.m_sizeHeightWindow = height;
    }

    double SettingManager::getWindowWidth()
    {

        return m_settingInfo.m_sizeWidthWindow;
    }
    void SettingManager::setWindowWidth(double Width)
    {

        m_settingInfo.m_sizeWidthWindow = Width;
    }
	glm::vec3 SettingManager::getGravity()
    {
        return m_settingInfo.m_gravity;
    }
    void SettingManager::setGravity(glm::vec3 gravity)
    {

        m_settingInfo.m_gravity = gravity;
    }
    void SettingManager::setVersion(Version v)
    {

        m_settingInfo.m_version = v;
    }

    Version SettingManager::getVersion()
    {
        return m_settingInfo.m_version;
    }

    void SettingManager::setClearColor(glm::vec4 color)
    {
        m_settingInfo.m_clearColor = color;
    }

	glm::vec4 SettingManager::getClearColor()
    {
        return m_settingInfo.m_clearColor;
    }

    void SettingManager::setGamma(float gamma)
    {
        m_settingInfo.m_gamma = gamma;
    }

    float SettingManager::getGamma()
    {
        return m_settingInfo.m_gamma;
    }

	void SettingManager::setIconPath(const char * path)
	{
		m_settingInfo.iconPath = path;
	}

	const char * SettingManager::getIconPath()
	{
		return m_settingInfo.iconPath;
	}
}