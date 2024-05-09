#pragma once

#include <Runtime/Core/Definitions.h>
#include <Runtime/Core/StdFix.h>

#include <Runtime/Memory/MemoryBuffer.h>

namespace Hollow
{
	class RUNTIME_API Win32Directory final
	{
	public:
		static bool Exists(const string& path);
		static bool Create(const string& path);
		static bool Delete(const string& path);
		static bool Copy(const string& source, const string& destination);
		static bool Move(const string& source, const string& destination);
		static bool Rename(const string& source, const string& destination);
		static bool GetFiles(const string& path, arrayList<string>& files);
		static bool GetDirectories(const string& path, arrayList<string>& directories);
		static bool GetFilesWithExtension(const string& path, const string& extension, arrayList<string>& files);

	public:
		Win32Directory() = delete;
		~Win32Directory() = delete;
	};
}
