#ifndef __ENGINE_GRAPHIC_OBJECT__
#define __ENGINE_GRAPHIC_OBJECT__

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/common.hpp"
#include "Debug.hpp"
#include "Component.hpp"
#include "imgui-cmake/Header/imgui.h"
#include "Transform.hpp"
#include <vector>
#include <string> 
#include <algorithm>

namespace Ge
{
	class GObject
	{
	public:
		GObject();
		~GObject();
		void setName(std::string nom);
		std::string * getName();
		virtual glm::mat4 getModelMatrix() const;
		virtual void setPosition(glm::vec3 pos);
		virtual void setRotation(glm::quat rot);
		virtual void setEulerAngles(glm::vec3 eul);
		virtual void setScale(glm::vec3 scale);
		virtual void setTarget(glm::vec3 target);
		virtual glm::vec3 getPosition();
		virtual glm::quat getRotation();
		virtual glm::vec3 getEulerAngles();
		virtual glm::vec3 getDirection();
		virtual glm::vec3 getScale();
		virtual void mapMemory() = 0;
		glm::vec3 transformDirectionAxeX();
		glm::vec3 transformDirectionAxeY();
		glm::vec3 transformDirectionAxeZ();
		glm::vec3 transformDirectionAxe(glm::vec3 dir);
		Transform globalTransform(const glm::vec3& localPosition, const glm::quat& localRotation, const glm::vec3& localScale, const glm::vec3& parentPosition, const glm::quat& parentRotation, const glm::vec3& parentScale);
		Transform globalTransform(const glm::vec3& localPosition, const glm::quat& localRotation, const GObject* parent);		
		Transform globalTransform(const glm::vec3& localPosition, const glm::vec3& localEuler, const glm::vec3& localScale, const glm::vec3& parentPosition, const glm::quat& parentRotation, const glm::vec3& parentScale);
		Transform globalTransform(const glm::vec3& localPosition, const glm::vec3& localEuler, const GObject* parent);
		void applyTransform(const glm::vec3& localPosition, const glm::quat& localRotation, const GObject* parent);
		void applyTransform(const glm::vec3& localPosition, const glm::vec3& localEuler, const GObject* parent);
		void addComponent(Component * c);
		void removeComponent(Component * c);
		static std::vector<GObject *> GetGObjects();
		virtual void onGUI();
	protected:
		static std::vector<GObject *> s_gobjects;
		GObject * m_parents;
		std::string m_nom;
		Transform m_transform{};
		glm::vec3 m_eulerAngles;//only UI
		std::vector<Component *> m_component;

	};
}

#endif //__ENGINE_GRAPHIC_OBJECT__