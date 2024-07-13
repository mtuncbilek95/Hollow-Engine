#include "PlatformMessage.h"

#if defined(HOLLOW_PLATFORM_WINDOWS)
#include <Engine/Win32/Win32Message.h>
typedef Hollow::Win32Message PlatformDependency;
#endif

namespace Hollow
{
	void PlatformMessage::ShowMessage(const String& title, const String& message, const i32 flags)
	{
		PlatformDependency::ShowMessage(title, message, flags);
	}
}
