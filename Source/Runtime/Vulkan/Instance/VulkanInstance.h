#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Instance/GraphicsInstance.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanInstance : public GraphicsInstance
	{
	public:
		VulkanInstance(const GraphicsInstanceDesc& desc);
		~VulkanInstance() override = default;

		const VkInstance& GetVkInstance() const noexcept { return mVkInstance; }

		virtual void OnShutdown() override;
	private:
		static PFN_vkCmdBeginRendering vkCmdBeginRendering;
		static PFN_vkCmdEndRendering vkCmdEndRendering;

	private:
		VkInstance mVkInstance;
		Array<const char*> mInstanceLayers;

#ifdef HOLLOW_DEBUG
		VkDebugUtilsMessengerEXT mDebugMessenger;
#endif
	};
}
