#ifndef __ENGINE_SETTING_MANAGER__
#define __ENGINE_SETTING_MANAGER__

#include "Initializer.hpp"
#include "Debug.hpp"
#include "SettingInfo.hpp"

namespace Ge
{
    class SettingManager
    {
    public:
        double getFps();
        void setFps(double fps);
        const char *getName();
        void setName(const char *name);
        double getWindowHeight();
        void setWindowHeight(double height);
        double getWindowWidth();
        void setWindowWidth(double Width);
        glm::vec3 getGravity();
        void setGravity(glm::vec3 gravity);
        void setVersion(Version v);
        Version getVersion();
        void setClearColor(glm::vec4 color);
		glm::vec4 getClearColor();
        void setGamma(float gamma);
        float getGamma();
		void setIconPath(const char * path);
		const char * getIconPath();

    private:
        SettingInfo m_settingInfo;
    };
}

#endif //__ENGINE_SETTING_MANAGER__