#include "Pipeline.h"

#include <Shader/Shader.h>
#include <RenderPass/RenderPass.h>
#include <Descriptor/DescriptorLayout.h>

namespace MiniVk
{
	Pipeline::Pipeline(const GraphicsPipelineDesc& desc, Renderer* pRenderer) : mShaders(desc.Shaders), mPipeline(VK_NULL_HANDLE),
		mPipelineLayout(VK_NULL_HANDLE), mRenderPass(desc.pRenderPass)
	{
		mDevice = pRenderer->GetVkLogicalDevice();

		// Create the shader stages
		Array<VkPipelineShaderStageCreateInfo> shaderStages(desc.Shaders.size());

		for (uint32 i = 0; i < desc.Shaders.size(); i++)
		{
			shaderStages[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

			switch (desc.Shaders[i]->GetStage())
			{
			case ShaderStage::Vertex:
				shaderStages[i].stage = VK_SHADER_STAGE_VERTEX_BIT;
				break;
			case ShaderStage::Fragment:
				shaderStages[i].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
				break;
			case ShaderStage::Geometry:
				shaderStages[i].stage = VK_SHADER_STAGE_GEOMETRY_BIT;
				break;
			case ShaderStage::TessellationControl:
				shaderStages[i].stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
				break;
			case ShaderStage::TessellationEvaluation:
				shaderStages[i].stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
				break;
			case ShaderStage::Compute:
				shaderStages[i].stage = VK_SHADER_STAGE_COMPUTE_BIT;
				break;
			default:
				DEV_LOG(HE_ERROR, "Unknown shader stage!");
				break;
			}

			shaderStages[i].module = desc.Shaders[i]->GetShaderModule();
			shaderStages[i].pName = desc.Shaders[i]->GetEntryPoint().c_str();
		}

		// Create InputLayout
		VkVertexInputBindingDescription binding = {};
		binding.binding = 0;
		binding.stride = desc.InputStride;
		binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		Array<VkVertexInputAttributeDescription> attributes(2);

		attributes[0].binding = 0;
		attributes[0].location = 0;
		attributes[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributes[0].offset = desc.InputOffsets[0];

		attributes[1].binding = 0;
		attributes[1].location = 1;
		attributes[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributes[1].offset = desc.InputOffsets[1];

		// Create InputAssembly
		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		// Create Dynamic State
		Array<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		VkPipelineDynamicStateCreateInfo dynamicState = {};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = static_cast<uint32>(dynamicStates.size());
		dynamicState.pDynamicStates = dynamicStates.data();

		// Create VertexInputState
		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.pVertexBindingDescriptions = &binding; // Optional
		vertexInputInfo.vertexAttributeDescriptionCount = attributes.size(); // Optional
		vertexInputInfo.pVertexAttributeDescriptions = attributes.data(); // Optional

		// Create Viewport
		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)pRenderer->GetWindowSize().x;
		viewport.height = (float)pRenderer->GetWindowSize().y;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		// Create Scissor
		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = { pRenderer->GetWindowSize().x, pRenderer->GetWindowSize().y };

		// Create ViewportState
		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;

		// Create RasterizationState
		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;
		rasterizer.depthBiasConstantFactor = 0.0f; // Optional
		rasterizer.depthBiasClamp = 0.0f; // Optional
		rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

		// Create MultisampleState
		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampling.minSampleShading = 1.0f; // Optional
		multisampling.pSampleMask = nullptr; // Optional
		multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
		multisampling.alphaToOneEnable = VK_FALSE; // Optional

		// Create ColorBlendAttachment
		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

		// Create ColorBlendState
		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f; // Optional
		colorBlending.blendConstants[1] = 0.0f; // Optional
		colorBlending.blendConstants[2] = 0.0f; // Optional
		colorBlending.blendConstants[3] = 0.0f; // Optional

		// Descriptor Set Layouts
		Array<VkDescriptorSetLayout> layouts(desc.DescriptorLayouts.size());
		for (uint32 i = 0; i < desc.DescriptorLayouts.size(); i++)
		{
			layouts[i] = desc.DescriptorLayouts[i]->GetLayout();
		}

		// Pipeline Layout Info
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = layouts.size(); // Optional
		pipelineLayoutInfo.pSetLayouts = layouts.data(); // Optional
		pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
		pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional
		pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional

		DEV_ASSERT(vkCreatePipelineLayout(mDevice, &pipelineLayoutInfo, nullptr, &mPipelineLayout) == VK_SUCCESS, "PipelineLayout", "Failed to create pipeline layout!");

		CORE_LOG(HE_VERBOSE, "Pipeline", "PipelineLayout created successfully!");

		// Create Pipeline
		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = static_cast<uint32>(shaderStages.size());
		pipelineInfo.pStages = shaderStages.data();
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = nullptr;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = &dynamicState;
		pipelineInfo.layout = mPipelineLayout;
		pipelineInfo.renderPass = desc.pRenderPass->GetRenderPass();
		pipelineInfo.subpass = 0;

		DEV_ASSERT(vkCreateGraphicsPipelines(mDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &mPipeline) == VK_SUCCESS, "GraphicsPipeline", "Failed to create graphics pipeline!");

		CORE_LOG(HE_VERBOSE, "Pipeline", "GraphicsPipeline created successfully!");
	}

	Pipeline::~Pipeline()
	{
		vkDestroyPipeline(mDevice, mPipeline, nullptr);
		vkDestroyPipelineLayout(mDevice, mPipelineLayout, nullptr);
	}
}