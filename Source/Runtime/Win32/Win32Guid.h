#pragma once

#include <Runtime/Core/Definitions.h>
#include <Runtime/Core/StdFix.h>

namespace Hollow
{
	class RUNTIME_API Guid;

	class RUNTIME_API Win32Guid final
	{
	public:
		static void GenerateGuid(Guid& guidOut);

	public:
		Win32Guid() = delete;
		~Win32Guid() = delete;
	};
}