#include "ShaderIncluder.h"

#include <Runtime/Platform/PlatformAPI.h>
#include <Runtime/Platform/PlatformFile.h>
#include <Runtime/Platform/PlatformDirectory.h>

namespace Hollow
{
	ShaderIncluder::ShaderIncluder()
	{
#if defined(HOLLOW_DEBUG)
		mIncludePaths.push_back(PlatformAPI::GetAPI()->GetEngineSourcePath() + "Source/Shaders/");
#endif
	}

	shaderc_include_result* ShaderIncluder::GetInclude(const char* requested_source, shaderc_include_type type, const char* requesting_source, u64 include_depth)
	{
		const char* includePath = GetActualPath(requested_source);
		const char* content = RecordInclude(includePath);

		shaderc_include_result* result = new shaderc_include_result();
		result->content = content;
		result->content_length = strlen(content);
		result->source_name = includePath;
		result->source_name_length = strlen(includePath);
		result->user_data = nullptr;

		return result;
	}

	void ShaderIncluder::ReleaseInclude(shaderc_include_result* data)
	{
		delete data;
	}

	String ShaderIncluder::ResolveInlcude(const String& requestedSource)
	{
		// Check if the file exists in the include paths
		for (const String& includePath : mIncludePaths)
		{
			String path = includePath + requestedSource;
			if (PlatformFile::Exists(path))
				return path;
		}

		CORE_ASSERT(false, "ShaderCompiler", "Failed to resolve include file");
		return "";
	}

	const char* ShaderIncluder::RecordInclude(const String& requestedSource)
	{
		String content;
		CORE_ASSERT(PlatformFile::Read(requestedSource, content), "ShaderCompiler", "Failed to read include file");

		char* result = new char[content.size() + 1];
		strcpy(result, content.c_str());

		return result;
	}

	const char* ShaderIncluder::GetActualPath(const String& requestedSource)
	{
		String includePath = ResolveInlcude(requestedSource);

		char* result = new char[includePath.size() + 1];
		strcpy(result, includePath.c_str());

		return result;
	}
}
