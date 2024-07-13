#include "PlatformDirectory.h"

#if defined(HOLLOW_PLATFORM_WINDOWS)
#include <Engine/Win32/Win32Directory.h>
typedef Hollow::Win32Directory PlatformDependency;
#endif

namespace Hollow
{
	bool PlatformDirectory::Exists(const String& path)
	{
		return PlatformDependency::Exists(path);
	}

	bool PlatformDirectory::Create(const String& path)
	{
		return PlatformDependency::Create(path);
	}

	bool PlatformDirectory::Delete(const String& path)
	{
		return PlatformDependency::Delete(path);
	}

	bool PlatformDirectory::Copy(const String& source, const String& destination)
	{
		return PlatformDependency::Copy(source, destination);
	}

	bool PlatformDirectory::Move(const String& source, const String& destination)
	{
		return PlatformDependency::Move(source, destination);
	}

	bool PlatformDirectory::Rename(const String& source, const String& destination)
	{
		return PlatformDependency::Rename(source, destination);
	}

	bool PlatformDirectory::GetFiles(const String& path, DArray<String>& files)
	{
		return PlatformDependency::GetFiles(path, files);
	}

	bool PlatformDirectory::GetDirectories(const String& path, DArray<String>& directories)
	{
		return PlatformDependency::GetDirectories(path, directories);
	}

	bool PlatformDirectory::GetFilesWithExtension(const String& path, const String& extension, DArray<String>& files)
	{
		return PlatformDependency::GetFilesWithExtension(path, extension, files);
	}
}