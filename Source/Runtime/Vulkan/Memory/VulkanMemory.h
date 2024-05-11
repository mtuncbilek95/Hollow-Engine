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
		VulkanMemory(const GraphicsMemoryDesc& desc, const SharedPtr<VulkanDevice> pDevice);
		~VulkanMemory() override = default;

		VkDeviceMemory GetVkDeviceMemory() const noexcept { return mVkDeviceMemory; }

		// Inherited via GraphicsMemory
		void OnShutdown() noexcept override;

	protected:
		uint32 FindMemoryType(GraphicsMemoryType type);

	private:
		VkDeviceMemory mVkDeviceMemory;
		VkPhysicalDevice mVkPhysicalDevice;
		VkDevice mVkDevice;
	};
}
