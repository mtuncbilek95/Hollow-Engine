#pragma once

#include <Engine/Core/Core.h>

namespace Hollow
{
	enum class CompareOperation : u8
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