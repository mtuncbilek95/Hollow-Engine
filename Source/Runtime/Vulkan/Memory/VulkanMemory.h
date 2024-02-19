#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Memory/GraphicsMemory.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanMemory : public GraphicsMemory
	{
	public:
		VulkanMemory(const GraphicsMemoryDesc& desc, VulkanDevice* pDevice);
		~VulkanMemory() override = default;

		const VkDeviceMemory& GetVkDeviceMemory() const noexcept { return mVkDeviceMemory; }

	private:
		VkDeviceMemory mVkDeviceMemory;
		VkDevice mVkLogicalDevice;
	};
}
