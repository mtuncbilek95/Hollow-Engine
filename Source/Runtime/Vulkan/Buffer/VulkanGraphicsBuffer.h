#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Buffer/GraphicsBuffer.h>

#include <Runtime/Vulkan/Device/VulkanDevice.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanGraphicsBuffer : public GraphicsBuffer
	{
	public:
		VulkanGraphicsBuffer(const GraphicsBufferDesc& desc, VulkanDevice* pDevice);
		~VulkanGraphicsBuffer() override = default;

		VkBuffer GetVkBuffer() const noexcept { return mVkGraphicsBuffer; }

		void OnShutdown() noexcept override;
	
	private:
		VkBuffer mVkGraphicsBuffer;
		VkDevice mVkLogicalDevice;

		uint64 mVkMemoryOffset;
		uint64 mVkMemorySize;
	};
}