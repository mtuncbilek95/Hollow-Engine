#pragma once

#include <Core/Core.h>
#include <Renderer/Renderer.h>

#include <vulkan.h>

namespace MiniVk
{
	enum class ShaderLanguage
	{
		GLSL,
		HLSL,
		SPIRV
	};

	enum class ShaderStage
	{
		Vertex,
		Fragment,
		Compute,
		Geometry,
		TessellationControl,
		TessellationEvaluation
	};

	struct ShaderDesc
	{
		String SourceString;
		String ShaderName;
		String EntryPoint;
		ShaderLanguage Language;
		ShaderStage Stage;
	};

	class Shader
	{
	public:
		Shader(const ShaderDesc& desc, Renderer* pRenderer);
		~Shader();

		VkShaderModule GetShaderModule() { return mShaderModule; }
		VkDevice GetDevice() { return mDevice; }

		const String& GetShaderName() { return mShaderName; }
		const String& GetEntryPoint() { return mEntryPoint; }
		ShaderStage GetStage() { return mStage; }
		ShaderLanguage GetLanguage() { return mLanguage; }

	private:
		VkShaderModule mShaderModule;
		VkDevice mDevice;

	private:
		String mShaderName;
		String mEntryPoint;
		ShaderStage mStage;
		ShaderLanguage mLanguage;
		uint32* mSourceSPIRV;
	};
}