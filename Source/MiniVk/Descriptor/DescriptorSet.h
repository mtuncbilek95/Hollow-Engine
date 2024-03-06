#pragma once

#include <Core/Core.h>
#include <Renderer/Renderer.h>

#include <vulkan.h>

namespace MiniVk
{
	struct DescriptorSetDesc
	{

	};

	class DescriptorSet
	{
	public:
		DescriptorSet(const DescriptorSetDesc& desc, Renderer* pRenderer);
		~DescriptorSet();

	private:
	};
}