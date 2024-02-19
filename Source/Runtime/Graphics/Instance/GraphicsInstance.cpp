#include "GraphicsInstance.h"

#include <Runtime/Vulkan/Instance/VulkanInstance.h>

namespace Hollow
{
	SharedPtr<GraphicsInstance> GraphicsInstance::Create(const GraphicsInstanceDesc& desc)
	{
		switch (desc.API)
		{
		case GraphicsAPI::D3D11:
			return nullptr;
		case GraphicsAPI::Vulkan:
			return std::make_shared<VulkanInstance>(desc);
		default:
			DEV_LOG(HE_ERROR, "Invalid Graphics API");
			return nullptr;
		}
	}
}
