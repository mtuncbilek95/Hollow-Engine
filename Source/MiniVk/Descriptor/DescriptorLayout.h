#pragma once

#include <Core/Core.h>
#include <Renderer/Renderer.h>
#include <Descriptor/DescriptorPool.h>
#include <Shader/Shader.h>

#include <vulkan.h>

namespace MiniVk
{
	struct LayoutEntry
	{
		DescriptorType Type;
		ShaderStage Stage;
		uint32 Binding;
	};

	struct DescriptorLayoutDesc
	{
		Array<LayoutEntry> LayoutEntries;
	};

	class DescriptorLayout
	{
	public:
		DescriptorLayout(const DescriptorLayoutDesc& desc, Renderer* pRenderer);
		~DescriptorLayout();

		const VkDescriptorSetLayout GetLayout() const { return mLayout; }

	private:
		VkDescriptorSetLayout mLayout;
		VkDevice mDevice;
	};
}