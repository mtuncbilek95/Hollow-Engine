#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Pipeline/Pipeline.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanPipeline final : public Pipeline
	{
	public:
		VulkanPipeline(const GraphicsPipelineDesc& desc, SharedPtr<VulkanDevice> pDevice);
		~VulkanPipeline() override;

		VkPipeline GetVkPipeline() const { return mVkPipeline; }
		VkPipelineLayout GetVkPipelineLayout() const { return mVkPipelineLayout; }

	private:
		VkPipeline mVkPipeline;
		VkPipelineLayout mVkPipelineLayout;
		VkDevice mVkDevice;
	};
}
