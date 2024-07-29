#pragma once

#include <Engine/Core/Definitions.h>
#include <Engine/Core/StdFix.h>

namespace Hollow
{
	class PlatformClipboard final
	{
	public:
		static void SetClipboardText(const String& text);
		static String GetClipboardText();

	public:
		PlatformClipboard() = delete;
		~PlatformClipboard() = delete;
	};
}