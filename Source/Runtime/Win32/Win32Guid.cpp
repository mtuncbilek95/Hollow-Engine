#include "Win32Guid.h"

#if defined(HOLLOW_PLATFORM_WINDOWS)
#include <Windows.h>
#include <Runtime/Core/Guid.h>
#endif

namespace Hollow
{
	void Win32Guid::GenerateGuid(Guid& guidOut)
	{
#if defined(HOLLOW_PLATFORM_WINDOWS)
		HRESULT result = CoCreateGuid((GUID*)&guidOut.GetA());
#else
		return;
#endif
	}
}
