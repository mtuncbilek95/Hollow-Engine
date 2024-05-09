#pragma once

#include <Runtime/Core/Definitions.h>
#include <Runtime/Core/StdFix.h>

namespace Hollow
{
	enum class RUNTIME_API WindowEventType
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
