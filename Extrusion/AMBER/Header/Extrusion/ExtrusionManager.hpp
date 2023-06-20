#ifndef __EXTRUSION_MANAGER__
#define __EXTRUSION_MANAGER__

#include "ImguiBlock.hpp"
#include "Behaviour.hpp"
#include "GameEngine.hpp"
#include <string>
#include <vector>
#include "CourbeType.hpp"

struct Courbe
{
	Courbe()
	{
		pas = 50;
		height = 5.0f;
		scale = 1.0f;
		revolutionSegments = 36;
	}
	std::vector<glm::vec2> cloudPoint;
	std::vector<int> coefControl;
	std::vector<Model*> points;
	std::vector<PointLight*> light;
	std::vector<Model*> segments;
	float height;
	float scale;
	int pas;
	int revolutionSegments;
	CourbeType courbetype;
	bool rotation = false;
};

class ExtrusionManager : public ImguiBlock, public Behaviour
{
public:
	ExtrusionManager(Model* plane);
	void start();
	void fixedUpdate();
	void update();
	void stop();
	void onGUI();
	void preRender(VulkanMisc* vM);
	void render(VulkanMisc* vM);	
	void updateCourbe(int i);
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
	bool m_planeShowHide = false;
	bool m_planeActive = true;
	bool m_priority = false;
	bool m_cloudPoint = false;
	bool m_clearCloudPoint = false;
	bool m_erase = false;
	bool m_rotate = false;
	bool m_extrusion = false;
	bool m_extrusionRevolution = false;

	bool m_cloudButton = false;
	bool m_isMouseOverUI = false;	

	bool m_triangleDePascale = false;
	bool m_decastelJau = false;


	float m_size = 0.05f;
	int m_control_point = -1;
	int m_courbe_point = 0;
	bool m_pas = false;
	int m_listboxCurrentItem = 0;
	std::vector<const char*> cnames;
	std::vector<std::string> valueS;

	std::vector<Model*> m_points_clouds;
	std::vector<PointLight*> m_points_light_clouds;

	ShapeBuffer* m_sb = nullptr;
	Materials* m_pointMat = nullptr;
	Materials* m_segmentMat = nullptr;
	std::vector<Courbe> m_courbes;
	Model* m_plane;
};

#endif//!__EXTRUSION_MANAGER__