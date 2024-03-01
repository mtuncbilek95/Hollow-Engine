#include "Shader.h"

#include <ShaderCompiler/ShaderCompiler.h>

namespace MiniVk
{
	Shader::Shader(const ShaderDesc& desc, Renderer* pRenderer) : mSourceSPIRV(nullptr), mShaderName(desc.ShaderName), mEntryPoint(desc.EntryPoint),
		mLanguage(desc.Language), mStage(desc.Stage), mDevice(VK_NULL_HANDLE), mShaderModule(VK_NULL_HANDLE)
	{
		mDevice = pRenderer->GetVkLogicalDevice();

		uint32 bytesSizeOut = 0;
		String errorMessageOut;

		ShaderCompiler::CompileShaderToSPIRV(desc.SourceString, desc.EntryPoint, desc.Stage, desc.Language, &mSourceSPIRV, bytesSizeOut, errorMessageOut);

		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = bytesSizeOut;
		createInfo.pCode = reinterpret_cast<const uint32*>(mSourceSPIRV);

		DEV_ASSERT(vkCreateShaderModule(mDevice, &createInfo, nullptr, &mShaderModule) == VK_SUCCESS, "Shader", "Failed to create shader module");

		CORE_LOG(HE_VERBOSE, desc.ShaderName.c_str(), "Shader Module Created!", );
	}

	Shader::~Shader()
	{
		vkDestroyShaderModule(mDevice, mShaderModule, nullptr);
	}
}