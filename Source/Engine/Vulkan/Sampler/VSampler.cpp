#include "VSampler.h"
#include "VSamplerUtils.h"
#include "../Core/VCoreUtils.h"

#include <Engine/Vulkan/Device/VDevice.h>

namespace Hollow
{
	VSampler::VSampler(const SamplerDesc& desc, SharedPtr<VDevice> pDevice) : Sampler(desc, pDevice), mDevice(pDevice->GetVkDevice()), mAdapter(pDevice->GetVkAdapter())
	{
		VkSamplerCreateInfo samplerInfo = {};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = Utils::GetVkFilter(desc.MagFilter);
		samplerInfo.minFilter = Utils::GetVkFilter(desc.MinFilter);
		samplerInfo.addressModeU = Utils::GetVkAddressMode(desc.AddressMode);
		samplerInfo.addressModeV = Utils::GetVkAddressMode(desc.AddressMode);
		samplerInfo.addressModeW = Utils::GetVkAddressMode(desc.AddressMode);
		samplerInfo.anisotropyEnable = desc.bAnisotropyEnabled;
		samplerInfo.maxAnisotropy = desc.MaxAnisotropy;
		samplerInfo.borderColor = Utils::GetVkBorderColor(desc.BorderColor);
		samplerInfo.compareEnable = desc.bCompareEnabled;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareOp = Utils::GetVkCompareOp(desc.CompareOp);
		samplerInfo.mipmapMode = Utils::GetVkMipmapMode(desc.MapMode);
		samplerInfo.mipLodBias = desc.MipLODBias;
		samplerInfo.minLod = desc.MinLOD;
		samplerInfo.maxLod = desc.MaxLOD;

		CORE_ASSERT(vkCreateSampler(mDevice, &samplerInfo, nullptr, &mSampler) == VK_SUCCESS, "VSampler", "Failed to create sampler!");
	}
}
