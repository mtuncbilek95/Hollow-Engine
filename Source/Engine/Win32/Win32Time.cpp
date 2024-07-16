#include "Win32Time.h"

#if defined(HOLLOW_PLATFORM_WINDOWS)
#include <Windows.h>
#endif

namespace Hollow
{
	Time Win32Time::CurrentDateTime()
	{
		SYSTEMTIME pTime;
		GetSystemTime(&pTime);

		Time time;
		time.Year = pTime.wYear;
		time.Month = pTime.wMonth;
		time.Day = pTime.wDay;
		time.Hour = pTime.wHour;
		time.Minute = pTime.wMinute;
		time.Second = pTime.wSecond;
		time.Millisecond = pTime.wMilliseconds;

		return time;
	}
}
