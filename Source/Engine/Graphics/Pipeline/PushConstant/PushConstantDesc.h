#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Shader/ShaderStage.h>

namespace Hollow
{
	struct ENGINE_API PushConstantRange
	{
		ShaderStage Stage;
		u32 Offset;
		u32 Size;
	};

	struct ENGINE_API PushConstantDesc
	{
		DArray<PushConstantRange> PushConstantRanges;
	};
}
