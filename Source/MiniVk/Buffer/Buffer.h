#pragma once

#include <Core/Core.h>
#include <Renderer/Renderer.h>

#include <vulkan.h>

namespace MiniVk
{
	enum class BufferUsage : byte
	{
		Vertex = 1,
		Index = 2,
		Uniform = 4,
		Storage = 8,
		TransferSrc = 16,
		TransferDst = 32
	};

	GENERATE_ENUM_FLAG(BufferUsage, byte);

	struct BufferDesc
	{
		BufferUsage Usage;
		uint32 SizeInBytes;
	};

	class Buffer
	{
	public:
		Buffer(const BufferDesc& desc, Renderer* pRenderer);
		~Buffer();

		VkBuffer GetBuffer() const { return mVertexBuffer; }
		VkDeviceMemory GetBufferMemory() const { return mVertexBufferMemory; }

		uint32 GetSizeInBytes() const { return mSizeInBytes; }
		BufferUsage GetUsage() const { return mUsage; }
	private:
		uint32 FindMemoryType(uint32 typeFilter, VkMemoryPropertyFlags properties);
	private:
		VkBuffer mVertexBuffer;
		VkDeviceMemory mVertexBufferMemory;
		VkDevice mDevice;
		VkPhysicalDevice mPhysicalDevice;

		uint32 mSizeInBytes;
		BufferUsage mUsage;
	};
}