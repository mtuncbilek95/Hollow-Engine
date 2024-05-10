#pragma once

#include <Runtime/Core/Definitions.h>
#include <Runtime/Core/StdFix.h>

#include <Runtime/Memory/MemoryBuffer.h>

namespace Hollow
{
	class RUNTIME_API Win32Directory final
	{
	public:
		static bool Exists(const String& path);
		static bool Create(const String& path);
		static bool Delete(const String& path);
		static bool Copy(const String& source, const String& destination);
		static bool Move(const String& source, const String& destination);
		static bool Rename(const String& source, const String& destination);
		static bool GetFiles(const String& path, ArrayList<String>& files);
		static bool GetDirectories(const String& path, ArrayList<String>& directories);
		static bool GetFilesWithExtension(const String& path, const String& extension, ArrayList<String>& files);

	public:
		Win32Directory() = delete;
		~Win32Directory() = delete;
	};
}
