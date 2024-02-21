#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Pipeline/PipelineLayout/PipelineLayoutDesc.h>
#include <Runtime/Graphics/Shader/Shader.h>

namespace Hollow
{
	struct RUNTIME_API ComputePipelineDesc
	{
		Array<SharedPtr<PipelineLayoutDesc>> DescriptorLayout;
		SharedPtr<Shader> ComputeShader;
	};
}
