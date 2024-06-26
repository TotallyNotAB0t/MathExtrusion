#include "ExtrusionManager.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

ExtrusionManager::ExtrusionManager(Model* plane)
{
	m_plane = plane;
	m_planeShowHide = true;
}

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

	// Fonction d'interpolation lin�aire entre deux points
	auto interpolate = [](glm::vec2 p1, glm::vec2 p2, float t)
	{
		float x = (1 - t) * p1.x + t * p2.x;
		float y = (1 - t) * p1.y + t * p2.y;
		return glm::vec2(x, y);
	};

	for (int t = 0; t <= courbe.pas-1; ++t)
	{
		float u = static_cast<float>(t) / (courbe.pas);

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

void ExtrusionManager::updateCourbe(int i)
{
	std::vector<glm::vec2> data;
	if (m_courbes[i].courbetype == CourbeType::DeCasteljau)
	{
		data = DeCasteljau(m_courbes[i]);
	}
	else
	{
		data = trianglePascal(m_courbes[i]);
	}

	glm::vec2 pos;
	for (int j = 0; j < m_courbes[i].points.size(); j++)
	{		
		pos = m_courbes[i].cloudPoint[j];
		if (!m_courbes[i].rotation)
		{
			m_courbes[i].points[j]->setPosition(glm::vec3(pos.x, 0.0f, pos.y));
			m_courbes[i].light[j]->setPosition(glm::vec3(pos.x, 0.0f, pos.y));
		}
		else
		{
			m_courbes[i].points[j]->setPosition(glm::vec3(pos.x, pos.y, 0.0f));
			m_courbes[i].light[j]->setPosition(glm::vec3(pos.x, pos.y, 0.0f));
		}
	}



	for (int j = 0; j < m_courbes[i].segments.size(); j++)
	{
		glm::vec2 p1 = data[j];
		glm::vec2 p2 = data[j+1];
		glm::vec2 pos = (p1 + p2) / 2.0f;
		float scale = glm::distance(p1, p2) / 2.0f;
		if (!m_courbes[i].rotation)
		{
			m_courbes[i].segments[j]->setPosition(glm::vec3(pos.x, 0.0f, pos.y));
		}
		else
		{
			m_courbes[i].segments[j]->setPosition(glm::vec3(pos.x, pos.y, 0.0f));
		}
		m_courbes[i].segments[j]->setScale(glm::vec3(scale, m_size, m_size));
		glm::vec2 direction = p2 - p1;
		if (!m_courbes[i].rotation)
		{
			m_courbes[i].segments[j]->setEulerAngles(directionToRotation(glm::vec3(direction.x, 0.0f, direction.y)));
		}
		else
		{
			m_courbes[i].segments[j]->setEulerAngles(directionToRotation(glm::vec3(direction.x, direction.y, 0.0f)));
		}
	}
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

	// Calculer les coefficients binomiaux � l'aide du triangle de Pascal
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

	// G�n�rer les points de la courbe de B�zier
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
	m->setScale(glm::vec3(scale, m_size, m_size));
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
	GraphiquePipeline* gp = m_pc.graphiquePipelineManager->createPipeline("../Shader/frag_wave_unlit.spv", "../Shader/vert_wave_unlit.spv");
	GraphiquePipeline* gp_unlit = m_pc.graphiquePipelineManager->createPipeline("../Shader/frag_unlit.spv", "../Shader/vert_unlit.spv");
	GraphiquePipeline* gp1 = m_pc.graphiquePipelineManager->createPipeline("../Shader/frag_normal.spv", "../Shader/vert_normal.spv", false);
	


    m_pointMat = m_pc.materialManager->createMaterial();
    m_pointMat->setColor(glm::vec3(0.0f, 0.0f, 1.0f));
    m_pointMat->setMetallic(0.7f);
    m_pointMat->setRoughness(0.15f);
	m_pointMat->setPipeline(gp_unlit);

    m_segmentMat = m_pc.materialManager->createMaterial();      
    m_segmentMat->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
    m_segmentMat->setMetallic(0.7f);
    m_segmentMat->setRoughness(0.15f);
	m_segmentMat->setPipeline(gp);


	//allocateBuffer(float * pos, float * texCord, float * normal, unsigned int * indice, unsigned vertexSize, unsigned indiceSize)
/*float* pos = new float[4 * 3];
unsigned int* indice = new unsigned int[6];
float* texCord = new float[4*2];
float* normal = new float[4 * 3];*/

	/*float pos[] = {0.0f,0.0f,0.0f, 0.0f,0.0f,1.0f, 1.0f,0.0f,1.0f, 1.0f,0.0f,0.0f};
	unsigned int indice[] = { 0,1,2,  0,2,3 };
	float texCord[] = { 0.0f,0.0f, 0.0f,1.0f, 1.0f,1.0f, 1.0f,0.0f };
	float normal[] = { 0.0f,1.0f,0.0f, 0.0f,1.0f,0.0f, 0.0f,1.0f,0.0f, 0.0f,1.0f,0.0f };

	ShapeBuffer* sbb = m_pc.modelManager->allocateBuffer(pos, texCord, normal, indice, 4, 6);
	Model* modellqsdpf = m_pc.modelManager->createModel(sbb);
	modellqsdpf->setPosition(glm::vec3(0.0f, -0.5f, 0.0f));*/
}

void ExtrusionManager::fixedUpdate()
{

}

// PoyoCode
std::vector<glm::vec3> generateRevolutionPoints(const std::vector<glm::vec2>& bezierPoints, unsigned int revolutionSegments,Courbe c) 
{
	std::vector<glm::vec3> revolutionPoints;

	float angleStep = 2.0f * glm::pi<float>() / static_cast<float>(revolutionSegments);

	for (unsigned int segment = 0; segment < revolutionSegments; ++segment) 
	{
		float angle = static_cast<float>(segment) * angleStep;

		glm::mat3 rotationMatrix = glm::mat3(
			glm::cos(angle), 0.0f, glm::sin(angle),
			0.0f, 1.0f, 0.0f,
			-glm::sin(angle), 0.0f, glm::cos(angle)
		);		
		for (const auto& point : bezierPoints) 
		{
			glm::vec3 rotatedPoint;
			if (!c.rotation)
			{
				rotatedPoint = rotationMatrix * glm::vec3(point.x, 0.0f, point.y);
			}
			else
			{
				rotatedPoint = rotationMatrix * glm::vec3(point.x, point.y, 0.0f);
			}
			revolutionPoints.push_back(rotatedPoint);
		}
	}

	return revolutionPoints;
}

// PoyoCode
void generateRevolutionData(std::vector<glm::vec3>& revolutionPoints, Courbe c, std::vector<unsigned int>& indices, std::vector<float>& texCoords, std::vector<float>& normals) 
{
	for (int i = 0; i < c.pas; i++)
	{
		revolutionPoints.push_back(glm::vec3(revolutionPoints[i])*0.99999f);
	}
	for (size_t i = 0; i < c.revolutionSegments+1; i++)
	{
		for (size_t j = 0; j < c.pas; j++)
		{			
			size_t topIndex = c.pas * i + j;
			size_t bottomIndex = (c.pas * i + j+1) % revolutionPoints.size();
			size_t nextTopIndex = (c.pas * (i+1) + j)% revolutionPoints.size();
			size_t nextBottomIndex = (c.pas * (i + 1) + (j+1)) % revolutionPoints.size();

			indices.push_back(topIndex);
			indices.push_back(bottomIndex);
			indices.push_back(nextTopIndex);
			
			
			indices.push_back(bottomIndex);
			indices.push_back(nextBottomIndex);
			indices.push_back(nextTopIndex);
	
			texCoords.push_back(i * 0.25f);
			texCoords.push_back(j * 0.25f);

			// Normals
			glm::vec3 edge1 = revolutionPoints[nextBottomIndex] - revolutionPoints[topIndex];
			glm::vec3 edge2 = revolutionPoints[nextTopIndex] - revolutionPoints[topIndex];
			glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

			
			if (j == c.pas-1)
			{
				normals.push_back(-normal.x);
				normals.push_back(-normal.y);
				normals.push_back(-normal.z);
			}
			else
			{
				normals.push_back(normal.x);
				normals.push_back(normal.y);
				normals.push_back(normal.z);
			}
		}
	}

}

//BoicCode
std::vector<glm::vec3> generateCourbeExtrusionPoints(const std::vector<glm::vec2>& bezierPoints, unsigned int revolutionSegments, Courbe c)
{
	std::vector<glm::vec3> revolutionPoints;

	float angleStep = 2.0f * glm::pi<float>() / static_cast<float>(revolutionSegments);

	for (unsigned int segment = 0; segment < revolutionSegments; ++segment)
	{
		float angle = static_cast<float>(segment) * angleStep;
		
		
		for (size_t i = 0; i < bezierPoints.size(); i++)
		{
			glm::vec2 direction = glm::normalize(bezierPoints[i] - bezierPoints[(i+1)% bezierPoints.size()]);
			glm::vec2 tangent = glm::rotate(direction, glm::half_pi<float>());
			glm::mat4 translationMatrix = glm::mat4(1.0f);
			
			translationMatrix = glm::translate(translationMatrix, glm::vec3(bezierPoints[i].x, bezierPoints[i].y, 0.0f));
			translationMatrix = glm::rotate(translationMatrix, angle, glm::vec3(direction.x, 0.0f, direction.y));
			//revolutionPoints.push_back(glm::vec3(glm::vec4(tangent.x + bezierPoints[i].x, 0.0f, tangent.y + bezierPoints[i].y, 1.0f)));
			revolutionPoints.push_back(glm::vec3(glm::vec4(tangent.x, 0.0f, tangent.y, 1.0f) * translationMatrix) + glm::vec3(bezierPoints[i].x, 0.0f, bezierPoints[i].y));
		}
	}

	return revolutionPoints;
}
void generateCourbeExtrusionData(std::vector<glm::vec3>& revolutionPoints, Courbe c, std::vector<unsigned int>& indices, std::vector<float>& texCoords, std::vector<float>& normals)
{
	for (int i = 0; i < c.pas; i++)
	{
		revolutionPoints.push_back(glm::vec3(revolutionPoints[i]) * 0.99999f);
	}
	for (size_t i = 0; i < c.revolutionSegments; i++)
	{
		for (size_t j = 0; j < c.pas-1; j++)
		{
			size_t topIndex = c.pas * i + j;
			size_t bottomIndex = (c.pas * i + j + 1) % revolutionPoints.size();
			size_t nextTopIndex = (c.pas * (i + 1) + j) % revolutionPoints.size();
			size_t nextBottomIndex = (c.pas * (i + 1) + (j + 1)) % revolutionPoints.size();
			
			indices.push_back(topIndex);
			indices.push_back(nextTopIndex);
			indices.push_back(bottomIndex);
			

			indices.push_back(bottomIndex);
			indices.push_back(nextTopIndex);
			indices.push_back(nextBottomIndex);
			
			texCoords.push_back(i * 0.25f);
			texCoords.push_back(j * 0.25f);

			// Normals
			glm::vec3 edge1 = revolutionPoints[nextBottomIndex] - revolutionPoints[topIndex];
			glm::vec3 edge2 = revolutionPoints[nextTopIndex] - revolutionPoints[topIndex];
			glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));


			if (j == c.pas - 1)
			{
				normals.push_back(-normal.x);
				normals.push_back(-normal.y);
				normals.push_back(-normal.z);
			}
			else
			{
				normals.push_back(normal.x);
				normals.push_back(normal.y);
				normals.push_back(normal.z);
			}
			
		}
		indices.push_back(1);
		indices.push_back(0);
		indices.push_back(0);

	}

}

