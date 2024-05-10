#include "Win32Message.h"

#if defined(HOLLOW_PLATFORM_WINDOWS)
#include <Windows.h>
#endif

namespace Hollow
{
	void Win32Message::ShowMessage(const String& title, const String& message, const int flags)
	{
#if defined(HOLLOW_PLATFORM_WINDOWS)
		MessageBoxA(NULL, message.c_str(), title.c_str(), flags);
#else
		return;
#endif
	}
}
