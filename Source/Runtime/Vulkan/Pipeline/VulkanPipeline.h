#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Pipeline/Pipeline.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanPipeline : public Pipeline
	{
	public:
		VulkanPipeline(const GraphicsPipelineDesc& desc, VulkanDevice* pDevice);
		VulkanPipeline(const ComputePipelineDesc& desc, VulkanDevice* pDevice);
		~VulkanPipeline() override = default;

		void OnShutdown() noexcept override;
	private:
		VkPipeline mVkPipeline;
		VkPipelineLayout mVkPipelineLayout;
		VkDevice mVkLogicalDevice;
	};
}