#include "VShader.h"
#include "VShaderUtils.h"

#include <Engine/Vulkan/Device/VDevice.h>

namespace Hollow
{
	VShader::VShader(const ShaderDesc& desc, SharedPtr<VDevice> pDevice) : Shader(desc, pDevice), mDevice(pDevice->GetVkDevice())
	{
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = desc.ShaderCode.GetSize();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(desc.ShaderCode.GetData());
		createInfo.flags = 0;

		CORE_ASSERT(vkCreateShaderModule(mDevice, &createInfo, nullptr, &mShaderModule) == VK_SUCCESS, "VShader", "Failed to create shader module");
	}
}
