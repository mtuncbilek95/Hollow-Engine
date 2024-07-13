#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Shader/ShaderLanguage.h>
#include <Runtime/Graphics/Shader/ShaderStage.h>
#include <Runtime/Graphics/Texture/TextureFormat.h>

#include <shaderc/shaderc.h>
#include <spirv_reflect.h>

namespace Hollow
{
	class RUNTIME_API ShaderCompilerUtils
	{
	public:
		static shaderc_shader_kind GetShaderKind(const ShaderStage stage)
		{
			switch (stage)
			{
			case ShaderStage::Vertex:			return shaderc_vertex_shader;
			case ShaderStage::Fragment:			return shaderc_fragment_shader;
			case ShaderStage::Compute:			return shaderc_compute_shader;
			case ShaderStage::Geometry:			return shaderc_geometry_shader;
			default:							return shaderc_glsl_infer_from_source;
			}
		}

		static shaderc_source_language GetLanguage(const ShaderLanguage langauge)
		{
			switch (langauge)
			{
			case ShaderLanguage::GLSL:			return shaderc_source_language_glsl;
			case ShaderLanguage::HLSL:			return shaderc_source_language_hlsl;
			default:							return shaderc_source_language_glsl;
			}
		}

	public:
		ShaderCompilerUtils() = delete;
		~ShaderCompilerUtils() = delete;
	};
}