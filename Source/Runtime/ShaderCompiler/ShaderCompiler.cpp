#include "ShaderCompiler.h"

#include <Runtime/ShaderCompiler/ShaderCompilerUtils.h>

#include <spirv_cross/spirv_hlsl.hpp>
#include <spirv_cross/spirv_glsl.hpp>
#include <shaderc/shaderc.hpp>

#include <spirv_reflect.h>

namespace Hollow
{
	void ShaderBlockReadRecursive(SpvReflectBlockVariable& block, ShaderBlockMember& reflectedBlock)
	{
		reflectedBlock.Name = block.name;
		reflectedBlock.Offset = block.offset;
		reflectedBlock.AbsoluteOffset = block.absolute_offset;
		reflectedBlock.Size = block.size;
		reflectedBlock.PaddedSize = block.padded_size;

		for (u32 i = 0; i < block.member_count; ++i)
		{
			SpvReflectBlockVariable& member = block.members[i];
			ShaderBlockMember reflectedMember = {};
			ShaderBlockReadRecursive(member, reflectedMember);
			reflectedBlock.Members.push_back(reflectedMember);
		}
	}

	bool ShaderCompiler::CompileShaderToSPIRV(const MemoryBuffer& view, const String& entryMethodName, const ShaderStage stage, const ShaderLanguage language, SharedPtr<MemoryOwnedBuffer>& pViewOut, String& errorMessageOut)
	{
		shaderc::Compiler vkCompiler;
		shaderc::CompileOptions compileOptions;

		// Set the source language
		compileOptions.SetSourceLanguage(ShaderCompilerUtils::GetLanguage(language));
		compileOptions.SetSuppressWarnings();

		shaderc::PreprocessedSourceCompilationResult preprocessedResult = vkCompiler.PreprocessGlsl(static_cast<const char*>(view.GetData()),
			ShaderCompilerUtils::GetShaderKind(stage), entryMethodName.c_str(), compileOptions);

		if (preprocessedResult.GetCompilationStatus() != shaderc_compilation_status_success)
		{
			errorMessageOut = preprocessedResult.GetErrorMessage();
			pViewOut = nullptr;
			return false;
		}

		shaderc::CompilationResult result = vkCompiler.CompileGlslToSpv(static_cast<const char*>(view.GetData()), ShaderCompilerUtils::GetShaderKind(stage), "", compileOptions);

		if (result.GetCompilationStatus() != shaderc_compilation_status_success)
		{
			errorMessageOut = result.GetErrorMessage();
			pViewOut = nullptr;
			return false;
		}

		const u64 size = (byte*)result.end() - (byte*)result.begin();
		byte* data = new byte[size];
		memcpy(data, result.begin(), size);

		pViewOut = std::make_shared<MemoryOwnedBuffer>(std::move(data), size);
		CORE_ASSERT(pViewOut->GetSize() > 0, "ShaderCompiler", "Failed to compile shader to SPIRV");
		return true;
	}

	bool ShaderCompiler::ReflectShader(SharedPtr<MemoryOwnedBuffer>& buffer, SharedPtr<ShaderReflection>& reflectedData)
	{
		reflectedData = std::make_shared<ShaderReflection>();

		SpvReflectShaderModule module = {};

		if (spvReflectCreateShaderModule(buffer->GetSize(), buffer->GetData(), &module) != SPV_REFLECT_RESULT_SUCCESS)
		{
			CORE_LOG(HE_WARNING, "ShaderCompiler", "Failed to reflect shader");
			return false;
		}

		u32 count = 0;

		// Catch descriptor sets
		CORE_ASSERT(spvReflectEnumerateDescriptorSets(&module, &count, nullptr) == SPV_REFLECT_RESULT_SUCCESS, "ShaderCompiler", "Failed to enumerate descriptor sets");
		ArrayList<SpvReflectDescriptorSet*> descriptorSets(count);
		CORE_ASSERT(spvReflectEnumerateDescriptorSets(&module, &count, descriptorSets.data()) == SPV_REFLECT_RESULT_SUCCESS, "ShaderCompiler", "Failed to enumerate descriptor sets");

		for (u32 setIndex = 0; setIndex < count; ++setIndex)
		{
			SpvReflectDescriptorSet* set = descriptorSets[setIndex];
			ShaderDescriptorSet setData = {};
			setData.SetIndex = set->set;

			for (u32 bindingIndex = 0; bindingIndex < set->binding_count; ++bindingIndex)
			{
				SpvReflectDescriptorBinding* binding = set->bindings[bindingIndex];

				ShaderDescriptorBinding bindingData = {};
				bindingData.BindIndex = binding->binding;
				bindingData.Type = ShaderCompilerUtils::GetSpvDescriptorType(binding->descriptor_type);
				bindingData.Name = binding->name;

				for (u32 blockIndex = 0; blockIndex < binding->block.member_count; blockIndex++)
				{
					SpvReflectBlockVariable& member = binding->block.members[blockIndex];
					ShaderBlockMember blockMember = {};
					ShaderBlockReadRecursive(member, blockMember);

					bindingData.BlockMember = blockMember;
				}

				setData.Bindings.push_back(bindingData);
			}

			reflectedData->AddDescriptorData(setData);
		}

		// Catch input variables
		CORE_ASSERT(spvReflectEnumerateInputVariables(&module, &count, nullptr) == SPV_REFLECT_RESULT_SUCCESS, "ShaderCompiler", "Failed to enumerate input variables");
		ArrayList<SpvReflectInterfaceVariable*> inputVariables(count);
		CORE_ASSERT(spvReflectEnumerateInputVariables(&module, &count, inputVariables.data()) == SPV_REFLECT_RESULT_SUCCESS, "ShaderCompiler", "Failed to enumerate input variables");

		for (u32 i = 0; i < count; ++i)
		{
			SpvReflectInterfaceVariable* variable = inputVariables[i];
			ShaderIODescriptor descriptor = {};
			descriptor.Name = variable->name;
			descriptor.FormatType;

			reflectedData->AddInputDescriptor(descriptor);
		}

		// Catch output variables
		CORE_ASSERT(spvReflectEnumerateOutputVariables(&module, &count, nullptr) == SPV_REFLECT_RESULT_SUCCESS, "ShaderCompiler", "Failed to enumerate output variables");
		ArrayList<SpvReflectInterfaceVariable*> outputVariables(count);
		CORE_ASSERT(spvReflectEnumerateOutputVariables(&module, &count, outputVariables.data()) == SPV_REFLECT_RESULT_SUCCESS, "ShaderCompiler", "Failed to enumerate output variables");

		for (u32 i = 0; i < count; ++i)
		{
			SpvReflectInterfaceVariable* variable = outputVariables[i];
			ShaderIODescriptor descriptor = {};
			descriptor.Name = variable->name;
			descriptor.FormatType; variable->format;

			reflectedData->AddOutputDescriptor(descriptor);
		}

		CORE_ASSERT(spvReflectEnumeratePushConstants(&module, &count, nullptr) == SPV_REFLECT_RESULT_SUCCESS, "ShaderCompiler", "Failed to enumerate push constants");
		ArrayList<SpvReflectBlockVariable*> pushConstants(count);
		CORE_ASSERT(spvReflectEnumeratePushConstants(&module, &count, pushConstants.data()) == SPV_REFLECT_RESULT_SUCCESS, "ShaderCompiler", "Failed to enumerate push constants");

		return true;
	}
}
