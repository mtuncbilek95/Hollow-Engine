#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Instance/GraphicsInstance.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanInstance final: public GraphicsInstance
	{
	public:
		VulkanInstance(const GraphicsInstanceDesc& desc);
		~VulkanInstance() override = default;

		VkInstance GetVkInstance() const { return mVkInstance; }
		virtual void OnShutdown() override;

	protected:
		virtual void EnumerateAdaptersImpl() override;

	private:
		VkInstance mVkInstance;
		ArrayList<const char*> mInstanceLayers;

#if defined(HOLLOW_DEBUG)
		VkDebugUtilsMessengerEXT mVkDebugMessenger;
#endif
	};
}
