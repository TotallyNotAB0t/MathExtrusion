#include "Window.hpp"
#include "stb-cmake/stb_image.h"

namespace Ge
{

	bool Window::initialize(uint32_t Width, uint32_t Height, const char * name, const char * iconPath, VulkanMisc * vM)
	{
		glfwInit();//init la fenetre

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);/*ne cree pas de context OpenGl*/

		this->m_window = glfwCreateWindow(Width, Height, name, nullptr, nullptr);/*3: Moniteur 4: Specifique a OpenGl*/
		glfwSetWindowUserPointer(this->m_window, this);//;on fournie la class a la fenetre
		glfwSetFramebufferSizeCallback(this->m_window, this->framebufferResizeCallback);//utiliser une fonction statique car GLFW ne sait pas correctement appeler une fonction membre
		vM->str_VulkanDeviceMisc->str_window = this->m_window;
		Debug::INITSUCCESS("Window");		
		if (iconPath && iconPath[0])
		{									
			GLFWimage images;
			images.pixels = stbi_load(iconPath, &images.width, &images.height, 0, 4);
			glfwSetWindowIcon(this->m_window, 1, &images);
			stbi_image_free(images.pixels);
		}		
		return true;
	}

	void Window::release()
	{
		glfwDestroyWindow(this->m_window);/*DestroyWindow*/
		glfwTerminate();
		Debug::RELEASESUCCESS("Window");
	}

	void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		/*callback de redimentionement pour glfw avec la mise a jour du parametre framebufferResized
		 pour pouvoir indiquer a la swap chain quil faut ce redimentionner*/
		auto win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		win->framebufferResized = true;
		Debug::Info("Redimension de la fenetre : %d , %d", width,height);
	}
}