#ifndef __EXTRUSION_MANAGER__
#define __EXTRUSION_MANAGER__

#include "ImguiBlock.hpp"
#include "Behaviour.hpp"
#include "GameEngine.hpp"
#include <string>
#include <vector>

struct Courbe
{
	Courbe()
	{
		pas = 50;
	}
	std::vector<glm::vec2> cloudPoint;
	std::vector<int> coefControl;
	std::vector<Model*> points;
	std::vector<Model*> segments;
	int pas;
};

class ExtrusionManager : public ImguiBlock, public Behaviour
{
public:
	void start();
	void fixedUpdate();
	void update();
	void stop();
	void onGUI();
	void preRender(VulkanMisc* vM);
	void render(VulkanMisc* vM);	
	Model* createSegment(glm::vec2 p1, glm::vec2 p2);
	std::vector<glm::vec2> trianglePascal(Courbe courbe);
	std::vector<glm::vec2> DeCasteljau(Courbe courbe);
	glm::vec3 directionToRotation(glm::vec3 direction);
private:
	ptrClass m_pc;
	Camera* m_cam2D;
	Camera* m_cam3D;

	bool m_open = false;
	bool m_cameraChange = false;
	bool m_priority = false;
	bool m_cloudPoint = false;
	bool m_clearCloudPoint = false;

	bool m_cloudButton = false;
	bool m_isMouseOverUI = false;	

	bool m_triangleDePascale = false;
	bool m_decastelJau = false;

	std::vector<Model*> m_points_clouds;

	ShapeBuffer* m_sb = nullptr;
	Materials* m_pointMat = nullptr;
	Materials* m_segmentMat = nullptr;
	std::vector<Courbe> m_courbes;
};

#endif//!__EXTRUSION_MANAGER__