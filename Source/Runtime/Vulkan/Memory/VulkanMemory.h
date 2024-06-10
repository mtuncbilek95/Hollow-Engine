#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Memory/GraphicsMemory.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanMemory final : public GraphicsMemory
	{
	public:
		VulkanMemory(const GraphicsMemoryDesc& desc, const SharedPtr<VulkanDevice> pDevice);
		~VulkanMemory() override = default;

		VkDeviceMemory GetVkDeviceMemory() const noexcept { return mVkDeviceMemory; }

		// Inherited via GraphicsMemory
		void OnShutdown() noexcept override;

	protected:
		u32 FindMemoryType(GraphicsMemoryType type);

	private:
		VkDeviceMemory mVkDeviceMemory;
		VkPhysicalDevice mVkPhysicalDevice;
		VkDevice mVkDevice;
	};
}
