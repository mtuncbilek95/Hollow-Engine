#include "GraphicsInstance.h"

//#include <Runtime/Vulkan/Instance/VulkanInstance.h>

namespace Hollow
{
	sharedPtr<GraphicsInstance> GraphicsInstance::CreateInstance(const GraphicsInstanceDesc& desc)
	{
		switch (desc.API)
		{
		case GraphicsAPI::Vulkan:
			return 0;//std::make_shared<VulkanInstance>(desc);
		case GraphicsAPI::DX12:
			CORE_LOG(HE_ERROR, "GraphicsInstance", "DX12 is not supported yet");
			return nullptr;
		default:
			CORE_LOG(HE_ERROR, "GraphicsInstance", "Invalid Graphics API");
			return nullptr;
		}
	}

	void GraphicsInstance::EnumerateAdapters()
	{
		EnumerateAdaptersImpl();
	}

	sharedPtr<GraphicsDevice> GraphicsInstance::CreateDevice()
	{
		mDevice = CreateDeviceImpl();
		return mDevice;
	}
}