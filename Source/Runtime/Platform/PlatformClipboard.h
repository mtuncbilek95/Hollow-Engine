#pragma once

#include <Runtime/Core/Definitions.h>
#include <Runtime/Core/StdFix.h>

namespace Hollow
{
	class RUNTIME_API PlatformClipboard final
	{
	public:
		static void SetClipboardText(const string& text);
		static string GetClipboardText();

	public:
		PlatformClipboard() = delete;
		~PlatformClipboard() = delete;
	};
}