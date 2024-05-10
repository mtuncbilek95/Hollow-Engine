#include "GlobalConsole.h"

#include <cstdarg>
#include <cassert>

#include <Runtime/Platform/PlatformMessage.h>

namespace Hollow
{
	void GlobalConsole::Print(const char* message, ...)
	{
		va_list args;
		va_start(args, message);
		vprintf(message, args);
		va_end(args);
	}

	void GlobalConsole::PrintError(const char* message, ...)
	{
		va_list args;
		va_start(args, message);
		printf("\033[31m[VulkanLayer]\033[39m -- ");
		vprintf(message, args);
		printf("\n");
		va_end(args);
	}

	void GlobalConsole::Log(ConsoleType type, const char* title, const char* message, ...)
	{
		va_list args;
		va_start(args, message);

		switch (type)
		{
		case ConsoleType::Info:
			printf("\033[36m[%s]\033[39m -- ", title);
			break;
		case ConsoleType::Verbose:
			printf("\033[32m[%s]\033[39m -- ", title);
			break;
		case ConsoleType::Warning:
			printf("\033[33m[%s]\033[39m -- ", title);
			break;
		case ConsoleType::Error:
			printf("\033[31m[%s]\033[39m -- ", title);
			break;
		}

		vprintf(message, args);
		printf("\n");
		va_end(args);
	}

	void GlobalConsole::Assert(bool condition, const char* title, const char* message, ...)
	{
		if (!condition)
		{
			va_list args;
			va_start(args, message);
			printf("\033[31m[%s]\033[39m -- ", title);
			vprintf(message, args);

			PlatformMessage::ShowMessage("Assertion Failed", message, Mb_OkCancel | Mb_IconError);

			printf("\n");
			va_end(args);

			exit(-1);
		}
	}
}
