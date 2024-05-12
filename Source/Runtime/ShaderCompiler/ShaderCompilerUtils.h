#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Shader/ShaderLanguage.h>
#include <Runtime/Graphics/Shader/ShaderStage.h>
#include <Runtime/Graphics/Texture/TextureFormat.h>
#include <Runtime/ShaderCompiler/ShaderDescriptorBindingType.h>
#include <Runtime/ShaderCompiler/ShaderIOSemantic.h>
#include <Runtime/ShaderCompiler/ShaderVariableType.h>

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

		static ShaderDescriptorBindingType GetSpvDescriptorType(const SpvReflectDescriptorType type)
		{
			switch (type)
			{
			case SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLER:					return ShaderDescriptorBindingType::Sampler;
			case SPV_REFLECT_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:	return ShaderDescriptorBindingType::CombinedImageSampler;
			case SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLED_IMAGE:				return ShaderDescriptorBindingType::SampledImage;
			case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_IMAGE:				return ShaderDescriptorBindingType::StorageImage;
			case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:		return ShaderDescriptorBindingType::UniformTexelBuffer;
			case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:		return ShaderDescriptorBindingType::StorageTexelBuffer;
			case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER:			return ShaderDescriptorBindingType::UniformBuffer;
			case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER:			return ShaderDescriptorBindingType::StorageBuffer;
			case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:	return ShaderDescriptorBindingType::UniformBufferDynamic;
			case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:	return ShaderDescriptorBindingType::StorageBufferDynamic;
			case SPV_REFLECT_DESCRIPTOR_TYPE_INPUT_ATTACHMENT:			return ShaderDescriptorBindingType::InputAttachment;
			default:													return ShaderDescriptorBindingType::Sampler;
			}
		}

		static TextureFormat GetSpvFormat(const SpvReflectFormat format)
		{
			switch (format)
			{
			case SPV_REFLECT_FORMAT_UNDEFINED:					return TextureFormat::Unknown;
			case SPV_REFLECT_FORMAT_R16_UINT:					return TextureFormat::R16_UInt;
			case SPV_REFLECT_FORMAT_R16_SINT:					return TextureFormat::R16_Int;
			case SPV_REFLECT_FORMAT_R16_SFLOAT:					return TextureFormat::R16_Float;
			case SPV_REFLECT_FORMAT_R16G16_UINT:				return TextureFormat::RG16_UInt;
			case SPV_REFLECT_FORMAT_R16G16_SINT:				return TextureFormat::RG16_Int;
			case SPV_REFLECT_FORMAT_R16G16_SFLOAT:				return TextureFormat::RG16_Float;
			case SPV_REFLECT_FORMAT_R16G16B16_UINT:				return TextureFormat::RGBA16_UInt;
			case SPV_REFLECT_FORMAT_R16G16B16_SINT:				return TextureFormat::RGBA16_Int;
			case SPV_REFLECT_FORMAT_R16G16B16_SFLOAT:			return TextureFormat::Unknown;
			case SPV_REFLECT_FORMAT_R16G16B16A16_UINT:			return TextureFormat::Unknown;
			case SPV_REFLECT_FORMAT_R16G16B16A16_SINT:			return TextureFormat::Unknown;
			case SPV_REFLECT_FORMAT_R16G16B16A16_SFLOAT:		return TextureFormat::Unknown;
			case SPV_REFLECT_FORMAT_R32_UINT:					return TextureFormat::Unknown;
			case SPV_REFLECT_FORMAT_R32_SINT:					return TextureFormat::Unknown;
			case SPV_REFLECT_FORMAT_R32_SFLOAT:					return TextureFormat::Unknown;
			case SPV_REFLECT_FORMAT_R32G32_UINT:				return TextureFormat::Unknown;
			case SPV_REFLECT_FORMAT_R32G32_SINT:				return TextureFormat::Unknown;
			case SPV_REFLECT_FORMAT_R32G32_SFLOAT:				return TextureFormat::Unknown;
			case SPV_REFLECT_FORMAT_R32G32B32_UINT:				return TextureFormat::Unknown;
			case SPV_REFLECT_FORMAT_R32G32B32_SINT:				return TextureFormat::Unknown;
			case SPV_REFLECT_FORMAT_R32G32B32_SFLOAT:			return TextureFormat::Unknown;
			case SPV_REFLECT_FORMAT_R32G32B32A32_UINT:			return TextureFormat::Unknown;
			case SPV_REFLECT_FORMAT_R32G32B32A32_SINT:			return TextureFormat::Unknown;
			case SPV_REFLECT_FORMAT_R32G32B32A32_SFLOAT:		return TextureFormat::Unknown;
			case SPV_REFLECT_FORMAT_R64_UINT:					return TextureFormat::Unknown;
			case SPV_REFLECT_FORMAT_R64_SINT:					return TextureFormat::Unknown;
			case SPV_REFLECT_FORMAT_R64_SFLOAT:					return TextureFormat::Unknown;
			case SPV_REFLECT_FORMAT_R64G64_UINT:				return TextureFormat::Unknown;
			case SPV_REFLECT_FORMAT_R64G64_SINT:				return TextureFormat::Unknown;
			case SPV_REFLECT_FORMAT_R64G64_SFLOAT:				return TextureFormat::Unknown;
			case SPV_REFLECT_FORMAT_R64G64B64_UINT:				return TextureFormat::Unknown;
			case SPV_REFLECT_FORMAT_R64G64B64_SINT:				return TextureFormat::Unknown;
			case SPV_REFLECT_FORMAT_R64G64B64_SFLOAT:			return TextureFormat::Unknown;
			case SPV_REFLECT_FORMAT_R64G64B64A64_UINT:			return TextureFormat::Unknown;
			case SPV_REFLECT_FORMAT_R64G64B64A64_SINT:			return TextureFormat::Unknown;
			case SPV_REFLECT_FORMAT_R64G64B64A64_SFLOAT:		return TextureFormat::Unknown;
			default:											return TextureFormat::Unknown;
			}
		}
	public:
		ShaderCompilerUtils() = delete;
		~ShaderCompilerUtils() = delete;
	};
}