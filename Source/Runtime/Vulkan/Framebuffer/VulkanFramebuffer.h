#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Framebuffer/Framebuffer.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanFramebuffer : public Framebuffer
	{
	public:
		VulkanFramebuffer(const FramebufferDesc& desc, SharedPtr<VulkanDevice> device);
		virtual ~VulkanFramebuffer() override;

		VkFramebuffer GetVkFramebuffer() const noexcept { return mFramebuffer; }
		VkDevice GetVkDevice() const noexcept { return mLogicalDevice; }

		virtual void OnShutdown() override;
	private:
		VkFramebuffer mFramebuffer;
		VkDevice mLogicalDevice;
	};
}
