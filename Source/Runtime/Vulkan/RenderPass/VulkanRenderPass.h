#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/RenderPass/RenderPass.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanRenderPass : public RenderPass
	{
	public:
		VulkanRenderPass(const RenderPassDesc& desc, SharedPtr<VulkanDevice> device);
		virtual ~VulkanRenderPass() override;

		VkRenderPass GetVkRenderPass() const noexcept { return mRenderPass; }
		VkDevice GetVkDevice() const noexcept { return mLogicalDevice; }

	private:
		VkRenderPass mRenderPass;
		VkDevice mLogicalDevice;
	};
}
