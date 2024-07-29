#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Shader/ShaderStage.h>

namespace Hollow
{
	struct PushConstantRange
	{
		ShaderStage Stage;
		u32 Offset;
		u32 Size;
	};

	struct PushConstantDesc
	{
		DArray<PushConstantRange> PushConstantRanges;
	};
}
