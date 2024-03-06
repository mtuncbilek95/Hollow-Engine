#pragma once

#include <Core/Core.h>
#include <Renderer/Renderer.h>

#include <vulkan.h>

namespace MiniVk
{
	struct DescriptorLayoutDesc
	{

	};

	class DescriptorLayout
	{
	public:
		DescriptorLayout(const DescriptorLayoutDesc& desc, Renderer* pRenderer);
		~DescriptorLayout();

	private:
	};
}