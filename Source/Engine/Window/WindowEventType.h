#pragma once

#include <Engine/Core/Definitions.h>
#include <Engine/Core/StdFix.h>

namespace Hollow
{
	enum class ENGINE_API WindowEventType
	{
		None,
		WindowClosed,
		WindowMoved,
		WindowResized,
		WindowFocused,
		WindowLostFocus,
		WindowMinimized,
		WindowMaximized,
		FileDropped
	};
}
