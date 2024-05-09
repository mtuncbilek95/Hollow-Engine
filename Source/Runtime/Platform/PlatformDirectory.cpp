#include "PlatformDirectory.h"

#if defined(HOLLOW_PLATFORM_WINDOWS)
#include <Runtime/Win32/Win32Directory.h>
typedef Hollow::Win32Directory PlatformDependency;
#endif

namespace Hollow
{
	bool PlatformDirectory::Exists(const string& path)
	{
		return PlatformDependency::Exists(path);
	}

	bool PlatformDirectory::Create(const string& path)
	{
		return PlatformDependency::Create(path);
	}

	bool PlatformDirectory::Delete(const string& path)
	{
		return PlatformDependency::Delete(path);
	}

	bool PlatformDirectory::Copy(const string& source, const string& destination)
	{
		return PlatformDependency::Copy(source, destination);
	}

	bool PlatformDirectory::Move(const string& source, const string& destination)
	{
		return PlatformDependency::Move(source, destination);
	}

	bool PlatformDirectory::Rename(const string& source, const string& destination)
	{
		return PlatformDependency::Rename(source, destination);
	}

	bool PlatformDirectory::GetFiles(const string& path, arrayList<string>& files)
	{
		return PlatformDependency::GetFiles(path, files);
	}

	bool PlatformDirectory::GetDirectories(const string& path, arrayList<string>& directories)
	{
		return PlatformDependency::GetDirectories(path, directories);
	}

	bool PlatformDirectory::GetFilesWithExtension(const string& path, const string& extension, arrayList<string>& files)
	{
		return PlatformDependency::GetFilesWithExtension(path, extension, files);
	}
}