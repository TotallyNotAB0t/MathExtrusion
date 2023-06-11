#include "GObject.hpp"

namespace Ge
{
	std::vector<GObject *> GObject::s_gobjects;
	GObject::GObject()
	{
		m_nom = "NoName";
		m_transform.position = glm::vec3(0.0f);
		m_transform.rotation = glm::quat(0,0,0,0);
		m_eulerAngles = glm::vec3(0.0f);
		m_transform.scale = glm::vec3(1.0f);		
		s_gobjects.push_back(this);
	}

	GObject::~GObject()
	{
		s_gobjects.erase(std::remove(s_gobjects.begin(), s_gobjects.end(), this), s_gobjects.end());
	}

	std::vector<GObject *> GObject::GetGObjects()
	{
		return s_gobjects;
	}

	void GObject::setName(std::string name)
	{
		m_nom = name;
	}

	std::string *GObject::getName()
	{
		return &m_nom;
	}

	void GObject::setPosition(glm::vec3 pos)
	{
		m_transform.position = pos;
		mapMemory();
	}

	void GObject::setEulerAngles(glm::vec3 euler)
	{
		m_eulerAngles = euler;
		m_transform.rotation = glm::quat(glm::radians(euler));
		mapMemory();
	}

	void GObject::setRotation(glm::quat rot)
	{		
		m_transform.rotation = rot;
		m_eulerAngles = GObject::getEulerAngles();
		mapMemory();
	}

	void GObject::setScale(glm::vec3 scale)
	{
		m_transform.scale = scale;
		mapMemory();
	}

	void GObject::setTarget(glm::vec3 target)
	{
		glm::vec3 direction = glm::normalize(target - m_transform.position);
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::mat4 rotationMatrix = glm::lookAt(m_transform.position, target, up);		
		m_transform.rotation = glm::quat_cast(rotationMatrix);
		mapMemory();
	}

	glm::vec3 GObject::getPosition()
	{
		return m_transform.position;
	}

	glm::quat GObject::getRotation()
	{
		return m_transform.rotation;
	}

	glm::vec3 GObject::getEulerAngles()
	{
		return glm::degrees(glm::eulerAngles(m_transform.rotation));
	}

