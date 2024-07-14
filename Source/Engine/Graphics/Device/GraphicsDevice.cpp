#include "GraphicsDevice.h"

namespace Hollow
{
	GraphicsDevice::GraphicsDevice(SharedInstance pInstance) : mInstance(pInstance)
	{
	}

	SharedPtr<GraphicsQueue> GraphicsDevice::CreateQueue(const GraphicsQueueDesc& desc)
	{
		return CreateQueueImpl(desc);
	}
}
