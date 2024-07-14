#include "VQueue.h"

namespace Hollow
{
	VQueue::VQueue(const GraphicsQueueDesc& desc, VkQueue queue, SharedPtr<GraphicsDevice> pDevice) : GraphicsQueue(desc, pDevice), mQueue(queue)
	{
	}
}