	glm::vec3 GObject::getDirection()
	{
		return glm::rotate(m_transform.rotation, glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::vec3 GObject::getScale()
	{
		return m_transform.scale;
	}

	glm::vec3 GObject::transformDirectionAxeX()
	{
		return glm::rotate(m_transform.rotation, glm::vec3(-1.0f, 0.0f, 0.0f));
	}

	glm::vec3 GObject::transformDirectionAxeY()
	{
		return glm::rotate(m_transform.rotation, glm::vec3(0.0f, -1.0f, 0.0f));
	}

	glm::vec3 GObject::transformDirectionAxeZ()
	{
		return glm::rotate(m_transform.rotation, glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::vec3 GObject::transformDirectionAxe(glm::vec3 dir)
	{
		return glm::rotate(m_transform.rotation, dir);
	}

	Transform GObject::globalTransform(const glm::vec3& localPosition, const glm::quat& localRotation, const glm::vec3& localScale,const glm::vec3& parentPosition, const glm::quat& parentRotation, const glm::vec3& parentScale)
	{		
		Transform globalTransform;
		glm::mat4 localMatrix = glm::translate(glm::mat4(1.0f), localPosition) * glm::mat4_cast(localRotation) * glm::scale(glm::mat4(1.0f), localScale);
		glm::mat4 parentMatrix = glm::translate(glm::mat4(1.0f), parentPosition) * glm::mat4_cast(parentRotation) * glm::scale(glm::mat4(1.0f), parentScale);	
		glm::mat4 globalMatrix = parentMatrix * localMatrix;
		globalTransform.position = glm::vec3(globalMatrix[3][0], globalMatrix[3][1], globalMatrix[3][2]);
		globalTransform.rotation = glm::quat_cast(globalMatrix);
		globalTransform.scale = localScale;
		return globalTransform;
	}

	Transform GObject::globalTransform(const glm::vec3& localPosition, const glm::quat& localRotation, const GObject* parent)
	{
		Transform globalTransform;
		glm::mat4 localMatrix = glm::translate(glm::mat4(1.0f), localPosition) * glm::mat4_cast(localRotation) * glm::scale(glm::mat4(1.0f), m_transform.scale);
		glm::mat4 parentMatrix = glm::translate(glm::mat4(1.0f), parent->m_transform.position) * glm::mat4_cast(parent->m_transform.rotation) * glm::scale(glm::mat4(1.0f), parent->m_transform.scale);
		glm::mat4 globalMatrix = parentMatrix * localMatrix;
		globalTransform.position = glm::vec3(globalMatrix[3][0], globalMatrix[3][1], globalMatrix[3][2]);
		globalTransform.rotation = glm::quat_cast(globalMatrix);
		globalTransform.scale = m_transform.scale;
		return globalTransform;
	}

	void GObject::applyTransform(const glm::vec3& localPosition, const glm::quat& localRotation,const GObject * parent)
	{
		glm::mat4 localMatrix = glm::translate(glm::mat4(1.0f), localPosition) * glm::mat4_cast(localRotation) * glm::scale(glm::mat4(1.0f), m_transform.scale);
		glm::mat4 parentMatrix = glm::translate(glm::mat4(1.0f), parent->m_transform.position) * glm::mat4_cast(parent->m_transform.rotation) * glm::scale(glm::mat4(1.0f), parent->m_transform.scale);
		glm::mat4 globalMatrix = parentMatrix * localMatrix;
		m_transform.position = glm::vec3(globalMatrix[3][0], globalMatrix[3][1], globalMatrix[3][2]);
		m_transform.rotation = glm::quat_cast(globalMatrix);
		m_eulerAngles = getEulerAngles();
		mapMemory();
	}

	Transform GObject::globalTransform(const glm::vec3& localPosition, const glm::vec3& localEuler, const glm::vec3& localScale, const glm::vec3& parentPosition, const glm::quat& parentRotation, const glm::vec3& parentScale)
	{
		return globalTransform(localPosition, glm::quat(glm::radians(localEuler)), localScale, parentPosition, parentRotation, parentScale);
	}

	Transform GObject::globalTransform(const glm::vec3& localPosition, const glm::vec3& localEuler, const GObject* parent)
	{
		return globalTransform(localPosition, glm::quat(glm::radians(localEuler)), parent);
	}

	void GObject::applyTransform(const glm::vec3& localPosition, const glm::vec3& localEuler, const GObject* parent)
	{
		applyTransform(localPosition, glm::quat(glm::radians(localEuler)),parent);
	}

	void GObject::addComponent(Component *c)
	{
		m_component.push_back(c);
	}

	void GObject::removeComponent(Component *c)
	{
		m_component.erase(std::remove(m_component.begin(), m_component.end(), c), m_component.end());
	}

	glm::mat4 GObject::getModelMatrix() const
	{
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), m_transform.position);
		glm::mat4 rotationMatrix = glm::toMat4(glm::quat(m_transform.rotation));
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_transform.scale);
		return translationMatrix * rotationMatrix * scaleMatrix;
	}

	void GObject::onGUI()
	{
		if (ImGui::DragFloat3("Position", (float *)&m_transform.position, 0.2f))
		{
			setPosition(m_transform.position);
		}

		if (ImGui::DragFloat3("EulerAngles", (float*)&m_eulerAngles, 0.2f))
		{
			setEulerAngles(m_eulerAngles);
		}

		if (ImGui::DragFloat4("Rotation", (float *)&m_transform.rotation, 0.2f))
		{
			setRotation(m_transform.rotation);
		}

		if (ImGui::DragFloat3("Scale", (float *)&m_transform.scale, 0.2f))
		{
			setScale(m_transform.scale);
		}

		/*if (ImGui::DragFloat3("Direction", (float*)&m_transform.direction, 0.2f))
		{
			
		}*/

		for (Component *comp : m_component)
		{
			comp->onGUI();
		}
	}
}