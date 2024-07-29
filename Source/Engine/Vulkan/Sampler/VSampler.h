#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Sampler/Sampler.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	class VDevice;
	class VSampler : public Sampler
	{
	public:
		VSampler(const SamplerDesc& desc, SharedPtr<VDevice> pDevice);
		virtual ~VSampler() override = default;

		VkSampler GetVkSampler() const { return mSampler; }
		VkPhysicalDevice GetVkPhysicalDevice() const { return mAdapter; }
		VkDevice GetVkDevice() const { return mDevice; }

	private:
		VkDevice mDevice;
		VkPhysicalDevice mAdapter;
		VkSampler mSampler;
	};

}
