#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Pipeline/InputLayout/InputLayoutDesc.h>
#include <Runtime/Graphics/Pipeline/DepthStencil/DepthStencilDesc.h>
#include <Runtime/Graphics/Pipeline/Rasterizer/RasterizerDesc.h>
#include <Runtime/Graphics/Pipeline/BlendState/BlendStateDesc.h>
#include <Runtime/Graphics/Shader/Shader.h>

namespace Hollow
{
	struct RUNTIME_API GraphicsPipelineDesc
	{
		ArrayList<SharedPtr<Shader>> Shaders;
		InputLayoutDesc InputLayout;
		RasterizerDesc Rasterizer;
		DepthStencilDesc DepthStencil;
		BlendStateDesc Blend;
		PrimitiveMode PrimitiveMode;
	};
}
