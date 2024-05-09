#include "PlatformFile.h"

#if defined(HOLLOW_PLATFORM_WINDOWS)
#include <Runtime/Win32/Win32File.h>
typedef Hollow::Win32File PlatformDependency;
#endif

namespace Hollow
{
	bool PlatformFile::Exists(const string& path)
	{
		return PlatformDependency::Exists(path);
	}

	bool PlatformFile::Create(const string& path)
	{
		return PlatformDependency::Create(path);
	}

	bool PlatformFile::Delete(const string& path)
	{
		return PlatformDependency::Delete(path);
	}

	bool PlatformFile::Write(const string& path, const string& data, const uint64 offset)
	{
		return PlatformDependency::Write(path, data, offset);
	}

	bool PlatformFile::Write(const string& path, const MemoryBuffer& buffer, const uint64 offset)
	{
		return PlatformDependency::Write(path, buffer, offset);
	}

	bool PlatformFile::Read(const string& path, string& contentOut, const uint64 startByte, const uint64 endByte)
	{
		return PlatformDependency::Read(path, contentOut, startByte, endByte);
	}

	bool PlatformFile::Read(const string& path, MemoryBuffer& view, const uint64 startByte, const uint64 endByte)
	{
		return PlatformDependency::Read(path, view, startByte, endByte);
	}

	bool PlatformFile::Copy(const string& source, const string& destination)
	{
		return PlatformDependency::Copy(source, destination);
	}

	bool PlatformFile::Move(const string& source, const string& destination)
	{
		return PlatformDependency::Move(source, destination);
	}

	bool PlatformFile::Rename(const string& source, const string& destination)
	{
		return PlatformDependency::Rename(source, destination);
	}

	bool PlatformFile::GetSize(const string& path, uint64& sizeOut)
	{
		return PlatformDependency::GetSize(path, sizeOut);
	}

	bool PlatformFile::GetName(const string& path, string& nameOut)
	{
		return PlatformDependency::GetName(path, nameOut);
	}

	bool PlatformFile::GetExtension(const string& path, string& extensionOut)
	{
		return PlatformDependency::GetExtension(path, extensionOut);
	}

	bool PlatformFile::GetDirectory(const string& path, string& directoryOut)
	{
		return PlatformDependency::GetDirectory(path, directoryOut);
	}
}
