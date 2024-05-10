#pragma once

#include <Runtime/Core/Definitions.h>
#include <Runtime/Core/StdFix.h>

#include <Runtime/Memory/MemoryBuffer.h>

namespace Hollow
{
	class RUNTIME_API Win32File final
	{
	public:
		static bool Exists(const String& path);
		static bool Create(const String& path);
		static bool Delete(const String& path);
		static bool Write(const String& path, const String& data, const uint64 offset = 0);
		static bool Write(const String& path, const MemoryBuffer& buffer, const uint64 offset = 0);
		static bool Read(const String& path, String& contentOut, const uint64 startByte = 0, const uint64 endByte = 0);
		static bool Read(const String& path, MemoryBuffer& view, const uint64 startByte = 0, const uint64 endByte = 0);
		static bool Copy(const String& source, const String& destination);
		static bool Move(const String& source, const String& destination);
		static bool Rename(const String& source, const String& destination);
		static bool GetSize(const String& path, uint64& sizeOut);
		static bool GetName(const String& path, String& nameOut);
		static bool GetExtension(const String& path, String& extensionOut);
		static bool GetDirectory(const String& path, String& directoryOut);

	public:
		Win32File() = delete;
		~Win32File() = delete;
	};
}