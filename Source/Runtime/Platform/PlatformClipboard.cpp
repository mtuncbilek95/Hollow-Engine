#include "PlatformClipboard.h"

#if defined(HOLLOW_PLATFORM_WINDOWS)
#include <Runtime/Win32/Win32Clipboard.h>
typedef Hollow::Win32Clipboard PlatformDependency;
#endif

namespace Hollow
{
	void PlatformClipboard::SetClipboardText(const string& text)
	{
		PlatformDependency::SetClipboardText(text);
	}

	string PlatformClipboard::GetClipboardText()
	{
		return PlatformDependency::GetClipboardText();
	}
}
