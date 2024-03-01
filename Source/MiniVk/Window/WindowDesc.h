#pragma once

#include <Core/Core.h>

namespace MiniVk
{
	struct WindowDesc
	{
		Vector2u WindowSize;
		Vector2i WindowPosition;
		String WindowTitle;
		bool Windowed;
	};
}
