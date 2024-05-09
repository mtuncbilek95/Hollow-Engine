#pragma once

#include <Runtime/Core/Definitions.h>
#include <Runtime/Core/StdFix.h>
#include <Runtime/Window/WindowMode.h>

namespace Hollow
{
	struct RUNTIME_API WindowDesc
	{
		Vector2u WindowSize;
		Vector2i WindowPosition;
		string WindowTitle;
		WindowMode WindowMode;
	};
}
