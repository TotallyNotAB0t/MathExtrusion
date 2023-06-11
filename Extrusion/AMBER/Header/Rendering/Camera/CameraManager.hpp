#ifndef __ENGINE_CAMERA_MANAGER__
#define __ENGINE_CAMERA_MANAGER__

#include "VulkanMisc.hpp"
#include "Debug.hpp"
#include "Manager.hpp"
#include "UniformBufferCamera.hpp"
#include "FlyCamera.hpp"
#include <map>

namespace Ge
{
    class CameraManager : public Manager
    {
    public:
        bool initialize(VulkanMisc *vM, InputManager *input);
        void release();
        Camera *createCamera(std::string name = "Camera");
        void releaseCamera(Camera *camera);      
        Camera *getCurrentCamera();
		void updateFlyCam();
		void initDescriptor(VulkanMisc * vM);
		void updateDescriptor();
		static void updatePriorityCamera();
    private:		
		static CameraManager * Instance;
        static std::vector<Camera *> m_cameras;
        static Camera *currentCamera;
        VulkanMisc * vulkanM;        
        FlyCamera * m_flyCamera;
    };
}

#endif //__ENGINE_CAMERA_MANAGER__