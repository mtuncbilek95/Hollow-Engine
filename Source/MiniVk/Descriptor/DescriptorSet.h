#pragma once

#include <Core/Core.h>
#include <Renderer/Renderer.h>

#include <vulkan.h>

namespace MiniVk
{
	class DescriptorPool;
	class DescriptorLayout;

	struct DescriptorSetDesc
	{
		DescriptorPool* pPool;
		DescriptorLayout* pLayout;
	};

	class DescriptorSet
	{
	public:
		DescriptorSet(const DescriptorSetDesc& desc, Renderer* pRenderer);
		~DescriptorSet();

		VkDescriptorSet GetDescriptorSet() const { return mDescriptorSet; }

	private:
		VkDescriptorSet mDescriptorSet;
		VkDescriptorPool mOwnerPool;
		VkDescriptorSetLayout mTargetLayout;
		VkDevice mDevice;
	};
}