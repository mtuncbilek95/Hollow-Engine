#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Sampler/Sampler.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanSampler : public Sampler
	{
	public:
		VulkanSampler(const SamplerDesc& desc, VulkanDevice* pDevice);
		~VulkanSampler() override = default;

		VkSampler GetVkSampler() const { return mVkSampler; }

		void OnShutdown() noexcept override;

	private:
		VkSampler mVkSampler;
		VkDevice mVkLogicalDevice;
	};
}