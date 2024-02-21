#include "VulkanPipeline.h"

#include <Runtime/Vulkan/Shader/VulkanShader.h>

#include <Runtime/Vulkan/Pipeline/VulkanPipelineUtils.h>

namespace Hollow
{
	VulkanPipeline::VulkanPipeline(const GraphicsPipelineDesc& desc, VulkanDevice* pDevice) : Pipeline(desc, pDevice)
	{
		mVkLogicalDevice = pDevice->GetVkLogicalDevice();

		// Create shader stage for the pipeline
		Array<VkPipelineShaderStageCreateInfo> shaderStages;

		for (auto& shader : desc.Shaders)
		{
			VulkanShader* pShader = static_cast<VulkanShader*>(shader.get());
			VkPipelineShaderStageCreateInfo shaderStage = {};
			shaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			shaderStage.stage = VulkanPipelineUtils::GetVkShaderStage(pShader->GetType());
			shaderStage.module = pShader->GetVkShaderModule();
			shaderStage.pName = pShader->GetEntryPoint().c_str();
			shaderStages.push_back(shaderStage);
		}

		// Create input state for the pipeline
		VkPipelineVertexInputStateCreateInfo vertexInputState = {};
		vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputState.vertexBindingDescriptionCount = 0;
		vertexInputState.pVertexBindingDescriptions = nullptr;
		vertexInputState.vertexAttributeDescriptionCount = 0;
		vertexInputState.pVertexAttributeDescriptions = nullptr;

		// Input assembly state
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = {};
		inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssemblyState.topology = VulkanPipelineUtils::GetVkPrimitiveMode(desc.PrimitiveMode);
		inputAssemblyState.primitiveRestartEnable = VK_FALSE;

		// Viewport description
		VkViewport viewport = {};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(desc.Viewport.ImageSize.x);
		viewport.height = static_cast<float>(desc.Viewport.ImageSize.y);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		// Scissor description
		VkRect2D scissor = {};
		scissor.offset = { 0, 0 };
		scissor.extent = { desc.Scissor.ImageSize.x, desc.Scissor.ImageSize.y };

		// Viewport & Scissor state
		VkPipelineViewportStateCreateInfo viewportState = {};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;

		// Dynamic state
		Array<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		VkPipelineDynamicStateCreateInfo dynamicState = {};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = static_cast<uint32>(dynamicStates.size());
		dynamicState.pDynamicStates = dynamicStates.data();

		// Rasterization state
		VkPipelineRasterizationStateCreateInfo rasterizationState = {};
		rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizationState.depthClampEnable = VK_FALSE;
		rasterizationState.rasterizerDiscardEnable = VK_FALSE;
		rasterizationState.polygonMode = VulkanPipelineUtils::GetVkPolygonMode(desc.Rasterizer.FillMode);
		rasterizationState.lineWidth = 1.0f;
		rasterizationState.cullMode = VulkanPipelineUtils::GetVkCullMode(desc.Rasterizer.CullMode);
		rasterizationState.frontFace = VulkanPipelineUtils::GetVkFrontFace(desc.Rasterizer.FaceOrdering);
		rasterizationState.depthBiasEnable = VK_FALSE;
		rasterizationState.depthBiasConstantFactor = 0.0f;
		rasterizationState.depthBiasClamp = 0.0f;
		rasterizationState.depthBiasSlopeFactor = 0.0f;

		// Multisample state
		VkPipelineMultisampleStateCreateInfo multisampleState = {};
		multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampleState.sampleShadingEnable = VK_FALSE;
		multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		// Depth stencil state
		VkPipelineDepthStencilStateCreateInfo depthStencilState = {};
		depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencilState.depthTestEnable = desc.DepthStencil.DepthEnable ? VK_TRUE : VK_FALSE;

		// TO BE CONTINUED
	}

	VulkanPipeline::VulkanPipeline(const ComputePipelineDesc& desc, VulkanDevice* pDevice) : Pipeline(desc, pDevice)
	{
		mVkLogicalDevice = pDevice->GetVkLogicalDevice();
	}

	void VulkanPipeline::OnShutdown() noexcept
	{
		if(mVkPipeline != VK_NULL_HANDLE)
		{
			vkDestroyPipeline(mVkLogicalDevice, mVkPipeline, nullptr);
			mVkPipeline = VK_NULL_HANDLE;
		}

		if(mVkPipelineLayout != VK_NULL_HANDLE)
		{
			vkDestroyPipelineLayout(mVkLogicalDevice, mVkPipelineLayout, nullptr);
			mVkPipelineLayout = VK_NULL_HANDLE;
		}
	}

}
