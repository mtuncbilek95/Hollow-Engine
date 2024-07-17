#pragma once

#include <Engine/Core/Core.h>
#include <shaderc/shaderc.hpp>

namespace Hollow
{
	class ENGINE_API ShaderIncluder : public shaderc::CompileOptions::IncluderInterface
	{
		struct ENGINE_API IncludeData
		{
			shaderc_include_result result = {};
			String FullPath;
			String Content;
		};

	public:
		ShaderIncluder() = default;
		~ShaderIncluder() override = default;

		shaderc_include_result* GetInclude(const char* requestedSource, shaderc_include_type type, const char* requestingSource, s64 includeDepth) override;
		void ReleaseInclude(shaderc_include_result* data) override;

	private:
		String ResolveInclude(const String& includePath);

	private:
		String mIncludePath;
	};
}
