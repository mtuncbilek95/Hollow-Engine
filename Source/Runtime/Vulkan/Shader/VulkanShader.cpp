#include "VulkanShader.h"

//#include <Runtime/ShaderCompiler/ShaderCompiler.h>

namespace Hollow
{
	VulkanShader::VulkanShader(const ShaderDesc& desc, VulkanDevice* pDevice) : Shader(desc, pDevice), mVkShaderModule(VK_NULL_HANDLE),
		mVkDevice(pDevice->GetVkDevice())
	{
		uint32 byteSizeOut = 0;
		string errorMessageOut;

		// Compile the shader to SPIRV
		//CORE_ASSERT(ShaderCompiler::CompileShaderToSPIRV(desc.Source, desc.EntryPoint, desc.Stage, desc.Language, &mShaderSPIRV, byteSizeOut, errorMessageOut), "VulkanShader",
			//"Failed to compile shader: %s", errorMessageOut.c_str());

		// Create the shader module
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = byteSizeOut;
		createInfo.pCode = mShaderSPIRV;
		createInfo.flags = 0;
		createInfo.pNext = nullptr;

		CORE_ASSERT(vkCreateShaderModule(mVkDevice, &createInfo, nullptr, &mVkShaderModule) == VK_SUCCESS, "VulkanShader", "Failed to create shader module");

		CORE_LOG(HE_INFO, "VulkanShader", "%s compiled successfully", desc.ShaderName.c_str());
	}

	void VulkanShader::OnShutdown() noexcept
	{
		if (mShaderSPIRV != nullptr)
		{
			delete[] mShaderSPIRV;
			mShaderSPIRV = nullptr;
		}

		if (mVkShaderModule != VK_NULL_HANDLE)
			vkDestroyShaderModule(mVkDevice, mVkShaderModule, nullptr);

		mVkShaderModule = VK_NULL_HANDLE;
		mVkDevice = VK_NULL_HANDLE;

		CORE_LOG(HE_INFO, "VulkanShader", "Shader shutdown successfully");
	}
}