#include "ExtrusionManager.hpp"


std::vector<glm::vec2> ExtrusionManager::DeCasteljau(Courbe courbe)
{
	std::vector<glm::vec2> cloudPointCoef;
	for (int i = 0; i < courbe.cloudPoint.size(); i++)
	{
		for (int j = 0; j < courbe.coefControl[i]; j++)
		{
			cloudPointCoef.push_back(courbe.cloudPoint[i]);
		}
	}
	int n = cloudPointCoef.size() - 1;  // Ordre de la courbe
	std::vector<glm::vec2> curvePoints;

	// Fonction d'interpolation linéaire entre deux points
	auto interpolate = [](glm::vec2 p1, glm::vec2 p2, float t)
	{
		float x = (1 - t) * p1.x + t * p2.x;
		float y = (1 - t) * p1.y + t * p2.y;
		return glm::vec2(x, y);
	};

	for (int t = 0; t <= courbe.pas; ++t)
	{
		float u = static_cast<float>(t) / courbe.pas;

		std::vector<glm::vec2> points(cloudPointCoef);  // Copie du nuage de points

		// Appliquer l'algorithme de De Casteljau
		for (int k = 1; k <= n; ++k)
		{
			for (int i = 0; i <= n - k; ++i)
			{
				points[i] = interpolate(points[i], points[i + 1], u);
			}
		}

		curvePoints.push_back(points[0]);
	}

	return curvePoints;
}

std::vector<glm::vec2> ExtrusionManager::trianglePascal(Courbe courbe)
{
	std::vector<glm::vec2> curvePoints;
	std::vector<glm::vec2> cloudPointCoef;
	for (int i = 0; i < courbe.cloudPoint.size(); i++)
	{
		for (int j = 0; j < courbe.coefControl[i]; j++)
		{
			cloudPointCoef.push_back(courbe.cloudPoint[i]);
		}
	}
	int n = cloudPointCoef.size() - 1;
	double step = 1.0 / courbe.pas;

	// Calculer les coefficients binomiaux à l'aide du triangle de Pascal
	std::vector<std::vector<int>> pascalTriangle(n + 1);
	for (int i = 0; i <= n; i++)
	{
		pascalTriangle[i].resize(i + 1);
		pascalTriangle[i][0] = 1;
		pascalTriangle[i][i] = 1;

		for (int j = 1; j < i; j++)
		{
			pascalTriangle[i][j] = pascalTriangle[i - 1][j - 1] + pascalTriangle[i - 1][j];
		}
	}

	// Générer les points de la courbe de Bézier
	for (double t = 0.0; t <= 1.0; t += step)
	{
		double x = 0.0;
		double y = 0.0;

		for (int i = 0; i <= n; i++)
		{
			double blend = pascalTriangle[n][i] * std::pow(t, i) * std::pow(1 - t, n - i);
			x += blend * cloudPointCoef[i].x;
			y += blend * cloudPointCoef[i].y;
		}

		curvePoints.push_back(glm::vec2(x, y));
	}

	return curvePoints;
}

glm::vec3 ExtrusionManager::directionToRotation(glm::vec3 direction)
{	
	direction = glm::normalize(direction);
	glm::quat quaternion = glm::rotation(glm::vec3(1.0f, 0.0f, 0.0f), direction);
	return glm::degrees(glm::eulerAngles(quaternion));
}

Model* ExtrusionManager::createSegment(glm::vec2 p1, glm::vec2 p2)
{
	glm::vec2 pos = (p1 + p2) / 2.0f;
	float scale = glm::distance(p1,p2)/2.0f;
	Model* m = m_pc.modelManager->createModel(m_sb);
	m->setMaterial(m_segmentMat);
	m->setPosition(glm::vec3(pos.x, 0.0f, pos.y));
	m->setScale(glm::vec3(scale, 0.1f, 0.1f));
	glm::vec2 direction = p2-p1;
	m->setEulerAngles(directionToRotation(glm::vec3(direction.x,0.0f, direction.y)));
	return m;
}

void ExtrusionManager::start()
{
	m_pc = GameEngine::getPtrClass();
	m_cam3D = m_pc.cameraManager->getCurrentCamera();
	m_cam2D = m_pc.cameraManager->createCamera();
	m_cam2D->setOrtho(true);
	m_cam2D->setPosition(glm::vec3(0, 10, 0));
	m_cam2D->setEulerAngles(glm::vec3(-90.0f, 0.0f, 0.0f));
    m_cam2D->setOrthoSize(15.0f);

    m_sb = m_pc.modelManager->allocateBuffer("../Model/cube.obj");

    m_pointMat = m_pc.materialManager->createMaterial();
    m_pointMat->setColor(glm::vec3(0.0f, 0.0f, 1.0f));
    m_pointMat->setMetallic(0.7f);
    m_pointMat->setRoughness(0.15f);

    m_segmentMat = m_pc.materialManager->createMaterial();      
    m_segmentMat->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
    m_segmentMat->setMetallic(0.7f);
    m_segmentMat->setRoughness(0.15f);
}

