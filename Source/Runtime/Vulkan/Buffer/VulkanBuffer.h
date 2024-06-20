#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Buffer/GraphicsBuffer.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanBuffer final : public GraphicsBuffer
	{
	public:
		VulkanBuffer(const GraphicsBufferDesc& desc,const SharedPtr<VulkanDevice> pDevice);
		~VulkanBuffer() override;

		VkBuffer GetVkBuffer() const { return mVkBuffer; }
		VkDevice GetVkDevice() const { return mVkDevice; }

		u64 GetVkOffset() const { return mOffset; }
		u64 GetVkAlignedOffset() const { return mAlignedOffset; }

	private:
		VkBuffer mVkBuffer;
		VkDevice mVkDevice;

		u64 mOffset;
		u64 mAlignedOffset;
	};
}