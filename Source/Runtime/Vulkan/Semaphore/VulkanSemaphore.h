#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Semaphore/Semaphore.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanSemaphore final : public Semaphore
	{
	public:
		VulkanSemaphore(const SharedPtr<VulkanDevice>& pDevice);
		~VulkanSemaphore() override = default;

		VkSemaphore GetVkSemaphore() const { return mVkSemaphore; }
		VkDevice GetVkDevice() const { return mVkDevice; }

		virtual void OnShutdown() noexcept override;

	private:
		VkSemaphore mVkSemaphore;
		VkDevice mVkDevice;
	};
}
