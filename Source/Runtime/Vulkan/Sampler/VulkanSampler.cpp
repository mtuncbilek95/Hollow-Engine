#include "VulkanSampler.h"

#include <Runtime/Vulkan/Core/VulkanCoreUtils.h>
#include <Runtime/Vulkan/Sampler/VulkanSamplerUtils.h>

namespace Hollow
{
	VulkanSampler::VulkanSampler(const SamplerDesc& desc, SharedPtr<VulkanDevice> pDevice) : Sampler(desc, pDevice), mVkSampler(VK_NULL_HANDLE), 
		mVkDevice(pDevice->GetVkDevice()), mVkPhysicalDevice(pDevice->GetVkPhysicalDevice())
	{
		VkPhysicalDeviceProperties props;
		vkGetPhysicalDeviceProperties(mVkPhysicalDevice, &props);

		if (props.limits.maxSamplerAnisotropy < desc.MaxAnisotropy)
			SetMaxAnisotropy(props.limits.maxSamplerAnisotropy);

		VkSamplerCreateInfo samplerInfo = {};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VulkanSamplerUtils::GetVkFilter(desc.MagFilter);
		samplerInfo.minFilter = VulkanSamplerUtils::GetVkFilter(desc.MinFilter);
		samplerInfo.addressModeU = VulkanSamplerUtils::GetVkAddressMode(desc.UAddressMode);
		samplerInfo.addressModeV = VulkanSamplerUtils::GetVkAddressMode(desc.VAddressMode);
		samplerInfo.addressModeW = VulkanSamplerUtils::GetVkAddressMode(desc.WAddressMode);
		samplerInfo.anisotropyEnable = desc.bAnisotropyEnabled;
		samplerInfo.maxAnisotropy = GetMaxAnisotropy();
		samplerInfo.borderColor = VulkanSamplerUtils::GetVkBorderColor(desc.BorderColor);
		samplerInfo.compareEnable = desc.bCompareEnabled;
		samplerInfo.compareOp = VulkanCoreUtils::GetVkCompareOperation(desc.CompareOp);
		samplerInfo.mipmapMode = VulkanSamplerUtils::GetVkMipmapMode(desc.MapMode);
		samplerInfo.mipLodBias = desc.MipLODBias;
		samplerInfo.minLod = desc.MinLOD;
		samplerInfo.maxLod = desc.MaxLOD;

		CORE_ASSERT(vkCreateSampler(mVkDevice, &samplerInfo, nullptr, &mVkSampler) == VK_SUCCESS, "VulkanSampler", "Failed to create sampler!");
	}

	VulkanSampler::~VulkanSampler()
	{
		if (mVkSampler != VK_NULL_HANDLE)
		{
			vkDestroySampler(mVkDevice, mVkSampler, nullptr);
			mVkSampler = VK_NULL_HANDLE;
		}

		mVkPhysicalDevice = VK_NULL_HANDLE;
		mVkDevice = VK_NULL_HANDLE;
	}
}
