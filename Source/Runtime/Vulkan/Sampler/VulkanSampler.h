#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Sampler/Sampler.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanSampler final : public Sampler
	{
	public:
		VulkanSampler(const SamplerDesc& desc, SharedPtr<VulkanDevice> pDevice);
		~VulkanSampler() override = default;

		VkSampler GetVkSampler() const { return mVkSampler; }
		VkPhysicalDevice GetVkPhysicalDevice() const { return mVkPhysicalDevice; }
		VkDevice GetVkDevice() const { return mVkDevice; }

		virtual void OnShutdown() noexcept override;

	private:
		VkDevice mVkDevice;
		VkPhysicalDevice mVkPhysicalDevice;
		VkSampler mVkSampler;
	};
}
