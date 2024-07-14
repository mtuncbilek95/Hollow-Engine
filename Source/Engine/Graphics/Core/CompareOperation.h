#pragma once

#include <Engine/Core/Core.h>

namespace Hollow
{
	enum class ENGINE_API CompareOperation : u8
	{
		Never,
		Less,
		Equal,
		LessEqual,
		Greater,
		NotEqual,
		GreaterEqual,
		Always
	};
}