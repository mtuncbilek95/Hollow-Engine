#include "GraphicsDevice.h"

namespace Hollow
{
	GraphicsDevice::GraphicsDevice(const GraphicsDeviceDesc& desc) : mGraphicsAPI(desc.API), mGraphicsInstance(desc.Instance), mSwapchain(nullptr)
	{
		// Check for the best adapter
		GraphicsAdapter tempAdapter = {};
		for (auto& adapter : desc.Instance->GetAdapters())
		{
			tempAdapter = adapter.AdapterScore > tempAdapter.AdapterScore ? adapter : tempAdapter;
		}

		mGraphicsAdapter = tempAdapter;
	}
}
