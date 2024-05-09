#pragma once

#include <Runtime/Core/Definitions.h>
#include <Runtime/Core/StdFix.h>

namespace Hollow
{
	class RUNTIME_API Win32Message final
	{
	public:
		static void ShowMessage(const string& title, const string& message,const int flags);
	};
}
