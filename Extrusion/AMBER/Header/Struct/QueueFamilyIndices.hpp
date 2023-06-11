#ifndef __ENGINE_QUEUE_FAMILY_INDICES__
#define __ENGINE_QUEUE_FAMILY_INDICES__

#include <set>
#include <stdexcept>
#include <vector>
#include <optional>

struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
    bool isComplete()
    {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

#endif //__ENGINE_QUEUE_FAMILY_INDICES__