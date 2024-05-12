#include "Win32File.h"

#if defined(HOLLOW_PLATFORM_WINDOWS)
#include <Windows.h>
#include <stdio.h>
#endif

namespace Hollow
{
	bool Win32File::Exists(const String& path)
	{
#if defined(HOLLOW_PLATFORM_WINDOWS)
		DWORD dwAttrib = GetFileAttributesA(path.c_str());

		// If the file is found return true
		return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
#else
		return false;
#endif 
	}

	bool Win32File::Create(const String& path)
	{
#if defined(HOLLOW_PLATFORM_WINDOWS)
		// Create the file
		HANDLE hFile = CreateFileA(path.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		// Check if the file was created
		if (hFile == INVALID_HANDLE_VALUE || hFile == NULL)
			return false;

		// Close the file
		CloseHandle(hFile);
		return true;
#else
		return false;
#endif 
	}

	bool Win32File::Delete(const String& path)
	{
#if defined(HOLLOW_PLATFORM_WINDOWS)
		// Delete the file
		return DeleteFileA(path.c_str()) != 0;
#else
		return false;
#endif
	}

	bool Win32File::Write(const String& path, const String& data, const uint64 offset)
	{
#if defined(HOLLOW_PLATFORM_WINDOWS)
		HANDLE hFile;

		// Check if the file exists
		if(Exists(path))
			hFile = CreateFileA(path.c_str(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		else
			hFile = CreateFileA(path.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		// Check if the file was created
		if (hFile == INVALID_HANDLE_VALUE || hFile == NULL)
			return false;

		// Set the file pointer to the offset
		SetFilePointer(hFile, offset, NULL, FILE_BEGIN);

		// Write the data to the file
		DWORD bytesWritten;
		const bool result = WriteFile(hFile, data.c_str(), data.size(), &bytesWritten, NULL);

		// Close the file
		CloseHandle(hFile);
		return result;
#else
		return false;
#endif
	}

	bool Win32File::Write(const String& path, const MemoryBuffer& buffer, const uint64 offset)
	{
#if defined(HOLLOW_PLATFORM_WINDOWS)
		HANDLE hFile;

		// Check if the file exists
		if (Exists(path))
			hFile = CreateFileA(path.c_str(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		else
			hFile = CreateFileA(path.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		// Check if the file was created
		if (hFile == INVALID_HANDLE_VALUE || hFile == NULL)
			return false;

		// Set the file pointer to the offset
		SetFilePointer(hFile, offset, NULL, FILE_BEGIN);

		// Write the data to the file
		DWORD bytesWritten;
		const bool result = WriteFile(hFile, buffer.GetBuffer(), buffer.GetSize(), &bytesWritten, NULL);

		// Close the file
		CloseHandle(hFile);
		return result;
#else
		return false;
#endif
	}

	bool Win32File::Read(const String& path, String& contentOut, const uint64 startByte, const uint64 endByte)
	{
#if defined(HOLLOW_PLATFORM_WINDOWS)
		HANDLE hFile;

		// Check if the file exists
		if (!Exists(path))
			return false;

		// Open the file
		hFile = CreateFileA(path.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		// Check if the file was opened
		if (hFile == INVALID_HANDLE_VALUE || hFile == NULL)
			return false;

		// Get the file size
		uint64 fileSize = GetFileSize(hFile, NULL);

		// Set the file pointer to the offset
		SetFilePointer(hFile, startByte, NULL, FILE_BEGIN);

		// Calculate the expected read range
		uint64 acceptedRange = endByte == 0 ? GetFileSize(hFile, NULL) : endByte - startByte;

		// Check if the file is empty or the range is invalid
		if (fileSize == 0 || acceptedRange > fileSize)
			return false;

		// Read the file
		DWORD bytesRead;
		char* buffer = new char[acceptedRange + 1];
		const bool result = ReadFile(hFile, buffer, acceptedRange, &bytesRead, NULL);

		// Check if the file was read
		if (!result)
		{
			delete[] buffer;
			CloseHandle(hFile);
			contentOut = "";
			return false;
		}

		// Close the file
		CloseHandle(hFile);

		// Copy the buffer to the content
		contentOut = String(buffer, bytesRead);

		// Free the buffer
		delete[] buffer;
		return true;
#else
		return false;
#endif
	}

	bool Win32File::Read(const String& path, MemoryBuffer& view, const uint64 startByte, const uint64 endByte)
	{
#if defined(HOLLOW_PLATFORM_WINDOWS)
		HANDLE hFile;

		// Check if the file exists
		if (!Exists(path))
			return false;

		// Open the file
		hFile = CreateFileA(path.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		// Check if the file was opened
		if (hFile == INVALID_HANDLE_VALUE || hFile == NULL)
			return false;

		// Get the file size
		uint64 fileSize = GetFileSize(hFile, NULL);

		// Set the file pointer to the offset
		SetFilePointer(hFile, startByte, NULL, FILE_BEGIN);

		// Calculate the expected read range
		uint64 acceptedRange = endByte == 0 ? GetFileSize(hFile, NULL) : endByte - startByte;

		// Check if the file is empty or the range is invalid
		if (fileSize == 0 || acceptedRange > fileSize)
			return false;

		// Read the file
		DWORD bytesRead;
		char* buffer = new char[acceptedRange + 1];
		const bool result = ReadFile(hFile, buffer, acceptedRange, &bytesRead, NULL);
		buffer[acceptedRange] = '\0';

		// Check if the file was read
		if (!result)
		{
			delete[] buffer;
			CloseHandle(hFile);
			view = MemoryBuffer();
			return false;
		}

		// Close the file
		CloseHandle(hFile);

		// Copy the buffer to the content
		view = MemoryBuffer(buffer, bytesRead);
		return true;
#else
		return false;
#endif
	}

	bool Win32File::Copy(const String& source, const String& destination)
	{
#if defined(HOLLOW_PLATFORM_WINDOWS)
		// Check if the source file exists
		if (!Exists(source))
			return false;

		// Copy the file
		return CopyFileA(source.c_str(), destination.c_str(), TRUE) != 0;
#else
		return false;
#endif
	}

	bool Win32File::Move(const String& source, const String& destination)
	{
#if defined(HOLLOW_PLATFORM_WINDOWS)
		// Check if the source file exists
		if (!Exists(source))
			return false;

		// Move the file
		return MoveFileA(source.c_str(), destination.c_str()) != 0;
#else
		return false;
#endif
	}

	bool Win32File::Rename(const String& source, const String& destination)
	{
#if defined(HOLLOW_PLATFORM_WINDOWS)
		// Check if the source file exists
		if (!Exists(source))
			return false;

		// Rename the file
		return MoveFileA(source.c_str(), destination.c_str()) != 0;
#else
		return false;
#endif
	}

	bool Win32File::GetSize(const String& path, uint64& sizeOut)
	{
#if defined(HOLLOW_PLATFORM_WINDOWS)
		HANDLE hFile;

		// Check if the file exists
		if (!Exists(path))
			return false;

		// Open the file
		hFile = CreateFileA(path.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		// Check if the file was opened
		if (hFile == INVALID_HANDLE_VALUE || hFile == NULL)
			return false;

		// Get the file size
		sizeOut = GetFileSize(hFile, NULL);

		// Close the file
		CloseHandle(hFile);
		return true;
#else
		return false;
#endif
	}

	bool Win32File::GetName(const String& path, String& nameOut)
	{
#if defined(HOLLOW_PLATFORM_WINDOWS)

		// Get the name of the file
		String name = path;
		// find the last slash as '/' or '\'
		size_t lastSlash = name.find_last_of("/\\"); 
		if (lastSlash != String::npos)
			name = name.substr(lastSlash + 1);

		nameOut = name;
		return true;
#else
		return false;
#endif
	}

	bool Win32File::GetExtension(const String& path, String& extensionOut)
	{
#if defined(HOLLOW_PLATFORM_WINDOWS)
		// Get the extension of the file
		String extension = path;
		size_t lastDot = extension.find_last_of(".");
		if (lastDot != String::npos)
			extension = extension.substr(lastDot + 1);

		extensionOut = extension;
		return true;
#else
		return false;
#endif
	}

	bool Win32File::GetDirectory(const String& path, String& directoryOut)
	{
#if defined(HOLLOW_PLATFORM_WINDOWS)
		// Get the directory of the file
		String directory = path;
		size_t lastSlash = directory.find_last_of("/\\");
		if (lastSlash != String::npos)
			directory = directory.substr(0, lastSlash);

		directoryOut = directory;
		return true;
#else
		return false;
#endif
	}
}
