#pragma once

#include <Runtime/Core/Definitions.h>
#include <Runtime/Core/StdFix.h>
#include <Runtime/Window/WindowMode.h>

namespace Hollow
{
	struct RUNTIME_API WindowDesc
	{
		Vec2u WindowSize;
		Vec2i WindowPosition;
		String WindowTitle;
		WindowMode WindowMode;
	};
}
