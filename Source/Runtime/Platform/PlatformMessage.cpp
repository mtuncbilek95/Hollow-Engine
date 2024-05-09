#include "PlatformMessage.h"

#if defined(HOLLOW_PLATFORM_WINDOWS)
#include <Runtime/Win32/Win32Message.h>
typedef Hollow::Win32Message PlatformDependency;
#endif

namespace Hollow
{
	void PlatformMessage::ShowMessage(const string& title, const string& message, const int flags)
	{
		PlatformDependency::ShowMessage(title, message, flags);
	}
}
