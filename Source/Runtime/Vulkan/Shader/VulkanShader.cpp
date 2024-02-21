#include "VulkanShader.h"

namespace Hollow
{
	VulkanShader::VulkanShader(const ShaderDesc& desc, VulkanDevice* pDevice) : Shader(desc, pDevice)
	{
		mVkLogicalDevice = pDevice->GetVkLogicalDevice();

		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = desc.Source.size();
		createInfo.pCode = reinterpret_cast<const uint32*>(desc.Source.data());

		DEV_ASSERT(vkCreateShaderModule(mVkLogicalDevice, &createInfo, nullptr, &mVkShaderModule) == VK_SUCCESS,
			"VulkanShader", "Failed to create shader module!");
	}

	void VulkanShader::OnShutdown() noexcept
	{
		if(mVkShaderModule != VK_NULL_HANDLE)
		{
			vkDestroyShaderModule(mVkLogicalDevice, mVkShaderModule, nullptr);
			mVkShaderModule = VK_NULL_HANDLE;
		}
	}
}