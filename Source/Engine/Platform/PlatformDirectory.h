#pragma once

#include <Engine/Core/Definitions.h>
#include <Engine/Core/StdFix.h>

#include <Engine/Memory/MemoryBuffer.h>

namespace Hollow
{
	class PlatformDirectory final
	{
	public:
		static bool Exists(const String& path);
		static bool Create(const String& path);
		static bool Delete(const String& path);
		static bool Copy(const String& source, const String& destination);
		static bool Move(const String& source, const String& destination);
		static bool Rename(const String& source, const String& destination);
		static bool GetFiles(const String& path, DArray<String>& files);
		static bool GetDirectories(const String& path, DArray<String>& directories);
		static bool GetFilesWithExtension(const String& path, const String& extension, DArray<String>& files);

	public:
		PlatformDirectory() = delete;
		~PlatformDirectory() = delete;
	};
}