#include "GraphicsInstance.h"

#include <Runtime/Vulkan/Instance/VulkanInstance.h>

namespace Hollow
{
	SharedPtr<GraphicsInstance> GraphicsInstance::CreateInstance(const GraphicsInstanceDesc& desc)
	{
		switch (desc.API)
		{
		case GraphicsAPI::Vulkan:
			return std::make_shared<VulkanInstance>(desc);
		default:
			DEV_LOG(HE_ERROR, "Invalid Graphics API");
			return nullptr;
		}
	}

	void GraphicsInstance::EnumerateAdapters()
	{
		EnumerateAdaptersImpl();
	}

	SharedPtr<GraphicsDevice> GraphicsInstance::CreateDevice()
	{
		mDevice = CreateDeviceImpl();
		return mDevice;
	}
}