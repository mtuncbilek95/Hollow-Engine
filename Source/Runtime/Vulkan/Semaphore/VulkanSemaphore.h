#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Semaphore/Semaphore.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanSemaphore : public Semaphore
	{
	public:
		VulkanSemaphore(VulkanDevice* pDevice);
		~VulkanSemaphore() override = default;

		VkSemaphore GetVkSemaphore() { return mVkSemaphore; }
		VkDevice GetVkDevice() { return mVkDevice; }

		// Inherited via Swapchain
		void OnShutdown() noexcept override;

	private:
		VkSemaphore mVkSemaphore;
		VkDevice mVkDevice;
	};
}