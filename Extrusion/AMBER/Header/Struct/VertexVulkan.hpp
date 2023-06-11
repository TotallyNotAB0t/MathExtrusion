#ifndef __ENGINE_VERTEX_VULKAN__
#define __ENGINE_VERTEX_VULKAN__

#include "vulkan/vulkan.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include <array>
#include <functional>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

struct Vertex
{
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 texCoord;
    glm::vec3 tangents;

    static VkVertexInputBindingDescription getBindingDescription()
    {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static VkVertexInputAttributeDescription getShadowAttributeDescriptions()
    {
        VkVertexInputAttributeDescription attributeDescriptions{};

        attributeDescriptions.binding = 0;
        attributeDescriptions.location = 0;
        attributeDescriptions.format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions.offset = offsetof(Vertex, pos);

        return attributeDescriptions;
    }

    static std::array<VkVertexInputAttributeDescription, 5> getAttributeDescriptions()
    {
        std::array<VkVertexInputAttributeDescription, 5> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, normal);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, color);

        attributeDescriptions[3].binding = 0;
        attributeDescriptions[3].location = 3;
        attributeDescriptions[3].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[3].offset = offsetof(Vertex, texCoord);

        attributeDescriptions[4].binding = 0;
        attributeDescriptions[4].location = 4;
        attributeDescriptions[4].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[4].offset = offsetof(Vertex, tangents);

        return attributeDescriptions;
    }

    bool operator==(const Vertex &other) const
    {
        return pos == other.pos && normal == other.normal && color == other.color && texCoord == other.texCoord;
    }

    static void hashCombine(std::size_t& hash, std::size_t v)
    {        
        hash ^= v + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }
};

namespace std //pour combiner correctement les champs d'une structure
{
	template<> struct hash<Vertex>
	{    
		size_t operator()(Vertex const& vertex) const
		{			
            auto hash = std::size_t{0};
            Vertex::hashCombine(hash,std::hash<glm::vec3>()(vertex.pos));
            Vertex::hashCombine(hash,std::hash<glm::vec3>()(vertex.normal));
            Vertex::hashCombine(hash,std::hash<glm::vec3>()(vertex.color));
            Vertex::hashCombine(hash,std::hash<glm::vec2>()(vertex.texCoord));
            Vertex::hashCombine(hash, std::hash<glm::vec3>()(vertex.tangents));
            return hash;
		}
	};
}

#endif //__ENGINE_VERTEX_VULKAN__