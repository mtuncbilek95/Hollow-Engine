#pragma once

#include <Engine/Core/Definitions.h>
#include <Engine/Core/StdFix.h>

namespace Hollow
{
	class ENGINE_API Guid;

	class ENGINE_API Win32Guid final
	{
	public:
		static void GenerateGuid(Guid& guidOut);

	public:
		Win32Guid() = delete;
		~Win32Guid() = delete;
	};
}