#pragma once

#include <Engine/Core/Definitions.h>
#include <Engine/Core/StdFix.h>

namespace Hollow
{
	class Guid;

	class Win32Guid final
	{
	public:
		static void GenerateGuid(Guid& guidOut);

	public:
		Win32Guid() = delete;
		~Win32Guid() = delete;
	};
}