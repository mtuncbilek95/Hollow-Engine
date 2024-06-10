#include "PlatformFile.h"

#if defined(HOLLOW_PLATFORM_WINDOWS)
#include <Runtime/Win32/Win32File.h>
typedef Hollow::Win32File PlatformDependency;
#endif

namespace Hollow
{
	bool PlatformFile::Exists(const String& path)
	{
		return PlatformDependency::Exists(path);
	}

	bool PlatformFile::Create(const String& path)
	{
		return PlatformDependency::Create(path);
	}

	bool PlatformFile::Delete(const String& path)
	{
		return PlatformDependency::Delete(path);
	}

	bool PlatformFile::Write(const String& path, const String& data, const u64 offset)
	{
		return PlatformDependency::Write(path, data, offset);
	}

	bool PlatformFile::Write(const String& path, const MemoryBuffer& buffer, const u64 offset)
	{
		return PlatformDependency::Write(path, buffer, offset);
	}

	bool PlatformFile::Read(const String& path, String& contentOut, const u64 startByte, const u64 endByte)
	{
		return PlatformDependency::Read(path, contentOut, startByte, endByte);
	}

	bool PlatformFile::Read(const String& path, MemoryBuffer& view, const u64 startByte, const u64 endByte)
	{
		return PlatformDependency::Read(path, view, startByte, endByte);
	}

	bool PlatformFile::Copy(const String& source, const String& destination)
	{
		return PlatformDependency::Copy(source, destination);
	}

	bool PlatformFile::Move(const String& source, const String& destination)
	{
		return PlatformDependency::Move(source, destination);
	}

	bool PlatformFile::Rename(const String& source, const String& destination)
	{
		return PlatformDependency::Rename(source, destination);
	}

	bool PlatformFile::GetSize(const String& path, u64& sizeOut)
	{
		return PlatformDependency::GetSize(path, sizeOut);
	}

	bool PlatformFile::GetName(const String& path, String& nameOut)
	{
		return PlatformDependency::GetName(path, nameOut);
	}

	bool PlatformFile::GetExtension(const String& path, String& extensionOut)
	{
		return PlatformDependency::GetExtension(path, extensionOut);
	}

	bool PlatformFile::GetDirectory(const String& path, String& directoryOut)
	{
		return PlatformDependency::GetDirectory(path, directoryOut);
	}
}
