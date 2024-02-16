#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Shader/Shader.h>

namespace Hollow
{
	class ResourceLayout;

	struct RUNTIME_API ComputePipelineDesc
	{
		ArrayList<SharedPtr<ResourceLayout>> ResourceLayouts;
		SharedPtr<Shader> ComputeShader;
	};
}
