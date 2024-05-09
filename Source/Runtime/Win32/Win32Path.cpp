#include "Win32Path.h"

#if defined (HOLLOW_PLATFORM_WINDOWS)
#include <Windows.h>
#include <ShlObj.h>
#pragma comment(lib, "Shell32.lib")
#endif

namespace Hollow
{
	string Win32Path::GetExecutableDir()
	{
#if defined(HOLLOW_PLATFORM_WINDOWS)
		char buffer[MAX_PATH];
		GetModuleFileNameA(NULL, buffer, MAX_PATH);
		return string(buffer);
#else
		return string();
#endif
	}

	string Win32Path::GetProgramFilesDir()
	{
#if defined(HOLLOW_PLATFORM_WINDOWS)
		char buffer[MAX_PATH];
		SHGetFolderPathA(NULL, CSIDL_PROGRAM_FILES, NULL, 0, buffer);
		return string(buffer);
#else
		return string();
#endif
	}

	string Win32Path::GetAppDataDir()
	{
#if defined(HOLLOW_PLATFORM_WINDOWS)
		char buffer[MAX_PATH];
		SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, buffer);
		return string(buffer);
#else
		return string();
#endif
	}
	string Win32Path::GetTempDir()
	{
#if defined(HOLLOW_PLATFORM_WINDOWS)
		char buffer[MAX_PATH];
		GetTempPathA(MAX_PATH, buffer);
		return string(buffer);
#else
		return string();
#endif
	}
}
