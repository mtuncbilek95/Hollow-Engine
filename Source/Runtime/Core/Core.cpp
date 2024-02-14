#include "Core.h"

#include <stdio.h>
#include <stdarg.h>
#include <cassert>
#include <stdlib.h>

namespace Hollow
{
	void RUNTIME_API DevLog(LogLevel type, const char* pMessage, ...)
	{
		va_list args;
		va_start(args, pMessage);

		switch (type)
		{
		case LogLevel::Info:
			printf("\033[36m[INFO]\033[39m -- ");
			break;
		case LogLevel::Verbose:
			printf("\033[32m[VERBOSE]\033[39m -- ");
			break;
		case LogLevel::Warning:
			printf("\033[33m[WARNING]\033[39m -- ");
			break;
		case LogLevel::Error:
			printf("\033[31m[ERROR]\033[39m -- ");
			break;
		default:
			break;
		}

		vprintf(pMessage, args);
		printf("\n");
		va_end(args);

		if (type == LogLevel::Error)
		{
			printf("\033[33m[SYSTEM]\033[39m -- System is closing due to error!");
			abort();
		}
	}

	void RUNTIME_API DevAssert(bool condition, const char* pTitle, const char* pFailed, ...)
	{
		if (!condition)
		{
			va_list args;
			va_start(args, pFailed);
			printf("\033[31m[%s]\033[39m -- ", pTitle);
			vprintf(pFailed, args);
			printf("\n");
			va_end(args);

			abort();
		}
	}
}
