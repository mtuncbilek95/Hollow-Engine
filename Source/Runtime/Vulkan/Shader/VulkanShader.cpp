#include "VulkanShader.h"

namespace Hollow
{
	VulkanShader::VulkanShader(const ShaderDesc& desc, SharedPtr<VulkanDevice> device) : Shader(desc, device), mVkDevice(device->GetVkDevice())
	{

		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = desc.ShaderCode.GetSize();
		createInfo.pCode = static_cast<const u32*>(desc.ShaderCode.GetData());
		createInfo.flags = 0;

		CORE_ASSERT(vkCreateShaderModule(mVkDevice, &createInfo, nullptr, &mVkShaderModule) == VK_SUCCESS, "VulkanShader", "Failed to create shader module!");
	}

	void VulkanShader::OnShutdown() noexcept
	{
		vkDestroyShaderModule(mVkDevice, mVkShaderModule, nullptr);
		mVkShaderModule = VK_NULL_HANDLE;
		mVkDevice = VK_NULL_HANDLE;

		CORE_LOG(HE_INFO, "VulkanShader", "Shader shutdown successfully");
	}
}
