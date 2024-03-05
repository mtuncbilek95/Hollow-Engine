#include "Buffer.h"

namespace MiniVk
{
	Buffer::Buffer(const BufferDesc& desc, Renderer* pRenderer) : mVertexBuffer(VK_NULL_HANDLE), mVertexBufferMemory(VK_NULL_HANDLE), 
		mUsage(desc.Usage), mSizeInBytes(desc.SizeInBytes)
	{
		mDevice = pRenderer->GetVkLogicalDevice();
		mPhysicalDevice = pRenderer->GetVkPhysicalDevice();

		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = desc.SizeInBytes;
		if ((desc.Usage & BufferUsage::Vertex))
			bufferInfo.usage |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		else if ((desc.Usage & BufferUsage::Index))
			bufferInfo.usage |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		else if ((desc.Usage & BufferUsage::Uniform))
			bufferInfo.usage |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		else if ((desc.Usage & BufferUsage::Storage))
			bufferInfo.usage |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
		if ((desc.Usage & BufferUsage::TransferSrc))
			bufferInfo.usage |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		else if ((desc.Usage & BufferUsage::TransferDst))
			bufferInfo.usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;

		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		DEV_ASSERT(vkCreateBuffer(pRenderer->GetVkLogicalDevice(), &bufferInfo, nullptr, &mVertexBuffer) == VK_SUCCESS, "VulkanBuffer", "Failed to create vertex buffer!");

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(mDevice, mVertexBuffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

		DEV_ASSERT(vkAllocateMemory(mDevice, &allocInfo, nullptr, &mVertexBufferMemory) == VK_SUCCESS, "VulkanBuffer", "Failed to allocate vertex buffer memory!");
		DEV_ASSERT(vkBindBufferMemory(mDevice, mVertexBuffer, mVertexBufferMemory, 0) == VK_SUCCESS, "VulkanBuffer", "Failed to bind vertex buffer memory!");
	}

	Buffer::~Buffer()
	{
		if (mVertexBufferMemory != VK_NULL_HANDLE)
			vkFreeMemory(mDevice, mVertexBufferMemory, nullptr);
		if (mVertexBuffer != VK_NULL_HANDLE)
			vkDestroyBuffer(mDevice, mVertexBuffer, nullptr);
	}

	uint32 Buffer::FindMemoryType(uint32 typeFilter, VkMemoryPropertyFlags properties)
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(mPhysicalDevice, &memProperties);

		for (uint32 i = 0; i < memProperties.memoryTypeCount; i++)
		{
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
				return i;
		}

		DEV_LOG(HE_ERROR, "VulkanBuffer", "Failed to find suitable memory type!");
		return 0;
	}
}