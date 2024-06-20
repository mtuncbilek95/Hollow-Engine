#pragma once

#include <Runtime/Core/Core.h>

#include <spirv_cross/spirv_hlsl.hpp>
#include <spirv_cross/spirv_glsl.hpp>
#include <shaderc/shaderc.hpp>

namespace Hollow
{
	class RUNTIME_API ShaderIncluder : public shaderc::CompileOptions::IncluderInterface
	{
	public:
		ShaderIncluder();
		~ShaderIncluder() override = default;

		shaderc_include_result* GetInclude(const char* requestedSource, shaderc_include_type type, const char* requestingSource, u64 includeDepth) override;
		void ReleaseInclude(shaderc_include_result* data) override;

	private:
		String ResolveInlcude(const String& requestedSource);
		const char* RecordInclude(const String& requestedSource);
		const char* GetActualPath(const String& requestedSource);

	private:
		DArray<String> mIncludePaths;
	};
}
