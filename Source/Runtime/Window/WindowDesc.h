#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	struct RUNTIME_API WindowDesc
	{
		Vector2u WindowSize;
		Vector2i WindowPosition;
		String WindowTitle;
		bool Windowed;
	};
}
