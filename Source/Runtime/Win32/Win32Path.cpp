#include "Win32Path.h"

#if defined (HOLLOW_PLATFORM_WINDOWS)
#include <Windows.h>
#include <ShlObj.h>
#pragma comment(lib, "Shell32.lib")
#endif

namespace Hollow
{
	String Win32Path::GetExecutableDir()
	{
#if defined(HOLLOW_PLATFORM_WINDOWS)
		char buffer[MAX_PATH];
		GetModuleFileNameA(NULL, buffer, MAX_PATH);
		return String(buffer);
#else
		return String();
#endif
	}

	String Win32Path::GetProgramFilesDir()
	{
#if defined(HOLLOW_PLATFORM_WINDOWS)
		char buffer[MAX_PATH];
		SHGetFolderPathA(NULL, CSIDL_PROGRAM_FILES, NULL, 0, buffer);
		return String(buffer);
#else
		return String();
#endif
	}

	String Win32Path::GetAppDataDir()
	{
#if defined(HOLLOW_PLATFORM_WINDOWS)
		char buffer[MAX_PATH];
		SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, buffer);
		return String(buffer);
#else
		return String();
#endif
	}
	String Win32Path::GetTempDir()
	{
#if defined(HOLLOW_PLATFORM_WINDOWS)
		char buffer[MAX_PATH];
		GetTempPathA(MAX_PATH, buffer);
		return String(buffer);
#else
		return String();
#endif
	}
}
