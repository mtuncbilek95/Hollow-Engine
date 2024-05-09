#pragma once

#include <Runtime/Core/Definitions.h>
#include <Runtime/Core/StdFix.h>

#include <Runtime/Memory/MemoryBuffer.h>

namespace Hollow
{
	class RUNTIME_API Win32File final
	{
	public:
		static bool Exists(const string& path);
		static bool Create(const string& path);
		static bool Delete(const string& path);
		static bool Write(const string& path, const string& data, const uint64 offset = 0);
		static bool Write(const string& path, const MemoryBuffer& buffer, const uint64 offset = 0);
		static bool Read(const string& path, string& contentOut, const uint64 startByte = 0, const uint64 endByte = 0);
		static bool Read(const string& path, MemoryBuffer& view, const uint64 startByte = 0, const uint64 endByte = 0);
		static bool Copy(const string& source, const string& destination);
		static bool Move(const string& source, const string& destination);
		static bool Rename(const string& source, const string& destination);
		static bool GetSize(const string& path, uint64& sizeOut);
		static bool GetName(const string& path, string& nameOut);
		static bool GetExtension(const string& path, string& extensionOut);
		static bool GetDirectory(const string& path, string& directoryOut);

	public:
		Win32File() = delete;
		~Win32File() = delete;
	};
}