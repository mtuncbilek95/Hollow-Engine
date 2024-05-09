#pragma once

#include <Runtime/Core/Definitions.h>
#include <Runtime/Core/StdFix.h>

namespace Hollow
{
	class RUNTIME_API Win32Clipboard final
	{
	public:
		static void SetClipboardText(const string& text);
		static string GetClipboardText();

	public:
		Win32Clipboard() = delete;
		~Win32Clipboard() = delete;
	};
}
