#pragma once

#include <Engine/Core/Definitions.h>
#include <Engine/Core/StdFix.h>

namespace Hollow
{
	class ENGINE_API Win32Clipboard final
	{
	public:
		static void SetClipboardText(const String& text);
		static String GetClipboardText();

	public:
		Win32Clipboard() = delete;
		~Win32Clipboard() = delete;
	};
}