void ExtrusionManager::fixedUpdate()
{

}

void ExtrusionManager::update()
{
    if (m_cameraChange)
    {
        m_cameraChange = false;
        m_priority = !m_priority;
        m_cam3D->setPriority(m_priority ? 0 : 1);
        m_cam2D->setPriority(m_priority ? 1 : 0);
        m_pc.cameraManager->updatePriorityCamera();
    }
	if (m_clearCloudPoint)
	{
		m_clearCloudPoint = false;
		for (int i = 0; i < m_points_clouds.size(); i++)
		{			
			m_pc.modelManager->destroyModel(m_points_clouds[i]);
		}
		m_points_clouds.clear();
	}
    if (m_cloudPoint && !m_priority && !m_isMouseOverUI)
    {        
        if (m_pc.inputManager->getMouse(0))
        {
            if (m_cloudButton)
            {
                Model * m = m_pc.modelManager->createModel(m_sb);
                m->setMaterial(m_pointMat);
                m->setScale(glm::vec3(0.1f));				
                glm::vec2 mp = glm::vec2(m_pc.inputManager->getMousePosX(), m_pc.inputManager->getMousePosY());
                glm::vec2 ss = glm::vec2(m_pc.settingManager->getWindowWidth(), m_pc.settingManager->getWindowHeight());
                glm::vec2 pos = m_cam2D->ScreenToSpace(mp, ss);
                m->setPosition(glm::vec3(pos.x, 0.0f, pos.y));
				m_points_clouds.push_back(m);
                m_cloudButton = false;
            }
        }
        else
        {
            m_cloudButton = true;
        }
    }
	if (m_triangleDePascale)
	{
		m_triangleDePascale = false;
		Courbe c;
		std::vector<glm::vec2> cp;
		std::vector<int> coef;
		std::vector<Model*> model;
		std::vector<Model*> segement;
		for (int i = 0; i < m_points_clouds.size(); i++)
		{
			glm::vec3 pos = m_points_clouds[i]->getPosition();
			cp.push_back(glm::vec2(pos.x, pos.z));
			coef.push_back(1);
			model.push_back(m_points_clouds[i]);
		}
		c.cloudPoint = cp;
		c.coefControl = coef;
		c.pas = 50;
		c.points = model;

		std::vector<glm::vec2> data = trianglePascal(c);
		for (int i = 0; i < data.size()-1; i++)
		{
			segement.push_back(createSegment(data[i], data[i + 1]));
		}
		c.segments = segement;
		m_points_clouds.clear();
	}
	if (m_decastelJau)
	{
		Courbe c;
		std::vector<glm::vec2> cp;
		std::vector<int> coef;
		std::vector<Model*> model;
		std::vector<Model*> segement;
		for (int i = 0; i < m_points_clouds.size(); i++)
		{
			glm::vec3 pos = m_points_clouds[i]->getPosition();
			cp.push_back(glm::vec2(pos.x, pos.z));
			coef.push_back(1);
			model.push_back(m_points_clouds[i]);
		}
		c.cloudPoint = cp;
		c.coefControl = coef;
		c.pas = 50;
		c.points = model;

		std::vector<glm::vec2> data = DeCasteljau(c);
		for (int i = 0; i < data.size() - 1; i++)
		{
			segement.push_back(createSegment(data[i], data[i + 1]));
		}
		c.segments = segement;
		m_points_clouds.clear();
		m_decastelJau = false;
	}
}

void ExtrusionManager::stop()
{
	
}

void ExtrusionManager::preRender(VulkanMisc* vM)
{

}

void ExtrusionManager::render(VulkanMisc* vM)
{
    ImGuiIO& io = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing;
    
    if (ImGui::Begin("Extrusion", &m_open, window_flags))
    {
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Extrusion");
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 windowSize = ImGui::GetWindowSize();
        ImVec2 rectMin = windowPos;
        ImVec2 rectMax = ImVec2(windowPos.x+ windowSize.x, windowPos.y + windowSize.y);
        m_isMouseOverUI = ImGui::IsMouseHoveringRect(rectMin, rectMax);
        if (ImGui::Checkbox("Change Camera",&m_cameraChange)){}
        if (m_cloudPoint)
        {
            if (ImGui::Button("Close"))
            {
                m_cloudPoint = false;
            }
        }
        else
        {
            if (ImGui::Button("Create Cloud Point"))
            {
				m_clearCloudPoint = m_points_clouds.size() > 0;
                m_cloudPoint = true;
                m_cloudButton = false;
            }
        }
		if (ImGui::Button("Triangle de Pascal"))
		{
			m_triangleDePascale = m_points_clouds.size() >= 3 && !m_cloudPoint;
		}
		if (ImGui::Button("DeCasteljau"))
		{
			m_decastelJau = m_points_clouds.size() >= 3 && !m_cloudPoint;
		}
    }
    ImGui::End();
}

void ExtrusionManager::onGUI()
{

}