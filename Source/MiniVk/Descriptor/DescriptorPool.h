#pragma once

#include <Core/Core.h>
#include <Renderer/Renderer.h>

#include <vulkan.h>

namespace MiniVk
{
	enum class DescriptorType
	{
		Sampler,
		CombinedImageSampler,
		SampledImage,
		StorageImage,
		UniformTexelBuffer,
		StorageTexelBuffer,
		UniformBuffer,
		StorageBuffer,
		UniformBufferDynamic,
		StorageBufferDynamic,
		InputAttachment
	};

	struct DescriptorEntry
	{
		DescriptorType Type;
		uint32 Count;
	};

	struct DescriptorPoolDesc
	{
		Array<DescriptorEntry> DescriptorEntries;
		uint32 MaxSets;
	};

	class DescriptorPool
	{
	public:
		DescriptorPool(const DescriptorPoolDesc& desc, Renderer* pRenderer);
		~DescriptorPool();

		VkDescriptorPool GetPool() const { return mDescriptorPool; }

	private:
		VkDescriptorPool mDescriptorPool;
		VkDevice mDevice;
	};
}