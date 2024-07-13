#pragma once

#include <Engine/Core/Definitions.h>
#include <Engine/Core/StdFix.h>

namespace Hollow
{
	class ENGINE_API Win32Message final
	{
	public:
		static void ShowMessage(const String& title, const String& message,const i32 flags);
	};
}
