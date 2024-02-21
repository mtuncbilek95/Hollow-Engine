#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Pipeline/InputLayout/InputLayoutDesc.h>
#include <Runtime/Graphics/Pipeline/DepthStencil/DepthStencilDesc.h>
#include <Runtime/Graphics/Pipeline/Rasterizer/RasterizerDesc.h>
#include <Runtime/Graphics/Pipeline/BlendState/BlendStateDesc.h>
#include <Runtime/Graphics/Pipeline/Multisample/MultisampleDesc.h>
#include <Runtime/Graphics/Pipeline/PipelineLayout/PipelineLayoutDesc.h>
#include <Runtime/Graphics/RenderPass/RenderPass.h>
#include <Runtime/Graphics/Command/ViewportDesc.h>
#include <Runtime/Graphics/Command/ScissorDesc.h>

#include <Runtime/Graphics/Shader/Shader.h>

namespace Hollow
{
	struct RUNTIME_API GraphicsPipelineDesc
	{
		Array<SharedPtr<Shader>> Shaders;
		InputLayoutDesc InputLayout;
		RasterizerDesc Rasterizer;
		DepthStencilDesc DepthStencil;
		BlendStateDesc Blend;
		PrimitiveMode PrimitiveMode;
		MultisampleDesc Multisample;
		PipelineLayoutDesc DescriptorLayout;
		SharedPtr<RenderPass> pRenderPass;
		ViewportDesc Viewport;
		ScissorDesc Scissor;
		uint32 Subpass = 0;
	};
}
