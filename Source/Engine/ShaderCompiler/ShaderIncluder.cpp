#include "ShaderIncluder.h"

#include <Engine/Platform/PlatformFile.h>

#include <stdexcept>

namespace Hollow
{
	shaderc_include_result* ShaderIncluder::GetInclude(const char* requestedSource, shaderc_include_type type, const char* requestingSource, s64 includeDepth) 
	{
		IncludeData* data = new IncludeData();
		data->FullPath = ResolveInclude(requestedSource);

		if(!PlatformFile::Read(data->FullPath, data->Content))
			throw std::runtime_error("Failed to read include file: " + data->FullPath);

		shaderc_include_result* result = &data->result;
		result->source_name = data->FullPath.c_str();
		result->source_name_length = data->FullPath.size();
		result->content = data->Content.c_str();
		result->content_length = data->Content.size();
		result->user_data = data;

		return result;
	}

	void ShaderIncluder::ReleaseInclude(shaderc_include_result* data)
	{
		delete static_cast<IncludeData*>(data->user_data);
	}

	String ShaderIncluder::ResolveInclude(const String& includePath)
	{
		std::string path = mIncludePath + includePath;
		if (!PlatformFile::Exists(path))
			throw std::runtime_error("Include file not found: " + path);
		
		return path;
	}
}