std::vector<glm::vec3> generateExtrusionPoints(const std::vector<glm::vec2>& bezierPoints, float height, float scaleFactor) 
{
	std::vector<glm::vec3> extrusionPoints;

	for (const auto& point : bezierPoints) {
		glm::vec3 bottomPoint(point.x, 0.0f, point.y);
		glm::vec3 topPoint(point.x * scaleFactor, height, point.y * scaleFactor);

		extrusionPoints.push_back(bottomPoint);
		extrusionPoints.push_back(topPoint);
	}

	return extrusionPoints;
}

void generateExtrusionData(const std::vector<glm::vec3>& extrusionPoints, std::vector<unsigned int>& indices, std::vector<float>& texCoords, std::vector<float>& normals) 
{
	size_t numPoints = extrusionPoints.size();

	for (size_t i = 0; i < numPoints; i += 2) 
	{
		size_t bottomIndex = i;
		size_t topIndex = i + 1;
		size_t nextBottomIndex = (i + 2) % numPoints;
		size_t nextTopIndex = (i + 3) % numPoints;

		// Indices
		indices.push_back(bottomIndex);
		indices.push_back(topIndex);
		indices.push_back(nextTopIndex);

		indices.push_back(bottomIndex);
		indices.push_back(nextTopIndex);
		indices.push_back(nextBottomIndex);

		// Texture coordinates
		float texCoordX = static_cast<float>(i) / static_cast<float>(numPoints);
		float nextTexCoordX = static_cast<float>(i + 2) / static_cast<float>(numPoints);

		texCoords.push_back(texCoordX);
		texCoords.push_back(0.0f);

		texCoords.push_back(texCoordX);
		texCoords.push_back(1.0f);

		// Normals
		glm::vec3 edge1 = extrusionPoints[nextBottomIndex] - extrusionPoints[bottomIndex];
		glm::vec3 edge2 = extrusionPoints[topIndex] - extrusionPoints[bottomIndex];
		glm::vec3 normal = glm::normalize(glm::cross(edge2, edge1));

		for (int j = 0; j < 2; ++j) 
		{
			normals.push_back(normal.x);
			normals.push_back(normal.y);
			normals.push_back(normal.z);
		}
	}
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
	if (m_planeActive)
	{
		m_planeActive = false;		
		m_plane->setScale(m_planeShowHide ? glm::vec3(10.0f, 1.0f, 10.0f) : glm::vec3(0.0f));
	}
	if (m_clearCloudPoint)
	{
		m_clearCloudPoint = false;
		for (int i = 0; i < m_points_clouds.size(); i++)
		{			
			m_pc.modelManager->destroyModel(m_points_clouds[i]);
			m_pc.lightManager->destroyLight(m_points_light_clouds[i]);
		}		
		m_points_clouds.clear();
		m_points_light_clouds.clear();
	}
    if (m_cloudPoint && !m_priority && !m_isMouseOverUI)
    {        
        if (m_pc.inputManager->getMouse(0))
        {
            if (m_cloudButton)
            {
                Model * m = m_pc.modelManager->createModel(m_sb);				
                m->setMaterial(m_pointMat);
                m->setScale(glm::vec3(m_size));				
                glm::vec2 mp = glm::vec2(m_pc.inputManager->getMousePosX(), m_pc.inputManager->getMousePosY());
                glm::vec2 ss = glm::vec2(m_pc.settingManager->getWindowWidth(), m_pc.settingManager->getWindowHeight());
                glm::vec2 pos = m_cam2D->ScreenToSpace(mp, ss);
                m->setPosition(glm::vec3(pos.x, 0.0f, pos.y));
				m_points_clouds.push_back(m);
                m_cloudButton = false;
				PointLight* pl = m_pc.lightManager->createPointLight(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				pl->setRange(1.0f);
				pl->setPosition(m->getPosition());
				m_points_light_clouds.push_back(pl);
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
		std::vector<Model*> segment;
		std::vector<PointLight*> plight;
		for (int i = 0; i < m_points_clouds.size(); i++)
		{
			glm::vec3 pos = m_points_clouds[i]->getPosition();
			cp.push_back(glm::vec2(pos.x, pos.z));
			coef.push_back(1);
			model.push_back(m_points_clouds[i]);
			plight.push_back(m_points_light_clouds[i]);
		}
		c.cloudPoint = cp;
		c.coefControl = coef;
		c.pas = 50;
		c.points = model;
		c.light = plight;
		std::vector<glm::vec2> data = trianglePascal(c);
		for (int i = 0; i < data.size()-1; i++)
		{
			segment.push_back(createSegment(data[i], data[i + 1]));
		}
		c.segments = segment;
		m_points_clouds.clear();
		m_points_light_clouds.clear();
		c.courbetype = CourbeType::TriangleDePascal;
		m_courbes.push_back(c);		
	}
	if (m_decastelJau)
	{
		m_decastelJau = false;
		Courbe c;
		std::vector<glm::vec2> cp;
		std::vector<int> coef;
		std::vector<Model*> model;
		std::vector<Model*> segment;
		std::vector<PointLight*> plight;
		for (int i = 0; i < m_points_clouds.size(); i++)
		{
			glm::vec3 pos = m_points_clouds[i]->getPosition();
			cp.push_back(glm::vec2(pos.x, pos.z));
			coef.push_back(1);
			model.push_back(m_points_clouds[i]);
			plight.push_back(m_points_light_clouds[i]);
		}
		c.cloudPoint = cp;
		c.coefControl = coef;
		c.pas = 50;
		c.height = 5.0f;
		c.scale = 1.0f;
		c.points = model;
		c.light = plight;

		std::vector<glm::vec2> data = DeCasteljau(c);
		for (int i = 0; i < data.size() - 1; i++)
		{
			segment.push_back(createSegment(data[i], data[i + 1]));
		}
		c.segments = segment;
		m_points_clouds.clear();
		m_points_light_clouds.clear();
		c.courbetype = CourbeType::DeCasteljau;
		m_courbes.push_back(c);
	}

	if (m_erase)
	{		
		Courbe c = m_courbes[m_listboxCurrentItem];
		std::vector<Model*> segments = c.segments;
		std::vector<Model*> points = c.points;
		std::vector<PointLight*> light = c.light;
		for (int i = 0; i < segments.size(); i++)
		{
			m_pc.modelManager->destroyModel(segments[i]);
		}
		c.segments.clear();
		for (int i = 0; i < points.size(); i++)
		{
			m_pc.modelManager->destroyModel(points[i]);
			m_pc.lightManager->destroyLight(light[i]);
		}
		m_points_clouds.clear();
		m_points_light_clouds.clear();
		c.points.clear();
		m_courbes.erase(m_courbes.begin()+m_listboxCurrentItem);
		m_erase = false;
	}

	if (m_eraseExtrusion)
	{
		m_pc.modelManager->destroyModel(m_extrusionList[m_listboxCurrentItemExtrusion]);
		m_extrusionList.erase(m_extrusionList.begin() + m_listboxCurrentItemExtrusion);
		m_eraseExtrusion = false;
	}

	if (m_pc.inputManager->getMouse(0) && !m_isMouseOverUI)
	{
		glm::vec2 mp = glm::vec2(m_pc.inputManager->getMousePosX(), m_pc.inputManager->getMousePosY());
		glm::vec2 ss = glm::vec2(m_pc.settingManager->getWindowWidth(), m_pc.settingManager->getWindowHeight());
		glm::vec2 pos = m_cam2D->ScreenToSpace(mp, ss);
		if (m_control_point < 0)
		{
			for (int i = 0; i < m_courbes.size(); i++)
			{
				for (int j = 0; j < m_courbes[i].cloudPoint.size(); j++)
				{
					if (glm::distance(m_courbes[i].cloudPoint[j], pos) < m_size*4.0f)
					{
						m_control_point = i;
						m_courbe_point = j;
					}
				}
			}
		}
		else
		{
			m_courbes[m_control_point].cloudPoint[m_courbe_point] = pos;
			updateCourbe(m_control_point);
		}
	}
	else
	{
		m_control_point = -1;
	}

	if (m_rotate)
	{
		m_rotate = false;
		Courbe c = m_courbes[m_listboxCurrentItem];
		c.rotation = !c.rotation;
		glm::mat4 rotationMatrix;
		if (c.rotation)
		{
			rotationMatrix = glm::toMat4(glm::quat(glm::radians(glm::vec3(-90.0f, 0.0f, 0.0f))));
		}
		else
		{
			rotationMatrix = glm::toMat4(glm::quat(glm::radians(glm::vec3(90.0f, 0.0f, 0.0f))));
		}
		
		for (size_t i = 0; i < c.points.size(); i++)
		{
			c.points[i]->setPosition(glm::vec3(glm::vec4(c.points[i]->getPosition(), 1.0f) * rotationMatrix));
		}
		m_courbes[m_listboxCurrentItem] = c;
		updateCourbe(m_listboxCurrentItem);
	}

	if (m_extrusion)
	{
		m_extrusion = false;

		Courbe c = m_courbes[m_listboxCurrentItem];

		std::vector<glm::vec2> data;
		if (c.courbetype == CourbeType::DeCasteljau)
		{
			data = DeCasteljau(c);
		}
		else
		{
			data = trianglePascal(c);
		}


		std::vector<glm::vec3> extrusionPoints = generateExtrusionPoints(data, c.height, c.scale);

		std::vector<unsigned int> indices;
		std::vector<float> texCoords;
		std::vector<float> normals;

		generateExtrusionData(extrusionPoints, indices, texCoords, normals);

		float* pos = new float[extrusionPoints.size() * 3];
		for (size_t i = 0; i < extrusionPoints.size(); ++i) 
		{
			pos[i * 3] = extrusionPoints[i].x;
			pos[i * 3 + 1] = extrusionPoints[i].y;
			pos[i * 3 + 2] = extrusionPoints[i].z;
		}

		ShapeBuffer* sbb = m_pc.modelManager->allocateBuffer(pos, texCoords.data(), normals.data(), indices.data(), extrusionPoints.size(), indices.size());
		Model* m = m_pc.modelManager->createModel(sbb);
		Materials* mat = m_pc.materialManager->createMaterial();
		mat->setAlbedoTexture(m_pc.textureManager->createTexture("../Texture/damier.png",false));
		if (m_normalShader)
		{
			mat->setPipeline(m_pc.graphiquePipelineManager->GetPipelines()[4]);
		}
		m->setMaterial(mat);
		mat->setMetallic(0.7f);
		mat->setRoughness(0.1f);
		m_extrusionList.push_back(m);
	}

	// PoyoCode
	if (m_extrusionRevolution)
	{
		m_extrusionRevolution = false;		
		Courbe c = m_courbes[m_listboxCurrentItem];

		std::vector<glm::vec2> data;
		if (c.courbetype == CourbeType::DeCasteljau)
		{
			data = DeCasteljau(c);
		}
		else
		{
			data = trianglePascal(c);
		}

		std::vector<glm::vec3> revolutionPoints = generateRevolutionPoints(data, c.revolutionSegments,c);

		std::vector<unsigned int> indices;
		std::vector<float> texCoords;
		std::vector<float> normals;

		generateRevolutionData(revolutionPoints,c, indices, texCoords, normals);		

		float* pos = new float[revolutionPoints.size() * 3];
		for (size_t i = 0; i < revolutionPoints.size(); i++) 
		{
			pos[i * 3] = revolutionPoints[i].x;
			pos[i * 3 + 1] = revolutionPoints[i].y;
			pos[i * 3 + 2] = revolutionPoints[i].z;
		}

		ShapeBuffer* sbb = m_pc.modelManager->allocateBuffer(pos, texCoords.data(), normals.data(), indices.data(), revolutionPoints.size(), indices.size());
		Model* m = m_pc.modelManager->createModel(sbb);
		//GraphiquePipeline* gp = m_pc.graphiquePipelineManager->createPipeline("../Shader/frag_normal.spv", "../Shader/vert_normal.spv",false);
		Materials* mat = m_pc.materialManager->createMaterial();
		if (m_normalShader)
		{
			mat->setPipeline(m_pc.graphiquePipelineManager->GetPipelines()[4]);
		}
		mat->setAlbedoTexture(m_pc.textureManager->createTexture("../Texture/damier.png", false));
		mat->setMetallic(0.8f);
		mat->setRoughness(0.15f);
		m->setMaterial(mat);
		m_extrusionList.push_back(m);
		
		
	}

	//BoicCode
	if (m_extrusionCourbe)
	{
		m_extrusionCourbe = false;
		Courbe c = m_courbes[m_listboxCurrentItem];

		std::vector<glm::vec2> data;
		if (c.courbetype == CourbeType::DeCasteljau)
		{
			data = DeCasteljau(c);
		}
		else
		{
			data = trianglePascal(c);
		}

		std::vector<glm::vec3> revolutionPoints = generateCourbeExtrusionPoints(data, c.revolutionSegments, c);
		

		std::vector<unsigned int> indices;
		std::vector<float> texCoords;
		std::vector<float> normals;

		generateCourbeExtrusionData(revolutionPoints, c, indices, texCoords, normals);

		float* pos = new float[revolutionPoints.size() * 3];
		for (size_t i = 0; i < revolutionPoints.size(); i++)
		{
			pos[i * 3] = revolutionPoints[i].x;
			pos[i * 3 + 1] = revolutionPoints[i].y;
			pos[i * 3 + 2] = revolutionPoints[i].z;
		}

		ShapeBuffer* sbb = m_pc.modelManager->allocateBuffer(pos, texCoords.data(), normals.data(), indices.data(), revolutionPoints.size(), indices.size());
		Model* m = m_pc.modelManager->createModel(sbb);
		Materials* mat = m_pc.materialManager->createMaterial();
		if (m_normalShader)
		{
			mat->setPipeline(m_pc.graphiquePipelineManager->GetPipelines()[4]);
		}
		mat->setAlbedoTexture(m_pc.textureManager->createTexture("../Texture/damier.png", false));
		mat->setMetallic(0.8f);
		mat->setRoughness(0.15f);
		m->setMaterial(mat);
		m_extrusionList.push_back(m);
	}

	if (m_pas)
	{		
		for (int i = 0; i < m_courbes[m_listboxCurrentItem].segments.size(); i++)
		{
			m_pc.modelManager->destroyModel(m_courbes[m_listboxCurrentItem].segments[i]);
		}
		m_courbes[m_listboxCurrentItem].segments.clear();
		std::vector<glm::vec2> data;
		if (m_courbes[m_listboxCurrentItem].courbetype == CourbeType::DeCasteljau)
		{
			data = DeCasteljau(m_courbes[m_listboxCurrentItem]);
		}
		else
		{
			data = trianglePascal(m_courbes[m_listboxCurrentItem]);
		}
		for (int i = 0; i < data.size() - 1; i++)
		{
			m_courbes[m_listboxCurrentItem].segments.push_back(createSegment(data[i], data[i + 1]));
		}
		m_pas = false;
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
		if (ImGui::Checkbox("Show/Hide Plane", &m_planeShowHide)) 
		{
			m_planeActive = true;
		}
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
			m_cloudPoint = false;
			m_triangleDePascale = m_points_clouds.size() >= 3 && !m_cloudPoint;
		}
		if (ImGui::Button("DeCasteljau"))
		{
			m_cloudPoint = false;
			m_decastelJau = m_points_clouds.size() >= 3 && !m_cloudPoint;
		}
		cnames.clear();
		cExtrusionNames.clear();
		valueS.clear();
		valueSExtrusion.clear();
		for (size_t i = 0; i < m_courbes.size(); ++i)
		{
			valueS.push_back(("Courbe " + std::to_string(i)));
			cnames.push_back(valueS[i].c_str());
		}

		for (size_t i = 0; i < m_extrusionList.size(); ++i)
		{
			valueSExtrusion.push_back(("Extrusion " + std::to_string(i)));
			cExtrusionNames.push_back(valueSExtrusion[i].c_str());
		}

		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Bezier list");
		ImGui::PushItemWidth(m_pc.settingManager->getWindowWidth() / 7.0f);
		ImGui::ListBox("###Hierarchy", &m_listboxCurrentItem, cnames.data(), cnames.size(), 5);
		ImGui::ListBox("###Hierarchy Two", &m_listboxCurrentItemExtrusion, cExtrusionNames.data(), cExtrusionNames.size(), 5);
		ImGui::PopItemWidth();
		ImGui::TextColored(ImVec4(0.2f, 1, 0.2f, 1), "Inspector\n\n");
		// Curves
		if (cnames.size() > 0)
		{
			ImGui::BeginChild("");
			if (m_courbes.size() > m_listboxCurrentItem)
			{				
				switch (m_courbes[m_listboxCurrentItem].courbetype)
				{
				case CourbeType::TriangleDePascal:
					ImGui::TextColored(ImVec4(0.0f, 0.0f, 1.0f, 1.0f), "TriangleDePascal");
					break;
				case CourbeType::DeCasteljau:
					ImGui::TextColored(ImVec4(0.0f, 0.0f, 1.0f, 1.0f), "DeCasteljau");
					break;
				default:
					break;
				};

				if (ImGui::InputInt("Pas", &(m_courbes[m_listboxCurrentItem].pas), 1, 2))
				{
					if (m_courbes[m_listboxCurrentItem].pas <= 0)
					{
						m_courbes[m_listboxCurrentItem].pas = 1;
					}
					m_pas = true;
				}	

				if (ImGui::InputFloat("Height", &(m_courbes[m_listboxCurrentItem].height), 0.25f, 2.0f))
				{
					if (m_courbes[m_listboxCurrentItem].height <= 0)
					{
						m_courbes[m_listboxCurrentItem].height = 1;
					}
				}

				if (ImGui::InputFloat("ScaleFactor", &(m_courbes[m_listboxCurrentItem].scale), 0.25f, 2.0f))
				{
					if (m_courbes[m_listboxCurrentItem].scale <= 0)
					{
						m_courbes[m_listboxCurrentItem].scale = 1;
					}
				}

				if (ImGui::InputInt("RevolutionSegments", &(m_courbes[m_listboxCurrentItem].revolutionSegments), 1, 2))
				{
					if (m_courbes[m_listboxCurrentItem].revolutionSegments <= 0)
					{
						m_courbes[m_listboxCurrentItem].revolutionSegments = 1;
					}
				}

				if (ImGui::Button("Rotation"))
				{
					m_rotate = true;
				}

				ImGui::Checkbox("Normal Shader", &m_normalShader);

				if (ImGui::Button("Extrusion Simple"))
				{
					m_extrusion = true;
				}

				// PoyoCode
				if (ImGui::Button("Extrusion par revolution"))
				{
					m_extrusionRevolution = true;
				}

				// BoicCode
				if (ImGui::Button("Extrusion sur courbe 2D"))
				{
					m_extrusionCourbe = true;
				}

				if (ImGui::Button("Supprimer"))
				{
					m_erase = true;
				}
			}
			ImGui::EndChild();
		}

		// Extrusions
		if (cExtrusionNames.size() > 0)
		{
			ImGui::BeginChild("");
			if (m_extrusionList.size() > m_listboxCurrentItemExtrusion)
			{

				if (ImGui::Button("Supprimer extrusion"))
				{
					m_eraseExtrusion = true;
				}
			}
			ImGui::EndChild();
		}

		ImGui::Spacing();
    }
    ImGui::End();
}

void ExtrusionManager::onGUI()
{

}