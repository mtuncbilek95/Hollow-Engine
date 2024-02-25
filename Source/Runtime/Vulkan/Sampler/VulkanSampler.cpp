#include "VulkanSampler.h"

#include <Runtime/Vulkan/Common/VulkanCommonUtils.h>
#include <Runtime/Vulkan/Sampler/VulkanSamplerUtils.h>

namespace Hollow
{
	VulkanSampler::VulkanSampler(const SamplerDesc& desc, VulkanDevice* pDevice) : Sampler(desc, pDevice)
	{
		mVkLogicalDevice = pDevice->GetVkLogicalDevice();

		VkSamplerCreateInfo samplerInfo = {};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VulkanSamplerUtils::GetVkFiltering(desc.MagFilter);
		samplerInfo.minFilter = VulkanSamplerUtils::GetVkFiltering(desc.MinFilter);
		samplerInfo.mipmapMode = VulkanSamplerUtils::GetVkMapMode(desc.MipmapMode);
		samplerInfo.addressModeU = VulkanSamplerUtils::GetVkAddressMode(desc.AddressModeU);
		samplerInfo.addressModeV = VulkanSamplerUtils::GetVkAddressMode(desc.AddressModeV);
		samplerInfo.addressModeW = VulkanSamplerUtils::GetVkAddressMode(desc.AddressModeW);
		samplerInfo.mipLodBias = desc.MipLodBias;
		samplerInfo.anisotropyEnable = desc.MaxAnisotropy != 0;
		samplerInfo.maxAnisotropy = desc.MaxAnisotropy;
		samplerInfo.compareEnable = desc.bComparisonEnabled;
		samplerInfo.compareOp = VulkanCommonUtils::GetVkCompareOperation(desc.CompareOperation);
		samplerInfo.minLod = desc.MinLod;
		samplerInfo.maxLod = desc.MaxLod;
		samplerInfo.borderColor = VulkanSamplerUtils::GetVkBorderColor(desc.BorderColor);
		samplerInfo.unnormalizedCoordinates = false;

		DEV_ASSERT(vkCreateSampler(mVkLogicalDevice, &samplerInfo, nullptr, &mVkSampler) == VK_SUCCESS, "VulkanSampler", "Failed to create sampler");
	}

	void VulkanSampler::OnShutdown() noexcept
	{
		if (mVkSampler != VK_NULL_HANDLE)
		{
			vkDestroySampler(mVkLogicalDevice, mVkSampler, nullptr);
			mVkSampler = VK_NULL_HANDLE;
		}
	}
}