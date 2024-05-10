#pragma once

#include <Runtime/Core/Definitions.h>
#include <Runtime/Core/StdFix.h>
#include <Runtime/Window/WindowEventType.h>
#include <Runtime/Window/WindowMode.h>

namespace Hollow
{
	struct RUNTIME_API WindowEventDesc
	{
		WindowEventType EventType;

		Vector2u WindowSize;
		Vector2i WindowPosition;
		WindowMode WindowMode;

		ArrayList<String> DroppedFiles;
	};
}

