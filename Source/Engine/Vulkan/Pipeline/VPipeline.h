#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Pipeline/Pipeline.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	class VDevice;
	class VPipeline final : public Pipeline
	{
	public:
		VPipeline(const GraphicsPipelineDesc& desc, WeakPtr<VDevice> pDevice);
		~VPipeline() override = default;

		VkPipeline GetVkPipeline() const { return mVkPipeline; }
		VkPipelineLayout GetVkPipelineLayout() const { return mVkPipelineLayout; }

	private:
		VkPipeline mVkPipeline;
		VkPipelineLayout mVkPipelineLayout;
		VkDevice mVkDevice;
	};
}
