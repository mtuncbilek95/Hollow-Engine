#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Adapter/GraphicsAdapter.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanAdapter : public GraphicsAdapter
	{
	public:
		VulkanAdapter(const GraphicsAdapterDesc& desc, const VkPhysicalDevice device);
		~VulkanAdapter() override = default;

		const VkPhysicalDevice& GetVkPhysicalDevice() const noexcept { return mVkPhysicalDevice; }

		// Inherited via GraphicsAdapter
		virtual void OnShutdown() override;

		SharedPtr<GraphicsDevice> CreateDeviceCore(const GraphicsDeviceDesc& desc) override;
	private:
		VkPhysicalDevice mVkPhysicalDevice;
	};
}
