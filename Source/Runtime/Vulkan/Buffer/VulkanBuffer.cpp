#include "VulkanBuffer.h"

#include <Runtime/Vulkan/Buffer/VulkanBufferUtils.h>
#include <Runtime/Vulkan/Common/VulkanCommonUtils.h>

namespace Hollow
{
	VulkanBuffer::VulkanBuffer(const GraphicsBufferDesc& desc, VulkanDevice* pDevice) : GraphicsBuffer(desc, pDevice), mVkBuffer(VK_NULL_HANDLE),
		mVkDevice(pDevice->GetVkDevice())
	{
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = desc.SizeInBytes;
		bufferInfo.usage = VulkanBufferUtils::GetVkBufferUsage(desc.Usage);
		bufferInfo.sharingMode = VulkanCommonUtils::GetVkSharingMode(desc.ShareMode);

		DEV_ASSERT(vkCreateBuffer(mVkDevice, &bufferInfo, nullptr, &mVkBuffer) == VK_SUCCESS, "VulkanBuffer", "Failed to create buffer");
	}

	void VulkanBuffer::OnShutdown() noexcept
	{
		if(mVkBuffer != VK_NULL_HANDLE)
			vkDestroyBuffer(mVkDevice, mVkBuffer, nullptr);

		mVkBuffer = VK_NULL_HANDLE;
		mVkDevice = VK_NULL_HANDLE;

		CORE_LOG(HE_INFO, "VulkanBuffer", "Buffer has been destroyed");
	}
}