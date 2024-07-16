#pragma once

#include "Definitions.h"
#include "StdFix.h"

namespace Hollow
{
	enum class ConsoleType
	{
		Info,
		Verbose,
		Warning,
		Error
	};

	class ENGINE_API GlobalConsole final
	{
	public:
		static void Print(const char* message, ...);
		static void Log(ConsoleType type, const char* message, ...);
		static void Assert(bool condition, const char* title, const char* message, ...);

	public:
		GlobalConsole() = delete;
		~GlobalConsole() = delete;
	};
}