#include "VulkanPipeline.h"

#include <Runtime/Vulkan/Shader/VulkanShader.h>
#include <Runtime/Vulkan/Core/VulkanCoreUtils.h>
#include <Runtime/Vulkan/Shader/VulkanShaderUtils.h>
#include <Runtime/Vulkan/Pipeline/VulkanPipelineUtils.h>
#include <Runtime/Vulkan/Texture/VulkanTextureUtils.h>
#include <Runtime/Graphics/Texture/TextureUtils.h>
#include <Runtime/Vulkan/Descriptor/VulkanDescriptorLayout.h>

namespace Hollow
{
	VulkanPipeline::VulkanPipeline(const GraphicsPipelineDesc& desc, SharedPtr<VulkanDevice> pDevice) : Pipeline(desc, pDevice), mVkDevice(pDevice->GetVkDevice()), 
		mVkPipeline(VK_NULL_HANDLE), mVkPipelineLayout(VK_NULL_HANDLE)
	{
		DArray<VkPipelineShaderStageCreateInfo> shaderStages;
		for (const auto& shader : desc.GraphicsShaders)
		{
			VkPipelineShaderStageCreateInfo stageInfo = {};
			stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			stageInfo.stage = VulkanShaderUtils::GetVkShaderStageBit(shader->GetStage());
			stageInfo.module = std::static_pointer_cast<VulkanShader>(shader)->GetVkShaderModule();
			stageInfo.pName = shader->GetEntryPoint().c_str();

			shaderStages.push_back(stageInfo);
		}

		// Create Attribute Bindings
		DArray<VkVertexInputBindingDescription> bindings;
		DArray<VkVertexInputAttributeDescription> attributes;
		for (byte bindIndex = 0; bindIndex < desc.InputLayout.Bindings.size(); bindIndex++)
		{
			const auto& element = desc.InputLayout.Bindings[bindIndex];
			VkVertexInputBindingDescription binding = {};
			binding.binding = bindIndex;
			binding.inputRate = VulkanPipelineUtils::GetVkVertexInputRate(element.StepRate);

			u32 offset = 0;
			for (byte attIndex = 0; attIndex < element.Elements.size(); attIndex++)
			{
				const auto& attribute = element.Elements[attIndex];
				VkVertexInputAttributeDescription attr = {};
				attr.binding = bindIndex;
				attr.location = attIndex;
				attr.format = VulkanTextureUtils::GetVkTextureFormat(attribute.Format);
				attr.offset = offset;
				attributes.push_back(attr);

				offset += TextureUtils::GetTextureFormatSize(attribute.Format);
			}

			binding.stride = offset;
			bindings.push_back(binding);
		}

		// Create InputAssembly
		VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VulkanPipelineUtils::GetVkPrimitiveTopology(desc.InputLayout.Topology);
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		// Create VertexInputState
		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = bindings.size();
		vertexInputInfo.pVertexBindingDescriptions = bindings.data();
		vertexInputInfo.vertexAttributeDescriptionCount = attributes.size();
		vertexInputInfo.pVertexAttributeDescriptions = attributes.data();

		// Create Dynamic State
		//DArray<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		//VkPipelineDynamicStateCreateInfo dynamicState = {};
		//dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		//dynamicState.dynamicStateCount = static_cast<u32>(dynamicStates.size());
		//dynamicState.pDynamicStates = dynamicStates.data();

		// Create Viewport
		VkViewport viewport{};
		viewport.x = static_cast<f32>(desc.Viewport.OffsetSize.x);
		viewport.y = static_cast<f32>(desc.Viewport.OffsetSize.y);
		viewport.width = static_cast<f32>(desc.Viewport.ViewportSize.x);
		viewport.height = static_cast<f32>(desc.Viewport.ViewportSize.y);
		viewport.minDepth = desc.Viewport.DepthRange.x;
		viewport.maxDepth = desc.Viewport.DepthRange.y;

		// Create Scissor
		VkRect2D scissor{};
		scissor.offset = { static_cast<i32>(desc.Scissor.OffsetSize.x), static_cast<i32>(desc.Scissor.OffsetSize.y) };
		scissor.extent = { desc.Scissor.ScissorSize.x, desc.Scissor.ScissorSize.y };

		// Create ViewportState
		VkPipelineViewportStateCreateInfo viewportState = {};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;

		// Create RasterizationState
		VkPipelineRasterizationStateCreateInfo rasterizer = {};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VulkanPipelineUtils::GetVkPolygonMode(desc.RasterizerState.FillMode);
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VulkanPipelineUtils::GetVkCullMode(desc.RasterizerState.CullFlags);
		rasterizer.frontFace = desc.RasterizerState.bFrontCounterClockwise ? VK_FRONT_FACE_COUNTER_CLOCKWISE : VK_FRONT_FACE_CLOCKWISE;
		rasterizer.depthBiasEnable = desc.RasterizerState.bDepthBiasEnabled;
		rasterizer.depthBiasConstantFactor = desc.RasterizerState.DepthBiasFactor;
		rasterizer.depthBiasClamp = desc.RasterizerState.DepthBiasClamp;
		rasterizer.depthBiasSlopeFactor = desc.RasterizerState.DepthBiasSlope;

		// Create MultisampleState
		VkPipelineMultisampleStateCreateInfo multisampling = {};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = desc.Multisample.bSampleShadingEnabled;
		multisampling.rasterizationSamples = VulkanTextureUtils::GetVkSampleCount(desc.Multisample.Samples);
		multisampling.minSampleShading = 1.0f;
		multisampling.pSampleMask = nullptr;
		multisampling.alphaToCoverageEnable = VK_FALSE;
		multisampling.alphaToOneEnable = VK_FALSE;

		// Create ColorBlendAttachment
		DArray<VkPipelineColorBlendAttachmentState> colorAttachments;

		for (byte i = 0; i < desc.BlendState.Attachments.size(); i++)
		{
			VkPipelineColorBlendAttachmentState colorBlendAttachment{};
			colorBlendAttachment.colorWriteMask = VulkanPipelineUtils::GetVkColorWriteMask(desc.BlendState.Attachments[i].WriteMask);
			colorBlendAttachment.blendEnable = desc.BlendState.Attachments[i].bEnabled;
			colorBlendAttachment.srcColorBlendFactor = VulkanPipelineUtils::GetVkBlendFactor(desc.BlendState.Attachments[i].SourceColorFactor); // Optional
			colorBlendAttachment.dstColorBlendFactor = VulkanPipelineUtils::GetVkBlendFactor(desc.BlendState.Attachments[i].DestinationColorFactor); // Optional
			colorBlendAttachment.colorBlendOp = VulkanPipelineUtils::GetVkBlendOperation(desc.BlendState.Attachments[i].ColorOperation); // Optional
			colorBlendAttachment.srcAlphaBlendFactor = VulkanPipelineUtils::GetVkBlendFactor(desc.BlendState.Attachments[i].SourceAlphaFactor); // Optional
			colorBlendAttachment.dstAlphaBlendFactor = VulkanPipelineUtils::GetVkBlendFactor(desc.BlendState.Attachments[i].DestinationAlphaFactor); // Optional
			colorBlendAttachment.alphaBlendOp = VulkanPipelineUtils::GetVkBlendOperation(desc.BlendState.Attachments[i].AlphaOperation); // Optional

			colorAttachments.push_back(colorBlendAttachment);
		}

		// Create ColorBlendState
		VkPipelineColorBlendStateCreateInfo colorBlending = {};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = desc.BlendState.bLogicOperationEnabled;
		colorBlending.logicOp = VulkanCoreUtils::GetVkLogicOperation(desc.BlendState.LogicOperation); // Optional
		colorBlending.attachmentCount = colorAttachments.size();
		colorBlending.pAttachments = colorAttachments.data();
		colorBlending.blendConstants[0] = 0.0f; // Optional
		colorBlending.blendConstants[1] = 0.0f; // Optional
		colorBlending.blendConstants[2] = 0.0f; // Optional
		colorBlending.blendConstants[3] = 0.0f; // Optional

		// Create DepthStencilState
		VkPipelineDepthStencilStateCreateInfo depthStencil = {};
		depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencil.depthTestEnable = desc.DepthStencilState.bDepthTestEnabled;
		depthStencil.depthWriteEnable = desc.DepthStencilState.bDepthWriteEnabled;
		depthStencil.depthCompareOp = VulkanCoreUtils::GetVkCompareOperation(desc.DepthStencilState.DepthTestOperation);
		depthStencil.depthBoundsTestEnable = VK_FALSE;
		depthStencil.stencilTestEnable = desc.DepthStencilState.bStencilTestEnabled;
		depthStencil.front = VulkanPipelineUtils::GetVkStencilOpState(desc.DepthStencilState.StencilFrontFace);
		depthStencil.back = VulkanPipelineUtils::GetVkStencilOpState(desc.DepthStencilState.StencilBackFace);
		depthStencil.minDepthBounds = 0.0f; // Optional
		depthStencil.maxDepthBounds = 1.0f; // Optional

		// Descriptor Set Layouts
		DArray<VkDescriptorSetLayout> layouts(desc.ResourceLayout.ResourceLayouts.size());

		for (u32 i = 0; i < desc.ResourceLayout.ResourceLayouts.size(); i++)
		{
			layouts[i] = std::static_pointer_cast<VulkanDescriptorLayout>(desc.ResourceLayout.ResourceLayouts[i])->GetVkDescriptorLayout();
		}

		DArray<VkFormat> formats;
		for(auto& format : desc.ColorAttachmentFormats)
			formats.push_back(VulkanTextureUtils::GetVkTextureFormat(format));

		VkPipelineRenderingCreateInfo renderingInfo = {};
		renderingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
		renderingInfo.colorAttachmentCount = desc.ColorAttachmentCount;
		renderingInfo.pColorAttachmentFormats = formats.data();
		renderingInfo.depthAttachmentFormat = VulkanTextureUtils::GetVkTextureFormat(desc.DepthAttachmentFormat);
		renderingInfo.stencilAttachmentFormat = VulkanTextureUtils::GetVkTextureFormat(desc.StencilAttachmentFormat);
		renderingInfo.pNext = nullptr;
		renderingInfo.viewMask = 0;

		DArray<VkPushConstantRange> pushConstants;

		for(PushConstantRange pushConstant : desc.PushConstants.PushConstantRanges)
		{
			VkPushConstantRange range = {};
			range.stageFlags = VulkanShaderUtils::GetVkShaderStageSingle(pushConstant.Stage);
			range.offset = pushConstant.Offset;
			range.size = pushConstant.Size;

			pushConstants.push_back(range);
		}

		// Pipeline Layout Info
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = layouts.size();
		pipelineLayoutInfo.pSetLayouts = layouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = pushConstants.size();
		pipelineLayoutInfo.pPushConstantRanges = pushConstants.data();

		CORE_ASSERT(vkCreatePipelineLayout(mVkDevice, &pipelineLayoutInfo, nullptr, &mVkPipelineLayout) == VK_SUCCESS, "PipelineLayout", "Failed to create pipeline layout!");

		// Create Pipeline
		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = static_cast<u32>(shaderStages.size());
		pipelineInfo.pStages = shaderStages.data();
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = &depthStencil;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = nullptr;
		pipelineInfo.layout = mVkPipelineLayout;
		pipelineInfo.renderPass = nullptr;
		pipelineInfo.subpass = 0;
		pipelineInfo.pNext = &renderingInfo;

		CORE_ASSERT(vkCreateGraphicsPipelines(mVkDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &mVkPipeline) == VK_SUCCESS, "GraphicsPipeline", "Failed to create graphics pipeline!");
	}

	VulkanPipeline::~VulkanPipeline()
	{
		if (mVkPipeline != VK_NULL_HANDLE)
			vkDestroyPipeline(mVkDevice, mVkPipeline, nullptr);
		if (mVkPipelineLayout != VK_NULL_HANDLE)
			vkDestroyPipelineLayout(mVkDevice, mVkPipelineLayout, nullptr);

		mVkPipeline = VK_NULL_HANDLE;
		mVkPipelineLayout = VK_NULL_HANDLE;
		mVkDevice = VK_NULL_HANDLE;
	}
}
