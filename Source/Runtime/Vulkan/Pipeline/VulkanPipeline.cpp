#include "VulkanPipeline.h"

#include <Runtime/Vulkan/Shader/VulkanShader.h>

#include <Runtime/Graphics/Texture/TextureUtils.h>
#include <Runtime/Vulkan/Pipeline/VulkanPipelineUtils.h>
#include <Runtime/Vulkan/Texture/VulkanTextureUtils.h>
#include <Runtime/Vulkan/Common/VulkanCommonUtils.h>

namespace Hollow
{
	VulkanPipeline::VulkanPipeline(const GraphicsPipelineDesc& desc, VulkanDevice* pDevice) : Pipeline(desc, pDevice)
	{
		mVkLogicalDevice = pDevice->GetVkLogicalDevice();

		// Create input layout for the pipeline
		Array<VkVertexInputBindingDescription> vertexBindings;
		Array<VkVertexInputAttributeDescription> vertexAttributes;

		// Create vertex input binding and attribute descriptions per binding and attribute
		for (uint32 bindIndex = 0; bindIndex < desc.InputLayout.Bindings.size(); bindIndex++)
		{
			VkVertexInputBindingDescription bindingDesc = {};
			bindingDesc.binding = bindIndex;
			bindingDesc.inputRate = VulkanPipelineUtils::GetVkInputBindingStepRate(desc.InputLayout.Bindings[bindIndex].StepRate);

			uint32 inputOffset = 0;

			for (uint32 attribIndex = 0; attribIndex < desc.InputLayout.Bindings[bindIndex].Elements.size(); attribIndex++)
			{
				VkVertexInputAttributeDescription attributeDesc = {};
				attributeDesc.binding = bindIndex;
				attributeDesc.format = VulkanTextureUtils::GetVkTextureFormat(desc.InputLayout.Bindings[bindIndex].Elements[attribIndex].Format);
				attributeDesc.location = attribIndex;
				attributeDesc.offset = inputOffset;

				vertexAttributes.push_back(attributeDesc);

				inputOffset += TextureUtils::GetTextureSize(desc.InputLayout.Bindings[bindIndex].Elements[attribIndex].Format);
			}

			bindingDesc.stride = inputOffset;

			vertexBindings.push_back(bindingDesc);
		}

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
		vertexInputState.vertexBindingDescriptionCount = static_cast<uint32>(vertexBindings.size());
		vertexInputState.pVertexBindingDescriptions = vertexBindings.data();
		vertexInputState.vertexAttributeDescriptionCount = static_cast<uint32>(vertexAttributes.size());
		vertexInputState.pVertexAttributeDescriptions = vertexAttributes.data();
		vertexInputState.pNext = nullptr;

		// Input assembly state
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = {};
		inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssemblyState.topology = VulkanPipelineUtils::GetVkMeshTopology(desc.InputLayout.Topology);
		inputAssemblyState.primitiveRestartEnable = VK_FALSE;
		inputAssemblyState.pNext = nullptr;

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
		rasterizationState.cullMode = VulkanPipelineUtils::GetVkFaceCullMode(desc.Rasterizer.CullFlags);
		rasterizationState.frontFace = desc.Rasterizer.bFrontCounterClockwise ? VK_FRONT_FACE_COUNTER_CLOCKWISE : VK_FRONT_FACE_CLOCKWISE;
		rasterizationState.depthBiasEnable = desc.Rasterizer.bDepthBiasEnabled ? VK_TRUE : VK_FALSE;
		rasterizationState.depthBiasConstantFactor = desc.Rasterizer.DepthBiasFactor;
		rasterizationState.depthBiasClamp = desc.Rasterizer.DepthBiasClamp;
		rasterizationState.depthBiasSlopeFactor = desc.Rasterizer.DepthBiasSlope;

		// Multisample state
		VkPipelineMultisampleStateCreateInfo multisampleState = {};
		multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampleState.sampleShadingEnable = desc.Multisample.bSampleShadingEnabled;
		multisampleState.rasterizationSamples = VulkanTextureUtils::GetVkSampleCount(desc.Multisample.Samples);
		multisampleState.minSampleShading = 1.0f;
		multisampleState.pSampleMask = nullptr;
		multisampleState.alphaToCoverageEnable = 0;
		multisampleState.alphaToOneEnable = 0;

		// Depth stencil state
		VkPipelineDepthStencilStateCreateInfo depthStencilState = {};
		depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencilState.depthTestEnable = desc.DepthStencil.bDepthTestEnabled ? VK_TRUE : VK_FALSE;
		depthStencilState.depthWriteEnable = desc.DepthStencil.bDepthWriteEnabled ? VK_TRUE : VK_FALSE;
		depthStencilState.depthCompareOp = VulkanCommonUtils::GetVkCompareOperation(desc.DepthStencil.DepthTestOperation);
		depthStencilState.depthBoundsTestEnable = VK_FALSE;
		depthStencilState.stencilTestEnable = desc.DepthStencil.bStencilTestEnabled ? VK_TRUE : VK_FALSE;
		depthStencilState.front = VulkanPipelineUtils::GetVkStencilFaceDesc(desc.DepthStencil.StencilFrontFace);
		depthStencilState.back = VulkanPipelineUtils::GetVkStencilFaceDesc(desc.DepthStencil.StencilBackFace);
		depthStencilState.maxDepthBounds = 1.0f;
		depthStencilState.minDepthBounds = 0.0f;
		depthStencilState.pNext = nullptr;

		// Color blend attachments
		Array<VkPipelineColorBlendAttachmentState> colorBlendAttachments;
		for (auto& state : desc.Blend.Attachments)
		{
			VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
			colorBlendAttachment.colorWriteMask = VulkanPipelineUtils::GetVkBlendColorWriteMask(state.WriteMask);
			colorBlendAttachment.blendEnable = state.bEnabled ? VK_TRUE : VK_FALSE;
			colorBlendAttachment.srcColorBlendFactor = VulkanPipelineUtils::GetVkBlendFactor(state.SourceColorFactor);
			colorBlendAttachment.dstColorBlendFactor = VulkanPipelineUtils::GetVkBlendFactor(state.DestinationColorFactor);
			colorBlendAttachment.colorBlendOp = VulkanPipelineUtils::GetVkBlendOperation(state.ColorOperation);
			colorBlendAttachment.srcAlphaBlendFactor = VulkanPipelineUtils::GetVkBlendFactor(state.SourceAlphaFactor);
			colorBlendAttachment.dstAlphaBlendFactor = VulkanPipelineUtils::GetVkBlendFactor(state.DestinationAlphaFactor);
			colorBlendAttachment.alphaBlendOp = VulkanPipelineUtils::GetVkBlendOperation(state.AlphaOperation);

			colorBlendAttachments.push_back(colorBlendAttachment);
		}

		// Color blend state
		VkPipelineColorBlendStateCreateInfo colorBlendState = {};
		colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlendState.logicOpEnable = desc.Blend.bLogicOperationEnabled ? VK_TRUE : VK_FALSE;
		colorBlendState.logicOp = VulkanCommonUtils::GetVkLogicOperation(desc.Blend.LogicOperation);
		colorBlendState.attachmentCount = static_cast<uint32>(colorBlendAttachments.size());
		colorBlendState.pAttachments = colorBlendAttachments.data();
		colorBlendState.blendConstants[0] = 0.0f;
		colorBlendState.blendConstants[1] = 0.0f;
		colorBlendState.blendConstants[2] = 0.0f;
		colorBlendState.blendConstants[3] = 0.0f;

		Array<VkDescriptorSetLayout> descriptorSetLayouts;

		for (auto& layout : desc.DescriptorLayout.Layouts)
		{
			// TODO: Implement descriptor set layout
			//const VulkanDescriptorLayout* pLayout = static_cast<const VulkanDescriptorLayout*>(layout.get());
			//descriptorSetLayouts.push_back(pLayout->GetVkLayout());
		}

		// Pipeline layout
		VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32>(descriptorSetLayouts.size());
		pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;
		pipelineLayoutInfo.pNext = nullptr;

		DEV_ASSERT(vkCreatePipelineLayout(mVkLogicalDevice, &pipelineLayoutInfo, nullptr, &mVkPipelineLayout) == VK_SUCCESS, "VulkanPipeline", 
			"Failed to create pipeline layout");

		// Create pipeline
		VkGraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = static_cast<uint32>(shaderStages.size());
		pipelineInfo.pStages = shaderStages.data();
		pipelineInfo.pVertexInputState = &vertexInputState;
		pipelineInfo.pInputAssemblyState = &inputAssemblyState;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizationState;
		pipelineInfo.pMultisampleState = &multisampleState;
		pipelineInfo.pDepthStencilState = &depthStencilState;
		pipelineInfo.pColorBlendState = &colorBlendState;
		pipelineInfo.pDynamicState = &dynamicState;
		pipelineInfo.layout = mVkPipelineLayout;
		//pipelineInfo.renderPass = std::dynamic_pointer_cast<VulkanRenderPass>(desc.RenderPass)->GetVkRenderPass();
		pipelineInfo.subpass = desc.Subpass;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineInfo.basePipelineIndex = -1;
		pipelineInfo.flags = VkPipelineCreateFlags();
		pipelineInfo.pNext = nullptr;

		DEV_ASSERT(vkCreateGraphicsPipelines(mVkLogicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &mVkPipeline) == VK_SUCCESS, "VulkanPipeline", 
			"Failed to create graphics pipeline");

	}

	VulkanPipeline::VulkanPipeline(const ComputePipelineDesc& desc, VulkanDevice* pDevice) : Pipeline(desc, pDevice)
	{
		mVkLogicalDevice = pDevice->GetVkLogicalDevice();
	}

	void VulkanPipeline::OnShutdown() noexcept
	{
		if (mVkPipeline != VK_NULL_HANDLE)
		{
			vkDestroyPipeline(mVkLogicalDevice, mVkPipeline, nullptr);
			mVkPipeline = VK_NULL_HANDLE;
		}

		if (mVkPipelineLayout != VK_NULL_HANDLE)
		{
			vkDestroyPipelineLayout(mVkLogicalDevice, mVkPipelineLayout, nullptr);
			mVkPipelineLayout = VK_NULL_HANDLE;
		}
	}

}
