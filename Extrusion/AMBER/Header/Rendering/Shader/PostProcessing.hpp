#ifndef __POST_PROCESSING__
#define __POST_PROCESSING__

#include "VulkanMisc.hpp"
#include "Debug.hpp"

namespace Ge
{
	class PostProcessing
	{
	public:
		bool initialize(VulkanMisc* vM);
		void release();
	private:
		VulkanMisc* vulkanM;
	};
}

#endif //!__POST_PROCESSING__