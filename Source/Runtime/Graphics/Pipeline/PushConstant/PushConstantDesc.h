#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Shader/ShaderStage.h>

namespace Hollow
{
	struct RUNTIME_API PushConstantRange
	{
		ShaderStage Stage;
		u32 Offset;
		u32 Size;
	};

	struct RUNTIME_API PushConstantDesc
	{
		DArray<PushConstantRange> PushConstantRanges;
	};
}
