#include "VulkanAdapter.h"

#include <Runtime/Vulkan/Device/VulkanDevice.h>

namespace Hollow
{
	VulkanAdapter::VulkanAdapter(const GraphicsAdapterDesc& desc, const VkPhysicalDevice device) : GraphicsAdapter(desc), 
		mVkPhysicalDevice(device)
	{
	}

	void VulkanAdapter::OnShutdown()
	{
		CORE_LOG(HE_WARNING, "VulkanAdapter", "Shutting down...");
	}

	SharedPtr<GraphicsDevice> VulkanAdapter::CreateDeviceCore(const GraphicsDeviceDesc& desc)
	{
		return std::make_shared<VulkanDevice>(desc);
	}
}
